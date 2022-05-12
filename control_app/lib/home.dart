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
    double widths = 0;
    double heights = 0;
    if (MediaQuery.of(context).orientation == Orientation.portrait) {
      widths = MediaQuery.of(context).size.width;
      heights = MediaQuery.of(context).size.height;
    } else if (kIsWeb ||
        MediaQuery.of(context).orientation == Orientation.landscape) {
      widths = MediaQuery.of(context).size.height;
      heights = MediaQuery.of(context).size.width;
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
              child: Padding(
            padding: const EdgeInsets.symmetric(vertical: 10),
            child: ListView(
              children: const [
                Padding(
                  padding: EdgeInsets.all(8.0),
                  child:
                      Strip(icon: Icon(Icons.thermostat), txt: 'Temperature'),
                ),
                Padding(
                  padding: EdgeInsets.all(8.0),
                  child:
                      Strip(icon: Icon(Icons.thermostat), txt: 'Temperature'),
                ),
                Padding(
                  padding: EdgeInsets.all(8.0),
                  child:
                      Strip(icon: Icon(Icons.thermostat), txt: 'Temperature'),
                ),
                Padding(
                  padding: EdgeInsets.all(8.0),
                  child:
                      Strip(icon: Icon(Icons.thermostat), txt: 'Temperature'),
                ),
              ],
            ),
          )),
        ],
      ),
    );
  }
}
