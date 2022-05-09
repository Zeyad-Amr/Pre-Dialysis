import 'package:flutter/material.dart';

class Home extends StatefulWidget {
  const Home({Key? key}) : super(key: key);

  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {
  @override
  Widget build(BuildContext context) {
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
            children: [
              Image.asset(
                'assets/akwa.png',
                height: MediaQuery.of(context).size.height / 2,
              ),
              const SizedBox(height: 20),
              const Center(child: Text("AKWA MIX EVERY WHERE")),
            ],
          )),
        ],
      ),
    );
  }
}
