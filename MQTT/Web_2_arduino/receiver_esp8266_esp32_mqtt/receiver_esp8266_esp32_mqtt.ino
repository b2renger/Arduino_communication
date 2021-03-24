
/*
   based on : https://www.shiftr.io/docs/manuals/arduino
   install arduino-mqtt (by Joël Gähwiler) : https://github.com/256dpi/arduino-mqtt
   install WiFiManager from https://github.com/tzapu/WiFiManager
   install neopixel lib from adafruit
*/

// wifi stuff
// uncomment the 3 lines below if you are working with an esp8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <WiFiManager.h>
#include <Ticker.h>


// led stuff
#include <Adafruit_NeoPixel.h>
// neopixel variables
#define PIN        14
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// MQTT stuff
#include <MQTT.h>
WiFiClient net;
MQTTClient client;

// wifi connection feedback
Ticker ticker;
int LED = LED_BUILTIN;
#define LED_BUILTIN 13 // ESP32 DOES NOT DEFINE LED_BUILTIN

void tick() {
  digitalWrite(LED, !digitalRead(LED));
}



void setup() {

  Serial.begin(115200);

  // init the lights at startup
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(10, 10, 10));
  }
  pixels.show();


  // wifi stuff
  pinMode(LED, OUTPUT);
  ticker.attach(0.6, tick);// start ticker with 0.5 because we start in AP mode and try to connect

  init_wifi();

  ticker.detach();
  digitalWrite(LED, LOW);
  client.begin("public.cloud.shiftr.io", net);
  Serial.println("connecting to broker");
  while (!client.connect("ESP8266_EDNA", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  //delay(1000);
  Serial.println(client.connected());

  client.subscribe("col");
  client.onMessage(messageReceived);
  
}

void loop() {

  client.loop();
  delay(10);

  // check if connected
  if (!client.connected()) {
    // connect();
  }
}

void messageReceived(String &topic, String &payload) {
  Serial.println(topic + ": " + payload);
}

void init_wifi() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  //  WiFi.hostname(espname);
  //  wifi_station_set_hostname(espname);
  //WiFiManager
  WiFiManager wm;
  // wm.resetSettings();//reset settings - for testing
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wm.setAPCallback(configModeCallback);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" and goes into a blocking loop awaiting configuration
  if (!wm.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeah ! :)");


}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}
