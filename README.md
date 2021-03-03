# Arduino_communication

**Work in progress - thinking it through for now**


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
- Unreal ?

Easy access ressources through : 
- 1Sheeld
- Blynk 

## Contents
* [Serial communication](#Serial-communication)
    * [Arduino and processing](#Arduino-and-processing) <br>
      * [Control video playback with a distance sensor](#Control-video-playback-with-a-distance-sensor) <br>
      * [Control an animation with a photoresistor](#Control-an-animation-with-a-photoresistor) <br>
      * [Control neopixels leds with the mouse](#Control-neopixels-leds-with-the-mouse) <br>
      * [Save data to a file on your computer](#Save-data-to-a-file) <br>


---
## Serial communication
This means via a USB cable ! (Universal Serial Bus)

### Arduino and processing

In this part we will focus on the use of the serial port and we are going : from an arduino program write sensor values ​​in a serial port that we can then recover in a [processing](https://processing.org/) program.

We can do the same in the other direction: that is, send processing information to arduino.

FIY USB means Universal Serial Bus, so the serial port is the USB port so we program a way to pass from one device to another information via a USB connection.

[**home**](#Contents)

#### Control video playback with a distance sensor

The first program will use a distance sensor and the distance detected by our sensor will be used to adjust the running speed of a video read by a processing program: 

<img src="assets/serial_distance_to_movie_speed.gif" width="480" height="270" /><br>

As a reminder, the wiring of the distance sensor is as follows :

<img src="read_from_IRDistance/read_from_irdistance.png" width="480" height="270" /><br>

We will use the sample code provided in the processing video library. Available in the examples browser: *Exemples* -> *Libraries* -> *Video* -> *Movie* -> *Speed*

I advise you to open it from processing (as you already have the video and code), but the code looks like this with some comments.

```java
/**
 * Speed. 
 * Use the Movie.speed() method to change
 * the playback speed.
 */

import processing.video.*; // import the library video
Movie mov; // create a movie object called mov so you can load a movie and play it.

void setup() {
  size(640, 360);
  background(0);
  mov = new Movie(this, "transit.mov"); // load the movie "transit.mov" which is in the data folder of your sketch
  mov.loop(); // start looping the movie
}

void draw() {    
  image(mov, 0, 0); // display a frame of the movie
             
  // calculate a variable that will be used to control the movie's frame rate
  // this variable will depend on the mouse whose horizontal position is between 0 and 'width'
  // and we want to get values ​​between 0.1 and 2
  float newSpeed = map(mouseX, 0, width, 0.1, 2);
  mov.speed(newSpeed); // use our variable to change the speed of the movie
  
  // show the speed value on the top left
  fill(255);
  text(nfc(newSpeed, 2) + "X", 10, 30); 
}  

// function needed to trigger movie playback
void movieEvent(Movie movie) {
  mov.read();  
}

```
In this code we make the speed depend on the position of the mouse, so we will replace the variable * mouseX * on the line:

```java
float newSpeed = map(mouseX, 0, width, 0.1, 2);
```
by the value that we will capture via our distance sensor and arduino card. 

For this in our arduino code we will write a character string in the format [**JSON **](https://en.wikipedia.org/wiki/JSON) in which we will insert a value read from our analog input.

Our character string will have to look like this
```json
{
    "distance" : distance_value_measured_by_our_sensor
}
```

This arduino code makes it possible to do this with string concatenation.
```c
String json;
  json = "{\"distance\":";
  json = json + analogRead(0);
  json = json + "}";
```

We just have to post this character string on the serial port with **Serial.println ()** as usual. Here is the whole arduino code:

```c

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  int value = analogRead(00);

  String json;
  json = "{\"distance\":";
  json = json + value;
  json = json + "}";

  Serial.println(json);
}
```
On the processing side, it is now necessary to start receiving this string of characters, to extract the distance data and to use it.

We will have to use the library [**Serial**](https://processing.org/reference/libraries/serial/index.html), it is not necessary to install it because it is integrated by default in processing.

We have to start by importing the library by typing at the very top of the sketch we use to read our film:
```java
import processing.serial.*;
Serial myPort;  //Create a serial object to read the information posted on the serial port
```
we will use it to create a variable intended to store the information received by arduino

```java
int valueFromArduino = 50;
int movieSpeed = 0;
```
Then we have to add some lines to the **setup()**: it is necessary to start the program initialize the serial connection with our arduino card.

```java
// initialization of communication via usb from arduino
// BE CAREFUL to use the adapted port
printArray(Serial.list()); //print the list of devices connected to the serial port
String portName = Serial.list()[3]; // my arduino card is the third in the list printed in the console
myPort = new Serial(this, portName, 9600); //we open the communication
myPort.bufferUntil('\n');
```

Now, we have to execute code every time information is posted on the serial port, this is done by writing it in a function:
```java
void serialEvent (Serial myPort) {
  
}
```

The code below is a little hard to get, but you do not have to understand everything. Basically we will try to read the data arriving on the serial port, if it is open, then we will decode our json string:

```java
void serialEvent (Serial myPort) {
  try { // we try to do something but we do not crash if we can not do it
    while (myPort.available() > 0) {
      String inBuffer = myPort.readStringUntil('\n'); // read the string from the serial port until the carriage return
      if (inBuffer != null) { // if it's not null
        if (inBuffer.substring(0, 1).equals("{")) { // and if it looks like json
          JSONObject json = parseJSONObject(inBuffer); // we try to read it like json
          
          // It is from here that one must understand !!
          if (json == null) { // if it's not json we do nothing
            
          }
          else { // else we get the value with the name 'distance' and we store it in a variable!
            if (abs(json.getInt("distance")-valueFromArduino)> 50) { // check that the value has changed enough
              valueFromArduino    = json.getInt("distance");
              // we map the value and store it in our movie speed variable created at the beginning
              movieSpeed = map(valueFromArduino, 50, 650, 2, 0);
              movieSpeed = constrain(movieSpeed, 0, 2); // keep this value in a consistent range of times that our sensor returns too large or too small values
            }
          }
        // we just have to close all our braces ^^
        } 
        else {
        }
      }
    }
  } 
  catch (Exception e) {
  }
}
```
There are a lot of security tests to prevent our program from crashing if our string is null or if the data we are looking for is not available.

But we only have to use *movieSpeed ​* to actually control the speed of the movie.

```java
mov.speed(movieSpeed);
```
Here is the processing code to retrieve the distance value from arduino and use it to control the speed of reading a movie:

```java

import processing.video.*;
Movie mov;


import processing.serial.*;
Serial myPort;  

int valueFromArduino = 50;
float movieSpeed = 2;

void setup() {
  size(800, 500);
  background(0);

  mov = new Movie(this, "transit.mov");
  mov.loop();

 // initialization of communication via usb from arduino
  // BE CAREFUL to use the adapted port
  printArray(Serial.list());
  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void movieEvent(Movie movie) {
  mov.read();
}

void draw() {    
  image(mov, 0, 0,width,height);

  
  mov.speed(movieSpeed);

  fill(255);
  text(nfc(movieSpeed, 2) + "X", 10, 30);
}  

void serialEvent (Serial myPort) {
  try {
    while (myPort.available() > 0) {
      String inBuffer = myPort.readStringUntil('\n');
      if (inBuffer != null) {
        if (inBuffer.substring(0, 1).equals("{")) {
          JSONObject json = parseJSONObject(inBuffer);
          if (json == null) {
            //println("JSONObject could not be parsed");
          } else {
            if (abs(json.getInt("distance")-valueFromArduino)> 50) {
              valueFromArduino    = json.getInt("distance");
              movieSpeed = map(valueFromArduino, 50, 650, 2, 0);
              movieSpeed = constrain(movieSpeed, 0, 2);
            }
          }
        } else {
        }
      }
    }
  } 
  catch (Exception e) {
  }
}

```

[**home**](#Contents)

#### Control an animation with a photoresistor

The logic of this animation is very similar to the previous one. We will replace the distance sensor with a photoresistor and we will use a different example:

<img src="assets/serial_photores_to_animation.gif" width="480" height="270" /><br>

The wiring is a photoresistor connected to A0.
<img src="read_from_photores/read_from_photores.png" width="480" height="270" /><br>

The arduino code is almost the same, we will just adapt the identifier in our json that will store our value:

```c
void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(A0);
  String json;
  json = "{\"photores\":";
  json = json + value;
  json = json + "}";

  Serial.println(json);
}
```

From a processing point of view we will again use an example available with the software: *Examples* -> *Topics* -> *Fractals and L-Systems* -> *Tree*

```java
/**
 * Recursive Tree
 * by Daniel Shiffman.  
 * 
 * Renders a simple tree-like structure via recursion. 
 * The branching angle is calculated as a function of 
 * the horizontal mouse location. Move the mouse left
 * and right to change the angle.
 */
 
float theta;   

void setup() {
  size(640, 360);
}

void draw() {
  background(0);
  frameRate(30);
  stroke(255);
  // Let's pick an angle 0 to 90 degrees based on the mouse position
  float a = (mouseX / (float) width) * 90f;
  // Convert it to radians
  theta = radians(a);
  // Start the tree from the bottom of the screen
  translate(width/2,height);
  // Draw a line 120 pixels
  line(0,0,0,-120);
  // Move to the end of that line
  translate(0,-120);
  // Start the recursive branching!
  branch(120);

}

void branch(float h) {
  // Each branch will be 2/3rds the size of the previous one
  h *= 0.66;
  
  // All recursive functions must have an exit condition!!!!
  // Here, ours is when the length of the branch is 2 pixels or less
  if (h > 2) {
    pushMatrix();    // Save the current state of transformation (i.e. where are we now)
    rotate(theta);   // Rotate by theta
    line(0, 0, 0, -h);  // Draw the branch
    translate(0, -h); // Move to the end of the branch
    branch(h);       // Ok, now call myself to draw two new branches!!
    popMatrix();     // Whenever we get back here, we "pop" in order to restore the previous matrix state
    
    // Repeat the same thing, only branch off to the "left" this time!
    pushMatrix();
    rotate(-theta);
    line(0, 0, 0, -h);
    translate(0, -h);
    branch(h);
    popMatrix();
  }
}
```

In a similar way to the previous one, we can identify the relevant line for the interaction:
```java
// Let's pick an angle 0 to 90 degrees based on the mouse position
  float a = (mouseX / (float) width) * 90f;
```

So we will delete this line and 'replace' *mouseX* by a value coming from the photoresistance ... First, let's adapt the *serialEvent()* function used previously to retrieve the value from our arduino program:

- Let's first define a global variable that will store our value:
```java
float valueFromArduino = 0;
```

- Then let's adapt the *serialEvent()* function to store in *valueFromArduino* the value coming from arduino by using the right key, the very one we defined in our arduino program (ie 'photores'):
```java
void serialEvent (Serial myPort) {
  try {
    while (myPort.available() > 0) {
      String inBuffer = myPort.readStringUntil('\n');
      if (inBuffer != null) {
        if (inBuffer.substring(0, 1).equals("{")) {
          JSONObject json = parseJSONObject(inBuffer);

          if (json == null) {
            //println("JSONObject could not be parsed");
          } else {
            valueFromArduino    = json.getInt("photores");
          }

        } else {
        }
      }
    }
  } 
  catch (Exception e) {
  }
}
```
We only have to use this value by mapping it to a visually relevant value range
```java
theta = map(valueFromArduino, 15, 250, 0, PI);
```
So here is finally the entire processing program:

```java
/**
 * hacked from
 * Recursive Tree
 * by Daniel Shiffman.  
 * 
 * and acousteauphone-prototype from lesporteslogiques
 * by Pierre Commenge
 */

import processing.serial.*;
Serial myPort;  // Create object from Serial class

float valueFromArduino = 0;
float theta;   

void setup() {
  size(640, 360);

 // initialization of communication via usb from arduino
  // BE CAREFUL to use the adapted port
  printArray(Serial.list());
  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw() {
  background(0);
  frameRate(30);
  stroke(255);


  // Convert it to radians
  theta = map(valueFromArduino, 15, 250, 0, PI);
  // Start the tree from the bottom of the screen
  translate(width/2, height);
  // Draw a line 120 pixels
  line(0, 0, 0, -120);
  // Move to the end of that line
  translate(0, -120);
  // Start the recursive branching!
  branch(120);
}

void serialEvent (Serial myPort) {
  try {
    while (myPort.available() > 0) {
      String inBuffer = myPort.readStringUntil('\n');
      if (inBuffer != null) {
        if (inBuffer.substring(0, 1).equals("{")) {
          JSONObject json = parseJSONObject(inBuffer);
          if (json == null) {
            //println("JSONObject could not be parsed");
          } else {
            valueFromArduino    = json.getInt("photores");
          }
        } else {
        }
      }
    }
  } 
  catch (Exception e) {
  }
}

void branch(float h) {
  // Each branch will be 2/3rds the size of the previous one
  h *= 0.66;

  // All recursive functions must have an exit condition!!!!
  // Here, ours is when the length of the branch is 2 pixels or less
  if (h > 2) {
    pushMatrix();    // Save the current state of transformation (i.e. where are we now)
    rotate(theta);   // Rotate by theta
    line(0, 0, 0, -h);  // Draw the branch
    translate(0, -h); // Move to the end of the branch
    branch(h);       // Ok, now call myself to draw two new branches!!
    popMatrix();     // Whenever we get back here, we "pop" in order to restore the previous matrix state

    // Repeat the same thing, only branch off to the "left" this time!
    pushMatrix();
    rotate(-theta);
    line(0, 0, 0, -h);
    translate(0, -h);
    branch(h);
    popMatrix();
  }
}
```
[**home**](#Contents)

#### Control neopixels leds with the mouse

This third example will illustrate the reverse communication: we will send data from processing to arduino. Depending on the position of the mouse the color of the window will be affected and these mouse positions will also be sent to arduino to control the lighting of leds.

<img src="assets/serial_mouse_to_neopixels.gif" width="480" height="270" /><br>

The electrical diagram is simple: it is about connecting a ribbon of LEDs on an arduino card:

<img src="set_neopixel_rgb/set_neopixels.png" width="480" height="270" /><br>

The processing program will use the HSB mode to adjust the hue according to the position on the absciss of the mouse, the saturation according to the ordinate position of the mouse and the brightness according to the state of the mouse (is it whether the button is pressed or not?) This data will be mapped to processing and written to the serial port for reuse in arduino. We will send 3 values, these values ​​will be separated by commas and we will use a semicolon to mark the end of a data packet.

```java
import processing.serial.*;

Serial myPort;  

void setup() {
  size(600, 600); 
  colorMode(HSB,255,255,255);
  // initialize the serial library
  println(Serial.list());
  String portName = Serial.list()[3]; // be careful to use the correct port
  myPort = new Serial(this, portName, 9600);
}

void draw() {
  int pressed = 0;
  if (mousePressed == true) {    
    pressed = 1;         
  } else {                         
    pressed = 0; 
  }  
  int x =int( map(mouseX, 0, width, 0, 255));
  int y =int( map(mouseY, 0, height, 0, 255));
             
  background(x, y, pressed*255);
             
  String s = pressed+","+x+","+y+";";
  println(s);
  myPort.write(s);
}
```
The arduino code will be much more complex than usual, but it is not necessary to understand everything. You can refer to [the original post](http://www.esologic.com/parsing-serial-data-sent-to-arduino/) of the code I use here.

The part we are going to manipulate is located inside *else* in the loop function

```c
// from http://www.esologic.com/parsing-serial-data-sent-to-arduino/

// code needed for receiving and retrieving data from processing
const char EOPmarker = ';'; //This is the end of packet marker
char serialbuf[32]; //This gives the incoming serial some room. Change it if you want a longer incoming.
#include <string.h> // we'll need this for subString
#define MAX_STRING_LEN 20 // like 3 lines above, change as needed.

// code necessary for the operation of leds
#include <FastLED.h>
#define NUM_LEDS 5
CRGBArray<NUM_LEDS> leds;

void setup() {
 
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 9>(leds, NUM_LEDS);
}

void loop() {
    
  if (Serial.available() > 0) { //makes sure something is ready to be read
    // this part of code allows you to recompose the message from processing
    static int bufpos = 0; //starts the buffer back at the first position in the incoming serial.read
    char inchar = Serial.read(); //assigns one byte (as serial.read()'s only input one byte at a time
    if (inchar != EOPmarker) { //if the incoming character is not the byte that is the incoming package ender
      serialbuf[bufpos] = inchar; //the buffer position in the array get assigned to the current read
      bufpos++; //once that has happend the buffer advances, doing this over and over again until the end of package marker is read.
    }

    // inside this else we can recover our values ​​and use them
    else { //once the end of package marker has been read
      serialbuf[bufpos] = 0; //restart the buff
      bufpos = 0; //restart the position of the buff

      int pressed = atoi(subStr(serialbuf, ",", 1)); // get the first value and store it in a variable
      int x = atoi(subStr(serialbuf, ",", 2)); // get back the second ...
      int y = atoi(subStr(serialbuf, ",", 3)); // get back the third ...
        
     // turn on the leds according to our values
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(x, y, pressed *255);
      }
      FastLED.show();

    }
  }
}

// this function is necessary to allow to cut the string from processing
char* subStr (char* input_string, char *separator, int segment_number) {
  char *act, *sub, *ptr;
  static char copy[MAX_STRING_LEN];
  int i;
  strcpy(copy, input_string);
  for (i = 1, act = copy; i <= segment_number; i++, act = NULL) {
    sub = strtok_r(act, separator, &ptr);
    if (sub == NULL) break;
  }
  return sub;
}
```

[**home**](#Contents)

#### Save data to a file

In this example we will focus on reading data from several sensors and we will send them to processing so we can save them to a file on our hard drive.

This may be useful in order to realize data representations based on sensor recordings.

In our case we will record the analog data from two photoresistors, they will be side by side on a prototyping board, which is not ideal; but it is easy to imagine situations where the sensors can be positioned in different places and it would be possible to measure sunshine in different places through the course of day for instance.

[**home**](#Contents)

##### Circuit

The circuit is a fairly classic circuit:

<img src="serial_enregistrer_des_donnees_dans_un_fichier/read_from_photores_x2.png" width="480" height="360" /><br>

##### Code arduino

From the point of view of the arduino code, there is not much new. As before we will write a character string in the format [** JSON **](https://en.wikipedia.org/wiki/JSON) in which we will insert a value read on our analog input.

Our character string will have to look like this
```json
{
    "luminosity" : photoresistor_value
}
```
This arduino code makes it possible to do this with string concatenation.

```c
String json;
  json = "{\"luminosity\":";
  json = json + analogRead(0);
  json = json + "}";
```

In our case we will have two values ​​to save, then it will print the result in the Serial port.

```c
void setup() {
  Serial.begin(9600); // open a connection via the serial cable
}

void loop() {
  int photor1 = analogRead(A0); // read the data on A0 and store them in a variable
  int photor2 = analogRead(A1);// read the data on A1 and store them in a variable

  delay(1000); // wait 1 second
 
  // build a string of characters by concatenation
  String json;
  json = "{\"photor1\":"; // add the first key "photor1"
  json = json + photor1; // we add the first value
  json = json +";\"photor2\":"; // we add the second key "photor2"
  json = json +  photor2;// we add the second value  
  json = json + "}";

  Serial.println(json); // we write our file on the serial port
  // we will be able to recover these values ​​with processing
}
```

[**home**](#Contents)

##### processing code

Again the processing code will be quite similar to what we have done so far in the previous examples.

The only difference is that we will use different functions to load a file from the hard disk, manipulate json objects and save a file to the hard disk.

At first, here is a classic skit that allows you to recover the values ​​and visualize them at the rate at which they arrive.

```java

import processing.serial.*;
Serial myPort;  

int photor1 =0;
int photor2 =0;


void setup() {
  size(800, 500);
  // initialization of communication via usb from arduino
  // BE CAREFUL to use the adapted port
  printArray(Serial.list());
  String portName = Serial.list()[4];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw() {    
  background(0);
  fill(255);

  ellipse(width*0.25, height*0.5, photor1, photor1);
  ellipse(width*0.75, height*0.5, photor2, photor2);
}  

void serialEvent (Serial myPort) {
  try {
    while (myPort.available() > 0) {
      String inBuffer = myPort.readStringUntil('\n');
      if (inBuffer != null) {
        if (inBuffer.substring(0, 1).equals("{")) {
          JSONObject json = parseJSONObject(inBuffer);
          if (json == null) {
            //println("JSONObject could not be parsed");
          } else {
            // recover the data stored in the format json transmitted via usb
            photor1    = json.getInt("photor1"); // get the value corresponding to the key "photor1"
            photor2    = json.getInt("photor2");// get the value corresponding to the key "photor2"
          }
        } else {
        }
      }
    }
  } 
  catch (Exception e) {
  }
}
```

We now have to take care of the recording on the hard disk.

First we need an empty text file called **data.json** in a / data folder located where your program processing is saved. In reality it must not be completely empty but just contain an opening brace and a closing one:

```json
{}
```
this matchs to the basic structure of a json file.

After getting the data from the character string in json format from the arduino and still in the *void serialEvent(Serial myPort)* function 

```java
// recover the data stored in the format json transmitted via usb
photor1    = json.getInt("photor1"); // get the value corresponding to the key "photor1"
photor2    = json.getInt("photor2");
```

We will be able to proceed to the registration:
- first we have to load our json file (for now empty)
  ```java
  JSONArray js  = loadJSONArray("data.json"); // load the file data.json - in which we will add an entry
  ```

- then we will create a new json object that we will fill with new data
  ```java
  JSONObject njs = new JSONObject(); // create a new object json
           // add a timestamp with date and time
  njs.setString("timestamp", year()+"-"+month()+"-"+day()+"-"+hour()+"-"+minute()+"-"+second());
  njs.setInt("luminosity1", photor1); // add the first value
  njs.setInt("luminosity2", photor2); // add the second value
  ```

- then we will add this new object to our loaded file in step 1.
  ```java
  js.append(njs); // add this new object to the previously loaded object
  ```

- finally we save everything on our hard drive
  ```java
  saveJSONArray(js, "data/data.json"); //we save it while overwriting the previous file.
  ```
Whenever data is received we reload the file, add data and overwrite the previous file: as we store all the data sent by arduino.

After making a recording, remember to rename the file *data.json* with a new name and recreate an empty file if you want to restart a recording.

[**home**](#Contents)






---

## Wifi

### OSC 
https://fredrikolofsson.com/f0blog/f0dmx/
https://github.com/lwoodbury/Huzzah32OSC

### httpGet et httpPost 

### MQTT
https://github.com/mqttjs/MQTT.js
https://github.com/256dpi/processing-mqtt
https://github.com/256dpi/arduino-mqtt


https://cables.gl/op/Ops.Net.Mqtt.Mqtt_v2s
https://cables.gl/docs/3_communication/mqtt_arduino/mqtt_arduino

Brokers :
https://www.shiftr.io/cloud#pricing
https://io.adafruit.com/
https://www.maqiatto.com/

---

## Bluetooth


### 1sheeld

### bluetooth serial
https://circuitdigest.com/microcontroller-projects/using-classic-bluetooth-in-esp32-and-toogle-an-led

### p5js ble
https://create.arduino.cc/projecthub/jingwen_zhu/getting-started-with-p5-ble-js-using-arduino-nano-33-ble-120ea6
https://www.uuidgenerator.net/



--- 
## Infra-red


---
## Radio
https://www.gotronic.fr/art-module-rf-434-mhz-wrl-12823-25308.htm
https://www.carnetdumaker.net/articles/communiquer-sans-fil-en-433mhz-avec-la-bibliotheque-virtualwire-et-une-carte-arduino-genuino/
http://robotlabo.eu/pageTuto/tutoSansFil.html