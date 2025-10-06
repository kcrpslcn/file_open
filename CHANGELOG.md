## 0.2.0

- Add Windows support: receive file/URL open events on Windows (single primary instance; subsequent launches forward their opened file paths to the running instance).
- Same `FileOpen.onOpened` batching semantics and `FileOpen.setLoggingEnabled` host control now available on Windows.

## 0.1.0

- Receive file open events on macOS
- Provides `FileOpen.onOpened` stream for handling batches of opened file/URL URIs
- Includes `FileOpen.setLoggingEnabled(bool)` API to enable/disable native platform logging
