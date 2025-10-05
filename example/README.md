# file_open example

This app demonstrates how the `file_open` plugin receives launch events when a
file with a registered extension is opened from Windows Explorer.

## 🔗 File-type association via MSIX

The example reuses the same `.plidar` and `.loadfile` associations configured in
the `flutter_file_type` sample. The configuration lives in
`pubspec.yaml` under `msix_config`, and the `msix` tool is added as a
development dependency.

To generate an MSIX package with the associations:

```powershell
flutter pub run msix:create
```

You will need a code-signing certificate that matches the
`certificate_subject` in the manifest (update the values if you use your own).

After installation, double-clicking a `.plidar` or `.loadfile` file will bring
the app to the foreground and the `file_open` plugin will surface the launch
event.

## 🙋‍♀️ Need a primer on Flutter?

- [Lab: Write your first Flutter app](https://docs.flutter.dev/get-started/codelab)
- [Cookbook: Useful Flutter samples](https://docs.flutter.dev/cookbook)

Refer to the [Flutter online documentation](https://docs.flutter.dev) for more
guidance on app development and APIs.
