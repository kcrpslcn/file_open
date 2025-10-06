#include "file_open_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

// Public header moved
#include <file_open/file_open_plugin.h>

void FileOpenPluginCApiRegisterWithRegistrar(
        FlutterDesktopPluginRegistrarRef registrar) {
    FileOpenPluginRegisterWithRegistrar(registrar);
}
