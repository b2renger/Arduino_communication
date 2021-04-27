#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>


char pass[] = "JE38QBQE74T";                // your network SSID (name)
char ssid[] = "HUAWEI-B315-6EEC";            // your network password

WiFiUDP Udp;
const IPAddress outIp(192, 168, 8, 101);     // remote IP of your computer
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

  int pot = analogRead(0);
  Serial.println(pot);
  //the message wants an OSC address as first argument
  OSCMessage msg("/msg");
  msg.add(pot);

  Udp.beginPacket(outIp, outPort);
  msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message
  delay(20);
}
