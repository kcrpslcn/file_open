import 'package:flutter/material.dart';

import 'file_list_dialog.dart';
import 'utils.dart';

void showFileOpenedSnackBar(BuildContext context, List<Uri> uris,
    {bool isInitial = false}) {
  final count = uris.length;
  final prefix = isInitial ? 'Initial file(s)' : 'File(s) opened';
  final message = count == 1
      ? '$prefix: ${getFileName(uris.first)}'
      : '$prefix: $count files';

  ScaffoldMessenger.of(context).showSnackBar(
    SnackBar(
      content: Text(message),
      duration: const Duration(seconds: 3),
      action: count > 1
          ? SnackBarAction(
              label: 'View All',
              onPressed: () {
                showDialog(
                  context: context,
                  builder: (context) => FileListDialog(
                    uris: uris,
                    isInitial: isInitial,
                  ),
                );
              },
            )
          : null,
    ),
  );
}
