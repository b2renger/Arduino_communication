/*

  The circuit:
  - Arduino MKR WiFi 1010 or Arduino Uno WiFi Rev2 board or arduino nano 33 iot
  - Button connected to pin D4
  - Potentionmeter on pin A0

*/

#include <ArduinoBLE.h>

const int buttonPin = 4; // set buttonPin to digital pin 4

// create service
BLEService myService("19B10010-E8F2-537E-4F6C-D104768A1214");

// create characteristics (it will store our values) and allow remote device to get notifications
BLEIntCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1211", BLERead | BLENotify);
BLEIntCharacteristic potCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1212", BLERead | BLENotify);


void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT); // use button pin as an input

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("myBleDevice");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(myService);

  // add the characteristics to the service;
  myService.addCharacteristic(buttonCharacteristic);
  myService.addCharacteristic(potCharacteristic);

  // add the service
  BLE.addService(myService);

  // write initial values to characteristics
  buttonCharacteristic.writeValue(0);
  potCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();
  buttonCharacteristic.writeValue(digitalRead(4));
  potCharacteristic.writeValue(map(analogRead(0), 0, 1023, 0, 255));
   Serial.println(analogRead(0)/4.);
}
