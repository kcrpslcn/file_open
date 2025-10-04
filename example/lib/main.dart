import 'package:file_open/file_open.dart';
import 'package:flutter/material.dart';

import 'file_info_header.dart';
import 'file_list.dart';
import 'snackbar_helper.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: ExamplePage(),
    );
  }
}

class ExamplePage extends StatefulWidget {
  const ExamplePage({super.key});

  @override
  State<ExamplePage> createState() => _ExamplePageState();
}

class _ExamplePageState extends State<ExamplePage> {
  final List<Uri> _opened = [];
  bool _loggingEnabled = true;

  @override
  void initState() {
    super.initState();
    FileOpen.onOpened.listen((uris) {
      setState(() => _opened.insertAll(0, uris));
      if (!mounted) return;
      showFileOpenedSnackBar(context, uris);
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('file_open example'),
        actions: [
          Row(
            children: [
              const Text('Logging'),
              Switch(
                value: _loggingEnabled,
                onChanged: (value) {
                  setState(() => _loggingEnabled = value);
                  FileOpen.setLoggingEnabled(value);
                },
              ),
            ],
          ),
        ],
      ),
      body: Column(
        children: [
          FileInfoHeader(fileCount: _opened.length),
          Expanded(
            child: FileList(opened: _opened),
          ),
        ],
      ),
    );
  }
}
