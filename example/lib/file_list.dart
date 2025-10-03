import 'package:flutter/material.dart';

import 'empty_state.dart';
import 'file_list_item.dart';

class FileList extends StatelessWidget {
  final List<Uri> opened;

  const FileList({super.key, required this.opened});

  @override
  Widget build(BuildContext context) {
    if (opened.isEmpty) {
      return const EmptyState();
    }
    return ListView.separated(
      itemCount: opened.length,
      separatorBuilder: (_, __) => const Divider(height: 1),
      itemBuilder: (_, i) {
        final uri = opened[i];
        return FileListItem(uri: uri);
      },
    );
  }
}