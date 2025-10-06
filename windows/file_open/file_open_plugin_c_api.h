#ifndef FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_C_API_H_
#define FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_C_API_H_

#include <flutter_plugin_registrar.h>

#ifdef __cplusplus
extern "C" {
#endif

void FileOpenPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar);

// Some registrants expect a C-API suffixed symbol; provide it to be safe.
void FileOpenPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // FLUTTER_PLUGIN_FILE_OPEN_PLUGIN_C_API_H_
