import Cocoa
import FlutterMacOS

public class FileOpenPlugin: NSObject, FlutterPlugin, FileOpenHostApi {
    private static var shared: FileOpenPlugin?

    private var methodChannel: FlutterMethodChannel?
    private var flutterApi: FileOpenFlutterApi?
    private var applicationSwizzler: ApplicationSwizzler?

    static var loggingEnabled = true

    // This is the standard entry point for the plugin.
    // It's called once when the app launches.
    public static func register(with registrar: FlutterPluginRegistrar) {
        let instance = FileOpenPlugin()
        shared = instance

        let messenger = registrar.messenger
        instance.flutterApi = FileOpenFlutterApi(binaryMessenger: messenger)
        instance.applicationSwizzler = ApplicationSwizzler(flutterApi: instance.flutterApi!)

        FileOpenHostApiSetup.setUp(binaryMessenger: messenger, api: instance)
    }

    func setLoggingEnabled(enabled: Bool) throws {
        Self.loggingEnabled = enabled
        if Self.loggingEnabled {
            NSLog("[FileOpenPlugin] Logging enabled")
        }
    }
}
