#define FLUTTER_PLUGIN_IMPLEMENTATION
#include "file_open/file_open_plugin.h"

#include <Windows.h>

#include <flutter/event_channel.h>
#include <flutter/event_sink.h>
#include <flutter/event_stream_handler_functions.h>
#include <flutter/plugin_registrar_windows.h>

#include <codecvt>
#include <memory>
#include <string>
#include <vector>

namespace {

std::vector<std::wstring> GetCommandLineArgsW() {
  int argc = 0;
  LPWSTR* argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
  std::vector<std::wstring> args;
  if (argv) {
    for (int i = 1; i < argc; ++i) {  // skip program path
      args.emplace_back(argv[i]);
    }
    ::LocalFree(argv);
  }
  return args;
}

std::string WStringToUtf8(const std::wstring& w) {
  if (w.empty()) return {};
  int size_needed = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), NULL, 0, NULL, NULL);
  std::string strTo( size_needed, 0 );
  WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), &strTo[0], size_needed, NULL, NULL);
  return strTo;
}

}  // namespace

namespace {
constexpr wchar_t kMutexName[] = L"Global\\FileOpenHandlerSingletonMutex";
constexpr wchar_t kWndClassName[] = L"FileOpenHandlerMessageWindow";
constexpr UINT kMsgId = WM_APP + 1001;  // not used; we'll use WM_COPYDATA

struct CopyDataPayload {
  // Just UTF-8 data with NUL terminator(s) between multiple paths.
};

HWND FindExistingWindow() {
  return ::FindWindowW(kWndClassName, L"");
}

std::wstring Utf8ToWString(const std::string& s) {
  if (s.empty()) return L"";
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0);
  std::wstring w(size_needed, 0);
  MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &w[0], size_needed);
  return w;
}

}  // namespace

bool FileOpenPlugin::logging_enabled_ = true;

void FileOpenPlugin::DebugLog(const std::wstring& message) {
  if (!logging_enabled_) {
    return;
  }
  std::wstring line = message;
  if (line.empty() || line.back() != L'\n') {
    line.append(L"\n");
  }
  ::OutputDebugStringW(line.c_str());
}

void FileOpenPlugin::DebugLogUtf8(const std::string& message) {
  DebugLog(Utf8ToWString(message));
}

FileOpenPlugin::FileOpenPlugin() {}
FileOpenPlugin::~FileOpenPlugin() { DestroyMessageWindow(); if (instance_mutex_) { CloseHandle(instance_mutex_); instance_mutex_ = nullptr; } }

// static
void FileOpenPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto plugin = std::make_unique<FileOpenPlugin>();

  DebugLogUtf8("[FileOpenPlugin] Registering Windows implementation");

  // Enforce single-instance using a named mutex.
  plugin->instance_mutex_ = ::CreateMutexW(nullptr, TRUE, kMutexName);
  const DWORD err = GetLastError();
  const bool is_secondary = (plugin->instance_mutex_ == nullptr) || (err == ERROR_ALREADY_EXISTS);

  // Collect initial files from command line args.
  std::vector<std::string> initial_utf8;
  for (const auto& warg : GetCommandLineArgsW()) {
    initial_utf8.push_back(WStringToUtf8(warg));
  }

  if (is_secondary) {
    DebugLogUtf8("[FileOpenPlugin] Secondary instance detected; forwarding arguments");
    // Forward to primary instance via WM_COPYDATA and exit early (no plugin registration retained).
    HWND target = FindExistingWindow();
    if (target && !initial_utf8.empty()) {
      // Build a single UTF-8 buffer with NUL separators and double-NUL terminator.
      std::string buf;
      for (size_t i = 0; i < initial_utf8.size(); ++i) {
        buf.append(initial_utf8[i]);
        buf.push_back('\0');
      }
      buf.push_back('\0');
      COPYDATASTRUCT cds{};
      cds.dwData = 1;  // our protocol version/type
      cds.cbData = static_cast<DWORD>(buf.size());
      cds.lpData = (PVOID)buf.data();
      // Send synchronously; ignore result.
      SendMessageW(target, WM_COPYDATA, 0, (LPARAM)&cds);
    }
    // Do not add the plugin; return to avoid conflicting instances.
    return;
  }

  // Primary instance continues: capture initial files.
  plugin->initial_files_ = std::move(initial_utf8);

  DebugLogUtf8("[FileOpenPlugin] Primary instance ready");

  auto messenger = registrar->messenger();

  // Initialize Pigeon Flutter API to call back into Dart when files are opened.
  plugin->flutter_api_ = std::make_unique<pigeon::FileOpenFlutterApi>(messenger);

  // Emit initial files via Pigeon
  if (!plugin->initial_files_.empty()) {
    plugin->EmitFiles(plugin->initial_files_);
    plugin->initial_files_.clear();
  }

  // Host API for calls originating from Dart.
  pigeon::FileOpenHostApi::SetUp(messenger, plugin.get());

  // Method channel for getInitialFiles
  auto method_channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      messenger, "file_open/methods", &flutter::StandardMethodCodec::GetInstance());
  method_channel->SetMethodCallHandler([
      plugin_ptr = plugin.get()](const auto& call, auto result) {
    if (call.method_name() == "getInitialFiles") {
      flutter::EncodableList list;
      for (const auto& s : plugin_ptr->initial_files_) list.emplace_back(s);
      result->Success(list);
    } else {
      result->NotImplemented();
    }
  });

  // Event channel for subsequent events (none yet on Windows without IPC).
  auto event_channel = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
      messenger, "file_open/events", &flutter::StandardMethodCodec::GetInstance());

  auto stream_handler = std::make_unique<flutter::StreamHandlerFunctions<flutter::EncodableValue>>(
      /*on_listen=*/[
        plugin_ptr = plugin.get()
      ](const flutter::EncodableValue* /*args*/, std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& sink)
          -> std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> {
        plugin_ptr->event_sink_ = std::move(sink);
        // Emit any pending events first, then initial files.
        if (!plugin_ptr->pending_opened_.empty()) {
          flutter::EncodableList list;
          for (const auto& s : plugin_ptr->pending_opened_) list.emplace_back(s);
          plugin_ptr->event_sink_->Success(list);
          plugin_ptr->pending_opened_.clear();
        }
        if (!plugin_ptr->initial_files_.empty()) {
          plugin_ptr->EmitFiles(plugin_ptr->initial_files_);
          plugin_ptr->initial_files_.clear();
        }
        return nullptr;
      },
      /*on_cancel=*/[
        plugin_ptr = plugin.get()
      ](const flutter::EncodableValue* /*args*/)
          -> std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> {
        plugin_ptr->event_sink_.reset();
        return nullptr;
      });

  event_channel->SetStreamHandler(std::move(stream_handler));

  plugin->CreateMessageWindow();

  registrar->AddPlugin(std::move(plugin));
}

