# Arduino_communication

How to make arduino cards communicate with other programms or with each other 

**Work in progress**

---
## Working examples and tutorials 

### Serial

- [How to control video playback in processing with arduino and distance sensor](Serial_Arduino2Processing_distance.md) 
- [How to control an animation with arduino and a photoresistor](Serial_Arduino2Processing_photoresistance)
- [How to to save arduino sensor data on a file on your computer](Serial_Arduino2Processing_saveData2json.md)
- [How to control neopixels leds with your mouse](Serial_Processing2Arduino_mouse2leds.md)

---

## Goals
How to make arduino cards communicate with other programms or with each other using :
- serial 
- wifi (udp/osc , http, mqtt)
- bluetooth ( 1sheeld / bluetooth serial / ble )
- IR 
- Radio 
- ifttt 

Boads targetted : 
- Arduino Uno wifi rev2 / Arduino Nano iot 33 (ble)
- Feather Huzzah 8266 (wifi)
- Feather Esp32 (wifi + ble)
- Sparkfun ESP32 Thing (wifi + ble)

Programs targetted :
- Processing
- p5js 
- cablesgl
- Touch Designer
- Unity 
- Unreal 

HID behaviors via ble (specially handy to get data in any programm)
- keyboard
- mouse
- gamepad

---
## Todo list

[/] == working but needs documentation

[] == contribute if you want or ask for it

- [/] BLE arduino IOT (nano33 etc) and p5
- [/] MQTT (esp32 or 8266) and p5
- [/] MQTT (esp32 or 8266) and cablesgl
- [/] WIFI OSC (esp8266 or esp32) and processing

- [/] BLEKeyboard with ESP32
- [/] Serial from Arduino Serial Monitor
- [/] Serial and processing => add simple example without json
- [/] BLE esp32 (feather) and p5
- [/] BLE HM10 module and p5
- [/] WIFI OSC (esp8266) board to board  => support esp32
- [/] WIFI OSC (esp8266) and Touch Designer => support esp32
- [/] Serial and TouchDesigner => communication both ways
- [/] Serial and Unity => communication both ways
- [/] Wifi OSC Unity => communication both ways

- [] Bluetooth serial (like HC-06) 
- [] IR
- [] ifttt
- [] Radio
---



---
## Sources and references

### Libraries

#### Arduino

- https://github.com/tzapu/WiFiManager
  This library will open the possibility to run on esp32 and esp8266 like chips. There is no need for hardcoded wifi ssid or password. If a connection to a specific network is not made, the library will create a captiv portal for you to connect to so that you can enter the credentials of the desired network.

- https://github.com/CNMAT/OSC
  On the arduino side we will depend on this library for easy construction of udp messages

- https://github.com/256dpi/arduino-mqtt

- https://github.com/T-vK/ESP32-BLE-Keyboard

- https://github.com/T-vK/ESP32-BLE-Mouse

- https://github.com/lemmingDev/ESP32-BLE-Gamepad


#### Javascript

- https://github.com/mqttjs/MQTT.js


#### Processing
- https://sojamo.de/libraries/oscP5/
  Available through the library manager

- https://github.com/256dpi/processing-mqtt


#### Cables.gl operatos
- https://cables.gl/op/Ops.Net.Mqtt.Mqtt_v2s
- https://cables.gl/docs/3_communication/mqtt_arduino/mqtt_arduino


### Projects and apps
- https://github.com/b2renger/processing_osc_controller
  This project will get you started with easy interface prototyping with processing (desktop and android). It supports auto-discovery of the devices on the same network listenning on a specific port, and for android it supports sending the data of a few snesors.

- 1Sheeld : https://web.archive.org/web/20210124101731/https://1sheeld.com/

- Bluetooth Control Panel : http://www.martyncurrey.com/bluetooth-control-panel/

- Bluetooth Electronics : https://play.google.com/store/apps/details?id=com.keuwl.arduinobluetooth&hl=fr&gl=US

- UUID generator for ble : https://www.uuidgenerator.net/


### MQTT brokers
- https://www.shiftr.io/cloud#pricing
- https://io.adafruit.com/
- https://www.maqiatto.com/


### Articles
- Bluetooth serial on esp32 : https://circuitdigest.com/microcontroller-projects/using-classic-bluetooth-in-esp32-and-toogle-an-led

- Bluetooth low energy (ble) and p5js : https://create.arduino.cc/projecthub/jingwen_zhu/getting-started-with-p5-ble-js-using-arduino-nano-33-ble-120ea6


### Radio
- https://www.gotronic.fr/art-module-rf-434-mhz-wrl-12823-25308.htm
- https://www.carnetdumaker.net/articles/communiquer-sans-fil-en-433mhz-avec-la-bibliotheque-virtualwire-et-une-carte-arduino-genuino/
- http://robotlabo.eu/pageTuto/tutoSansFil.html

