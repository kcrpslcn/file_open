#include "include/file_open/file_open_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "file_open_plugin.h"

void FileOpenPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  FileOpenPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}

void FileOpenPluginCApiRegisterWithRegistrar(
        FlutterDesktopPluginRegistrarRef registrar) {
    FileOpenPluginRegisterWithRegistrar(registrar);
}
