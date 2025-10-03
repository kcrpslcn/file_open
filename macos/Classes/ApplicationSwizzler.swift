import Cocoa
import ObjectiveC.runtime

/// Handles method swizzling to intercept file open events from the app delegate.
class ApplicationSwizzler {
    static var flutterApi: FileOpenFlutterApi?
    private static var didSwizzle = false

    private static func log(_ message: String) {
        if FileOpenPlugin.loggingEnabled {
            NSLog("[ApplicationSwizzler] \(message)")
        }
    }

    init(flutterApi: FileOpenFlutterApi) {
        ApplicationSwizzler.flutterApi = flutterApi
        swizzleApplicationOpen()
    }

    private func swizzleApplicationOpen() {
        guard let appDelegate = NSApplication.shared.delegate,
              let appDelegateClass = object_getClass(appDelegate)
        else {
            Self.log("Failed to get app delegate.")
            return
        }

        // Avoid double-swizzling
        if ApplicationSwizzler.didSwizzle { return }
        ApplicationSwizzler.didSwizzle = true

        let originalSelector = #selector(NSApplicationDelegate.application(_:open:))

        if let originalMethod = class_getInstanceMethod(appDelegateClass, originalSelector) {
            let originalIMP = method_getImplementation(originalMethod)
            typealias OriginalOpenIMP = @convention(c) (AnyObject, Selector, NSApplication, [URL]) -> Void

            let block: @convention(block) (AnyObject, NSApplication, [URL]) -> Void = { receiver, application, urls in
                let paths = urls.map { $0.path }
                Self.log("application:open urls count=\(urls.count)")
                ApplicationSwizzler.handleOpenedFiles(paths)

                // Call the original implementation at the end
                let fn = unsafeBitCast(originalIMP, to: OriginalOpenIMP.self)
                Self.log("Calling original implementation.")
                fn(receiver, originalSelector, application, urls)
            }

            let newImp = imp_implementationWithBlock(block as Any)
            method_setImplementation(originalMethod, newImp)
        } else {
            Self.log("AppDelegate does not implement application:open:. Adding our own implementation.")
            // AppDelegate did not implement this method; add ours.
            let block: @convention(block) (AnyObject, NSApplication, [URL]) -> Void = { _, _, urls in
                let paths = urls.map { $0.path }
                Self.log("application:open urls count=\(urls.count)")
                ApplicationSwizzler.handleOpenedFiles(paths)
            }
            let newImp = imp_implementationWithBlock(block as Any)
            class_addMethod(appDelegateClass, originalSelector, newImp, "v@:@@")
        }
    }
}

extension ApplicationSwizzler {
    static func handleOpenedFiles(_ files: [String]) {
        // Normalize to absolute paths
        let paths = files.map { ($0 as NSString).expandingTildeInPath }
        if let api = flutterApi {
            Self.log("Forwarding files to Flutter: \(paths)")
            api.onFileOpened(filePaths: paths) { _ in }
        } else {
            Self.log("Buffering files (Flutter not ready yet): \(paths)")
        }
    }
}
