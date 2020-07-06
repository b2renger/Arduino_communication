#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
 
 
char ssid[] = "(-.-)";                // your network SSID (name)
char pass[] = "armand85";            // your network password
 
WiFiUDP Udp;                                
const IPAddress outIp(192,168,43,198);        // remote IP of your computer
const unsigned int outPort = 10000;          // remote port to receive OSC
const unsigned int localPort = 12000;        // local port to listen OSC
 
void setup() {
    Serial.begin(115200);
 
    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
 
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
 
    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(Udp.localPort());
}
 
void loop() {
   //the message wants an OSC address as first argument
  OSCMessage msg("/msg");
  msg.add(1234);
  
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message
  delay(20)
}
