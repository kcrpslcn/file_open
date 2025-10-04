This example does not include a checked-in Windows runner by default.

To enable and run the example on Windows from a Windows machine:

1. On a Windows machine, open a terminal in `example/` and run:

   flutter create --platforms=windows .

2. Generate Pigeon C++ bindings (on either platform) and copy them to
   `windows/runner/messages.g.cpp` and `windows/runner/messages.g.h`. You can
   run the helper script from the plugin root on a machine with the Dart SDK:

   ../../tool/generate_pigeon_windows.sh

3. Build and run the example on Windows:

   flutter run -d windows

Notes:
- The plugin already contains a Windows implementation under `windows/` that
  registers a `FileOpenHandlerPlugin` and uses WM_COPYDATA to receive file
  open events from secondary instances. The Pigeon-generated C++ files in
  `windows/runner/` are used to call back into Dart when files are opened.
