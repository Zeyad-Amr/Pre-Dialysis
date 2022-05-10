import 'package:flutter/material.dart';

class Strip extends StatelessWidget {
  const Strip({
    Key? key,
    required this.icon,
    required this.txt,
  }) : super(key: key);
  final Icon? icon;
  final String? txt;

  @override
  Widget build(BuildContext context) {
    double widths = MediaQuery.of(context).size.width;
    double heights = MediaQuery.of(context).size.height;

    return Stack(
      children: [
        Positioned(
            child: Container(
          width: widths * 0.7,
          height: heights * 0.05,
          color: Colors.grey[900],
        ))
      ],
    );
  }
}
