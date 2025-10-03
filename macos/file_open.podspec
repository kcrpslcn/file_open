Pod::Spec.new do |s|
  s.name             = 'file_open'
  s.version          = '0.1.0'
  s.summary          = 'Handle file open events on macOS.'
  s.description      = <<-DESC
Receive files/URLs that launched the app and subsequent open events, and forward them to Flutter.
  DESC
  s.homepage         = 'https://github.com/niclasEX/flutter_file_type'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'file_open' => 'dev@example.com' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'
  s.platform = :osx, '10.15'
  s.swift_version = '5.0'
end
