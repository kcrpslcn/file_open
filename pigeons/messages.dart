import 'package:pigeon/pigeon.dart';

@ConfigurePigeon(
  PigeonOptions(
    dartOut: 'lib/src/messages.g.dart',
    dartOptions: DartOptions(),
    swiftOut: 'macos/Classes/Messages.g.swift',
    swiftOptions: SwiftOptions(),
    cppHeaderOut: 'macos/Classes/Messages.g.h',
    cppSourceOut: 'macos/Classes/Messages.g.cpp',
  ),
)
@FlutterApi()
abstract class FileOpenFlutterApi {
  void onFileOpened(List<String> filePaths);
}

@HostApi()
abstract class FileOpenHostApi {
  void setLoggingEnabled(bool enabled);
}
