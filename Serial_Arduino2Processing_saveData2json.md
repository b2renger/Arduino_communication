[Back to main page](README.md)

# Save data to a file

In this example we will focus on reading data from several sensors and we will send them to processing so we can save them to a file on our hard drive.

This may be useful in order to realize data representations based on sensor recordings.

In our case we will record the analog data from two photoresistors, they will be side by side on a prototyping board, which is not ideal; but it is easy to imagine situations where the sensors can be positioned in different places and it would be possible to measure sunshine in different places through the course of day for instance.



## Circuit

The circuit is a fairly classic circuit:

<img src="serial/processing/serial_enregistrer_des_donnees_dans_un_fichier/read_from_photores_x2.png" width="480" height="360" /><br>

## Code arduino

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

## Processing code

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

[Back to main page](README.md)

