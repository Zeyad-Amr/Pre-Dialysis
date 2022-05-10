import 'package:flutter/foundation.dart' show kIsWeb;
import 'package:control_app/widgets/strip_widget.dart';
import 'package:flutter/material.dart';

class Home extends StatefulWidget {
  const Home({Key? key}) : super(key: key);

  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {
  @override
  Widget build(BuildContext context) {
    if (kIsWeb) {
      debugPrint("Hello Web");
    }

    return Scaffold(
      appBar: AppBar(
          title: const Text("Smart Hemodialysis"),
          backgroundColor: Colors.grey[900]),
      body: Column(
        children: [
          Container(
            child: const Center(
                child: Text('Dialysis Machine Now Available',
                    style: TextStyle(color: Colors.white))),
            color: Colors.green[600],
            width: double.infinity,
            height: 20,
          ),
          Expanded(
              child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: const [
              Strip(
                icon: Icon(Icons.thermostat),
                txt: 'Temperature',
              ),
            ],
          )),
        ],
      ),
    );
  }
}
