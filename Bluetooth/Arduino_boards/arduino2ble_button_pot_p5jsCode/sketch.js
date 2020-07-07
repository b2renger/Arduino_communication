// Run with google chrome
// The serviceUuid must match the serviceUuid of the device you would like to connect
const serviceUuid = "19b10010-e8f2-537e-4f6c-d104768a1214";
let myCharacteristics
let potChar
let butChar

let buttonValue = 0;
let potValue = 0;

let myBLE;

let menu

function setup() {
  
  // Create a p5ble class
  myBLE = new p5ble();

  createCanvas(600, 600);
  textSize(20);
  textAlign(CENTER, CENTER);

  // Create a 'Connect' button
  menu = QuickSettings.create(0, 0, 'options')
  menu.addButton("Connect", connectToBle)

}

function connectToBle() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid, gotCharacteristics);
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log('characteristics: ', characteristics, characteristics.length);
  myCharacteristics = characteristics;

  for (let i = 0 ; i < myCharacteristics.length ; i++){
    if (myCharacteristics[i].uuid == "19b10012-e8f2-537e-4f6c-d104768a1211") {
      butChar = i;
      myBLE.read(myCharacteristics[i], gotButtonValue);
    }
    else if (myCharacteristics[i].uuid == "19b10012-e8f2-537e-4f6c-d104768a1212"){
      potChar = i
      myBLE.read(myCharacteristics[i], gotPotValue);
    }


  }
  
  //myBLE.read(myCharacteristics.descriptor(1), gotPotValue);
}

// A function that will be called once got values
function gotButtonValue(error, value) {
  if (error) console.log('error: ', error);
  //console.log('button value: ', value);
  buttonValue = value;
  // After getting a value, call p5ble.read() again to get the value again to create a kind of loop
  myBLE.read(myCharacteristics[butChar], gotButtonValue);
}


// A function that will be called once got values
function gotPotValue(error, value) {
  if (error) console.log('error: ', error);
  console.log('pot value: ', value);
  potValue = value;
  // After getting a value, call p5ble.read() again to get the value again to create a kind of loop
  myBLE.read(myCharacteristics[potChar], gotPotValue);
}

function draw() {

  if (buttonValue == 0) {
    background(0)
    stroke(255)
    noFill()
  } else {
    background(255)
    stroke(0)
    noFill()
  }
  rectMode(CENTER)
  let corners = map(potValue, 0, 255, 0, width / 3)
  rect(width / 2, height / 2, width / 3, width / 3, corners)

  fill(255, 0, 0)
  stroke(255,0,0)
  textAlign(CENTER, BOTTOM)
  text("button value : " + buttonValue + " -  pot value : " + potValue, width / 2 ,height)
}