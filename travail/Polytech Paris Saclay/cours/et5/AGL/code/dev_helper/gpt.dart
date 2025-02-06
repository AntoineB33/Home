import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key}); // Add the key parameter

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      debugShowCheckedModeBanner: false,
      home: HomePage(),
    );
  }
}

class HomePage extends StatelessWidget {
  const HomePage({super.key}); // Add the key parameter

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        // decoration: const BoxDecoration(
        //   image: DecorationImage(
        //     image: AssetImage('assets/images/background_image_home.jpg'), // Replace with your image asset
        //     fit: BoxFit.cover,
        //   ),
        // ),
        decoration: const BoxDecoration(
          color: Colors.red, // Set the background color to red
        ),
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              const Text(
                "???",
                style: TextStyle(
                  fontSize: 36,
                  fontWeight: FontWeight.bold,
                  color: Colors.white,
                ),
              ),
              const SizedBox(height: 10),
              const Text(
                "Votre compagnon d'aide au tri",
                textAlign: TextAlign.center,
                style: TextStyle(
                  fontSize: 18,
                  color: Colors.white,
                ),
              ),
              const SizedBox(height: 30),
              ElevatedButton(
                onPressed: () {
                  // Handle "Se connecter"
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: Colors.grey.shade300, // Updated
                  foregroundColor: Colors.black, // Updated
                  padding: const EdgeInsets.symmetric(horizontal: 50, vertical: 15),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(30),
                  ),
                ),
                child: const Text("Se connecter"),
              ),
              const SizedBox(height: 15),
              ElevatedButton(
                onPressed: () {
                  // Handle "Utiliser en tant qu’invité"
                },
                style: ElevatedButton.styleFrom(
                  backgroundColor: Colors.green, // Updated
                  foregroundColor: Colors.white, // Updated
                  padding: const EdgeInsets.symmetric(horizontal: 50, vertical: 15),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(30),
                  ),
                ),
                child: const Text("Utiliser en tant qu’invité"),
              ),
              const SizedBox(height: 15),
              TextButton(
                onPressed: () {
                  // Handle "S’inscrire"
                },
                child: const Text(
                  "S’inscrire",
                  style: TextStyle(
                    color: Colors.white,
                    decoration: TextDecoration.underline,
                  ),
                ),
              ),
              const Spacer(),
              const Padding(
                padding: EdgeInsets.only(bottom: 20.0),
                child: Text(
                  "Une initiative de",
                  style: TextStyle(
                    color: Colors.white,
                  ),
                ),
              ),
              Image.asset(
                'assets/images/viveris_logo_blanc.svg', // Replace with your logo asset
                height: 30,
              ),
            ],
          ),
        ),
      ),
    );
  }
}
