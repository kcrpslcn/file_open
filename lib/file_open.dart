import 'dart:async';

import 'src/messages.g.dart';

/// Provides a simple API to receive file/URL open events on desktop.
///
/// - [onOpened] streams subsequent open events while the app is running.
/// - [setLoggingEnabled] enables or disables logging on the native platform.
/// - Logging is enabled by default.
class FileOpen implements FileOpenFlutterApi {
  FileOpen._() {
    FileOpenFlutterApi.setUp(this);
  }
  static FileOpen? _lazyInstance;
  static FileOpen get _instance {
    _lazyInstance ??= FileOpen._();
    return _lazyInstance!;
  }

  /// The host API for controlling native platform behavior.
  static final _hostApi = FileOpenHostApi();

  final _onOpenedController = StreamController<List<Uri>>.broadcast();

  /// Stream of subsequent open requests. Each event is a batch (one or more).
  static Stream<List<Uri>> get onOpened {
    return _instance._onOpenedController.stream;
  }

  /// Enable or disable logging on the native platform.
  /// Logging is enabled by default.
  static Future<void> setLoggingEnabled(bool enabled) {
    return _hostApi.setLoggingEnabled(enabled);
  }

  /// Internal callback from native platform.
  @override
  void onFileOpened(List<String> filePaths) {
    final uris = _toUris(filePaths);
    _onOpenedController.add(uris);
  }
}

List<Uri> _toUris(List<String> list) {
  final uris = <Uri>[];
  for (final v in list) {
    // Heuristic: treat file paths lacking a scheme as file URIs.
    if (v.contains('://')) {
      uris.add(Uri.parse(v));
    } else {
      uris.add(Uri.file(v));
    }
  }
  return List.unmodifiable(uris);
}
