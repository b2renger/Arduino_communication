/*
adapted from https://github.com/tigoe/mqtt-examples/blob/master/p5js-mqtt-client/public/sketch.js
//https://public.cloud.shiftr.io/

https://github.com/tigoe/mqtt-examples
*/


// MQTT client details:
let broker = {
  hostname: 'public.cloud.shiftr.io',
  port: 443
};
// MQTT client:
let client;
// client credentials:
let creds = {
  clientID: 'p5Client',
  userName: 'public',
  password: 'public'
}


let sliderR;
let psliderR = 0

let connected = false;

function setup() {
  // put setup code here
  createCanvas(100, 100);

  client = new Paho.MQTT.Client(broker.hostname, Number(broker.port), creds.clientID);
  // set callback handlers for the client:
  client.onConnectionLost = onConnectionLost;
  client.onMessageArrived = onMessageArrived;
  // connect to the MQTT broker:
  client.connect({
    onSuccess: onConnect, // callback function for when you connect
    userName: creds.userName, // username
    password: creds.password, // password
    useSSL: true // use SSL
  });


  sliderR = createSlider(0, 255, 100);
  sliderR.position(10, 10);
  sliderR.style('width', '80px');

}

function draw() {
  // put drawing code here
  background(180)

  let v = sliderR.value();
  if (psliderR != v) {
    if (connected) {
      let message = new Paho.MQTT.Message(String(v)); // start an MQTT message:
      message.destinationName = "col";
      client.send(message);
      console.log("sending slider R value" + v)
    }
  }
  psliderR = v;


}


// called when the client connects
function onConnect() {
  console.log("connected")
  client.subscribe("col");
  connected = true
}

// called when the client loses its connection
function onConnectionLost(response) {
  if (response.errorCode !== 0) {
    console.log(response.errorMessage)
  }
}

// called when a message arrives
function onMessageArrived(message) {
  console.log("incoming message : " + message.payloadString)
  /*
  let incomingNumber = parseInt(message.payloadString);
  if (incomingNumber > 0) {
    intensity = 255;
  }*/
}