cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME "file_open")
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(file_open_plugin SHARED
  "file_open_plugin.cpp"
  "file_open_plugin.h"
  "file_open_plugin_c_api.cpp"
  "include/file_open/messages.g.cpp"
)

apply_standard_settings(file_open_plugin)
set_target_properties(file_open_plugin PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  OUTPUT_NAME "file_open"
)

target_compile_definitions(file_open_plugin PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(file_open_plugin PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(file_open_plugin PRIVATE flutter)
target_link_libraries(file_open_plugin PRIVATE flutter_wrapper_plugin)
