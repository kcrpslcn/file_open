import 'package:flutter/material.dart';

class FileInfoHeader extends StatelessWidget {
  final int fileCount;

  const FileInfoHeader({super.key, required this.fileCount});

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(16),
      color: Theme.of(context).colorScheme.surfaceContainerHighest,
      child: Row(
        children: [
          const Icon(Icons.info_outline),
          const SizedBox(width: 8),
          Expanded(
            child: Text(
              '$fileCount file(s) opened',
              style: Theme.of(context).textTheme.titleMedium,
            ),
          ),
        ],
      ),
    );
  }
}
