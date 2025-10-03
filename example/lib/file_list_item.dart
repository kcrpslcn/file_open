import 'package:flutter/material.dart';

import 'utils.dart';

class FileListItem extends StatelessWidget {
  final Uri uri;

  const FileListItem({super.key, required this.uri});

  @override
  Widget build(BuildContext context) {
    final fileName = getFileName(uri);
    final isFile = uri.scheme == 'file';

    return ListTile(
      leading: Icon(
        isFile ? Icons.insert_drive_file_outlined : Icons.link,
        color: isFile ? Colors.blue : Colors.orange,
      ),
      title: Text(fileName),
      subtitle: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(
            uri.toString(),
            style: Theme.of(context).textTheme.bodySmall,
          ),
          const SizedBox(height: 4),
          Row(
            children: [
              Container(
                padding: const EdgeInsets.symmetric(horizontal: 6, vertical: 2),
                decoration: BoxDecoration(
                  color: isFile
                      ? Colors.blue.withValues(alpha: 0.1)
                      : Colors.orange.withValues(alpha: 0.1),
                  borderRadius: BorderRadius.circular(4),
                ),
                child: Text(
                  uri.scheme.toUpperCase(),
                  style: TextStyle(
                    fontSize: 10,
                    fontWeight: FontWeight.bold,
                    color: isFile ? Colors.blue : Colors.orange,
                  ),
                ),
              ),
              if (isFile && uri.path.contains('.')) ...[
                const SizedBox(width: 8),
                Container(
                  padding:
                      const EdgeInsets.symmetric(horizontal: 6, vertical: 2),
                  decoration: BoxDecoration(
                    color: Colors.green.withValues(alpha: 0.1),
                    borderRadius: BorderRadius.circular(4),
                  ),
                  child: Text(
                    uri.path.split('.').last.toUpperCase(),
                    style: const TextStyle(
                      fontSize: 10,
                      fontWeight: FontWeight.bold,
                      color: Colors.green,
                    ),
                  ),
                ),
              ],
            ],
          ),
        ],
      ),
      isThreeLine: true,
      onTap: () {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('Tapped: $fileName'),
            duration: const Duration(seconds: 1),
          ),
        );
      },
    );
  }
}