// Exported symbol
extern "C" FLUTTER_PLUGIN_EXPORT void FileOpenPluginRegisterWithRegistrar(
  FlutterDesktopPluginRegistrarRef registrar) {
  FileOpenPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

void FileOpenPlugin::CreateMessageWindow() {
  WNDCLASSW wc{};
  wc.lpfnWndProc = &FileOpenPlugin::WndProc;
  wc.hInstance = GetModuleHandleW(nullptr);
  wc.lpszClassName = kWndClassName;
  RegisterClassW(&wc);
  // Store this pointer in GWLP_USERDATA for dispatch.
  msg_hwnd_ = CreateWindowExW(0, kWndClassName, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, wc.hInstance, nullptr);
  SetWindowLongPtrW(msg_hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

void FileOpenPlugin::DestroyMessageWindow() {
  if (msg_hwnd_) {
    DestroyWindow(msg_hwnd_);
    msg_hwnd_ = nullptr;
  }
}

void FileOpenPlugin::EmitFiles(const std::vector<std::string>& files) {
  if (files.empty()) return;
  if (logging_enabled_) {
    std::wstring message = L"[FileOpenPlugin] Emitting files";
    for (const auto& s : files) {
      message.append(L" \"");
      message.append(Utf8ToWString(s));
      message.append(L"\"");
    }
    DebugLog(message);
  }
  // Always notify Dart via the Pigeon Flutter API if available. Also keep the
  // existing EventChannel semantics for compatibility.
  if (flutter_api_) {
    // Convert std::vector<std::string> to pigeon::EncodableList (flutter::EncodableList)
    flutter::EncodableList enc_list;
    for (const auto& s : files) enc_list.emplace_back(s);
    // Call the async Flutter API and ignore the completion handler.
    flutter_api_->OnFileOpened(enc_list, /*on_success=*/[]() {}, /*on_error=*/[](const pigeon::FlutterError&){ });
  }

  if (event_sink_) {
    flutter::EncodableList list;
    for (const auto& s : files) list.emplace_back(s);
    event_sink_->Success(list);
  } else {
    pending_opened_.insert(pending_opened_.end(), files.begin(), files.end());
  }
}

LRESULT CALLBACK FileOpenPlugin::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) noexcept {
  if (message == WM_COPYDATA) {
    auto* self = reinterpret_cast<FileOpenPlugin*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (self) {
      auto* cds = reinterpret_cast<COPYDATASTRUCT*>(lparam);
      if (cds && cds->lpData && cds->cbData > 0) {
        const char* data = static_cast<const char*>(cds->lpData);
        const size_t len = cds->cbData;
        std::vector<std::string> files;
        size_t i = 0;
        while (i < len) {
          std::string s(&data[i]);
          if (s.empty()) break;
          files.emplace_back(std::move(s));
          i += files.back().size() + 1;
        }
        self->EmitFiles(files);
      }
      return 0;
    }
  }
  return DefWindowProcW(hwnd, message, wparam, lparam);
}

std::optional<pigeon::FlutterError> FileOpenPlugin::SetLoggingEnabled(bool enabled) {
  logging_enabled_ = enabled;
  const wchar_t* status = enabled ? L"enabled" : L"disabled";
  std::wstring message = L"[FileOpenPlugin] Logging ";
  message.append(status);
  message.append(L" by host request");
  ::OutputDebugStringW((message + L"\n").c_str());
  return std::nullopt;
}
