[![Pub Package](https://img.shields.io/pub/v/file_open.svg)](https://pub.dev/packages/file_open)

# file_open
Flutter plugin to receive file open events on macOS and Windows.

## Requirements
- macOS 10.15 or later
- Windows 10 or later

## API
- `FileOpen.onOpened`: Stream of lists of URIs opened by the user. 
Each event contains one or more URIs.
- `FileOpen.setLoggingEnabled(bool)`: Enable or disable logging on the native platform. 
Logging is enabled by default.

## Important Notes
The `FileOpen` instance is lazily created. 
To receive initial file open events that launched the app, 
ensure to access `FileOpen.onOpened` or call any methods early in your app's lifecycle, 
such as in `main()` or during widget initialization. 
Accessing it too late may miss the initial files.

## Platform Setup
### macOS
To handle file open events, configure your app's `Info.plist` with the appropriate keys:

- **CFBundleDocumentTypes**: Declares the document types your app can open. 
Each entry specifies file extensions, MIME types, and Uniform Type Identifiers (UTIs). 
This tells macOS which files your app can handle, triggering open events when users double-click or open such files. 
See [Apple's documentation](https://developer.apple.com/documentation/bundleresources/information_property_list/cfbundledocumenttypes) for details.
- **CFBundleURLTypes**: Declares URL schemes your app can handle, such as custom protocols (e.g., `myapp://`). 
When URLs with these schemes are opened, the app receives the event. 
See [Apple's documentation](https://developer.apple.com/documentation/bundleresources/information_property_list/cfbundleurltypes) for details.
- **UTExportedTypeDeclarations**: Declares custom UTIs that your app defines and exports. 
Use this when your app creates its own file types not covered by standard system UTIs, allowing other apps to recognize and potentially open them. 
See [Apple's documentation](https://developer.apple.com/documentation/bundleresources/information_property_list/utexportedtypedeclarations) for details.

### Windows
Windows support is automatically configured when you add the plugin to your Flutter project. The plugin handles file open events through command line arguments when files are associated with your application. No additional configuration is required in most cases.

## Example
The example app in this repository demonstrates registering a custom file extension `.abcde` 
in its [Info.plist](example/macos/Runner/Info.plist) for macOS to handle file open events. 
Windows support works automatically through the plugin without additional configuration.

## Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

## License
This project is licensed under the MIT License.