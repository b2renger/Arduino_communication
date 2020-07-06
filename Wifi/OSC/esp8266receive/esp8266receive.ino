#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#include <Servo.h>

char ssid[] = "HUAWEI-B315-6EEC";          // your network SSID (name)
char pass[] = "JE38QBQE74T";                    // your network password

WiFiUDP Udp;
const IPAddress outIp(192,168,43,146);        // remote IP (not needed for receive)
const unsigned int outPort = 12000;          // remote port (not needed for receive)
const unsigned int localPort = 10000;        // local port to listen for UDP packets (here's where we send the packets)

int servoPos = 90;

Servo myServo; 
//
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
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif

   myServo.attach(2);
}

void message(OSCMessage &msg) {
  int State = msg.getInt(0);
  servoPos = map(State, 0, 1023, 0, 180);
  Serial.print("message: ");
  Serial.println(State);
}

void loop() {
  
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
      bundle.dispatch("/msg", message);
  }

  myServo.write(servoPos);
}



