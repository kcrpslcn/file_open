#ifndef FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_H_
#define FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <flutter/event_sink.h>
#include "file_open/messages.g.h"

#include <optional>

#ifdef FLUTTER_PLUGIN_IMPLEMENTATION
#define FLUTTER_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FLUTTER_PLUGIN_EXPORT __declspec(dllimport)
#endif

#include <memory>
#include <string>
#include <vector>

class FileOpenPlugin : public flutter::Plugin, public pigeon::FileOpenHostApi {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  FileOpenPlugin();
  virtual ~FileOpenPlugin();

  // Disallow copy and assign.
  FileOpenPlugin(const FileOpenPlugin&) = delete;
  FileOpenPlugin& operator=(const FileOpenPlugin&) = delete;

 private:
  // Messaging window for primary instance to receive forwarded opens.
  HWND msg_hwnd_ = nullptr;
  HANDLE instance_mutex_ = nullptr;

  std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> event_sink_;
  std::vector<std::string> initial_files_;
  std::vector<std::string> pending_opened_;
  // Pigeon-generated Flutter API to call back into Dart when files are opened.
  std::unique_ptr<pigeon::FileOpenFlutterApi> flutter_api_;

  // Host API implementation entry point.
  std::optional<pigeon::FlutterError> SetLoggingEnabled(bool enabled) override;

  static void DebugLog(const std::wstring& message);
  static void DebugLogUtf8(const std::string& message);
  static bool logging_enabled_;

  void CreateMessageWindow();
  void DestroyMessageWindow();
  void EmitFiles(const std::vector<std::string>& files);

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) noexcept;
};

extern "C" FLUTTER_PLUGIN_EXPORT void FileOpenPluginRegisterWithRegistrar(
  FlutterDesktopPluginRegistrarRef registrar);

#endif  // FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_H_
