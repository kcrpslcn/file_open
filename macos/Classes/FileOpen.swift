import Cocoa

/// Processes opened files and notifies the Flutter side via the API.
class FileOpen {
    private let flutterApi: FileOpenFlutterApi

    private static func log(_ message: String) {
        if FileOpenPlugin.loggingEnabled {
            NSLog("[FileOpen] \(message)")
        }
    }

    init(flutterApi: FileOpenFlutterApi) {
        self.flutterApi = flutterApi
    }

    /// Processes the list of opened file URLs, normalizes paths, and sends them to Flutter.
    func handleOpenedFiles(_ urls: [URL]) {
        let paths = urls.map { $0.path }
        Self.log("application:open urls count=\(urls.count)")
        let normalizedPaths = paths.map { ($0 as NSString).expandingTildeInPath }
        Self.log("Forwarding files to Flutter: \(normalizedPaths)")
        flutterApi.onFileOpened(filePaths: normalizedPaths) { _ in }
    }
}