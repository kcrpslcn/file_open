#include <flutter/dart_project.h>
#include <flutter/flutter_view_controller.h>
#include <windows.h>

#include "flutter_window.h"
#include "utils.h"

#include <codecvt>
#include <memory>
#include <string>
#include <vector>

namespace {

constexpr wchar_t kMutexName[] = L"Global\\FileOpenHandlerSingletonMutex";
constexpr wchar_t kWndClassName[] = L"FileOpenHandlerMessageWindow";

HWND FindExistingWindow() {
  return ::FindWindowW(kWndClassName, L"");
}

}  // namespace

int APIENTRY wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev,
                      _In_ wchar_t *command_line, _In_ int show_command) {
  // Attach to console when present (e.g., 'flutter run') or create a
  // new console when running with a debugger.
  if (!::AttachConsole(ATTACH_PARENT_PROCESS) && ::IsDebuggerPresent()) {
    CreateAndAttachConsole();
  }

  // Initialize COM, so that it is available for use in the library and/or
  // plugins.
  ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

  // Check for single instance
  HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, kMutexName);
  if (hMutex != nullptr) {
    ::CloseHandle(hMutex);
    // Secondary instance, forward args and exit
    std::vector<std::string> command_line_arguments = GetCommandLineArguments();
    if (!command_line_arguments.empty()) {
      HWND target = FindExistingWindow();
      if (target) {
        std::string buf;
        for (size_t i = 0; i < command_line_arguments.size(); ++i) {
          buf.append(command_line_arguments[i]);
          buf.push_back('\0');
        }
        buf.push_back('\0');
        COPYDATASTRUCT cds{};
        cds.dwData = 1;
        cds.cbData = static_cast<DWORD>(buf.size());
        cds.lpData = (PVOID)buf.data();
        ::SendMessageW(target, WM_COPYDATA, 0, (LPARAM)&cds);
      }
    }
    return EXIT_SUCCESS;
  }

  flutter::DartProject project(L"data");

  std::vector<std::string> command_line_arguments =
      GetCommandLineArguments();

  project.set_dart_entrypoint_arguments(std::move(command_line_arguments));

  FlutterWindow window(project);
  Win32Window::Point origin(10, 10);
  Win32Window::Size size(1280, 720);
  if (!window.Create(L"file_open_example", origin, size)) {
    return EXIT_FAILURE;
  }
  window.SetQuitOnClose(true);

  ::MSG msg;
  while (::GetMessage(&msg, nullptr, 0, 0)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  ::CoUninitialize();
  return EXIT_SUCCESS;
}
