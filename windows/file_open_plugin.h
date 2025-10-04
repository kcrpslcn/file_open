#ifndef FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_H_
#define FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include "runner/messages.g.h"

#include <memory>
#include <string>
#include <vector>

class FileOpenPlugin : public flutter::Plugin {
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
  std::unique_ptr<pigeon::FileOpenHandlerFlutterApi> flutter_api_;

  void CreateMessageWindow();
  void DestroyMessageWindow();
  void EmitFiles(const std::vector<std::string>& files);

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) noexcept;
};

#endif  // FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_H_
