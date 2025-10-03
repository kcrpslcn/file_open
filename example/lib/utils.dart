String getFileName(Uri uri) {
  final path = uri.path;
  final lastSlash = path.lastIndexOf('/');
  return lastSlash >= 0 ? path.substring(lastSlash + 1) : path;
}
