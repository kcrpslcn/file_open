import 'package:flutter/material.dart';

import 'utils.dart';

class FileListDialog extends StatelessWidget {
  final List<Uri> uris;
  final bool isInitial;

  const FileListDialog({super.key, required this.uris, this.isInitial = false});

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: Text(isInitial ? 'Initial Files' : 'Recently Opened Files'),
      content: SizedBox(
        width: double.maxFinite,
        child: ListView.builder(
          shrinkWrap: true,
          itemCount: uris.length,
          itemBuilder: (context, index) => ListTile(
            leading: const Icon(Icons.insert_drive_file_outlined),
            title: Text(getFileName(uris[index])),
            subtitle: Text(uris[index].toString(),
                style: Theme.of(context).textTheme.bodySmall),
          ),
        ),
      ),
      actions: [
        TextButton(
          onPressed: () => Navigator.of(context).pop(),
          child: const Text('Close'),
        ),
      ],
    );
  }
}