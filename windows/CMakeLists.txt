cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "file_open")
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(file_open_plugin SHARED
  "file_open/FileOpenPlugin.cpp"
  "file_open/FileOpenPluginCApi.cpp"
  "file_open/messages.g.cpp"
)

apply_standard_settings(file_open_plugin)
target_compile_options(file_open_plugin PRIVATE /WX-)
set_target_properties(file_open_plugin PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  OUTPUT_NAME "file_open"
  WINDOWS_EXPORT_ALL_SYMBOLS YES
)

target_compile_definitions(file_open_plugin PRIVATE FLUTTER_PLUGIN_IMPL)

target_link_libraries(file_open_plugin PRIVATE flutter)
target_link_libraries(file_open_plugin PRIVATE flutter_wrapper_plugin)

# Expose the plugin root (which contains the 'file_open' folder) publicly so
# consuming apps can resolve includes like <file_open/file_open_plugin.h>.
# Public headers now live under windows/include
target_include_directories(file_open_plugin
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/file_open"
)
