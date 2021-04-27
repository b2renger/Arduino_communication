/*
  LED
  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED.
  The circuit:
  - Arduino MKR WiFi 1010 or Arduino Uno WiFi Rev2 board or Aduino nano iot 33
  - neopixel ring on D10
  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service
// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic redCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1211", BLERead | BLEWrite);
BLEIntCharacteristic greenCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1212", BLERead | BLEWrite);
BLEIntCharacteristic blueCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1213", BLERead | BLEWrite);



#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

int NumPixels = 12;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NumPixels, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  ledService.addCharacteristic(redCharacteristic);
  ledService.addCharacteristic(greenCharacteristic);
  ledService.addCharacteristic(blueCharacteristic);


  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);
  redCharacteristic.writeValue(100);
  greenCharacteristic.writeValue(0);
  blueCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");

  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          for (int i = 0; i < NumPixels; i++) {
            // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
            pixels.setPixelColor(i, pixels.Color(redCharacteristic.value(), greenCharacteristic.value(), blueCharacteristic.value())); // Moderately bright green color.
          }
          pixels.show(); // This sends the updated pixel color to the hardware.
        } else {                              // a 0 value
          for (int i = 0; i < NumPixels; i++) {
            // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
            pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.
          }
          pixels.show(); // This sends the updated pixel color to the hardware.
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
