// Run with google chrome
// The serviceUuid must match the serviceUuid of the device you would like to connect
const serviceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
let myCharacteristic;

let myBLE;

let connected = false;

function setup() {
  myBLE = new p5ble();

  // Create a 'Connect' button
  const connectButton = createButton('Connect')
  connectButton.mousePressed(connectToBle);

  menu = QuickSettings.create(0, 0, 'options')
  menu.setWidth(windowWidth/2)

  menu.addButton("Connect", connectToBle)

  menu.addRange("Red", 0, 255, 100, 1, function (v) {
    if (connected) {
      myBLE.write(myCharacteristic[0], v);
      myBLE.write(myCharacteristic[3], 1); // to enter the if statement updating pixels values
    }
  })

  menu.addRange("Green", 0, 255, 100, 1, function (v) {
    if (connected) {
      myBLE.write(myCharacteristic[1], v);
      myBLE.write(myCharacteristic[3], 1); // to enter the if statement updating pixels values
    }
  })

  menu.addRange("Blue", 0, 255, 100, 1, function (v) {
    if (connected) {
      myBLE.write(myCharacteristic[2], v);
      myBLE.write(myCharacteristic[3], 1); // to enter the if statement updating pixels values
    }
  })


  menu.addButton("On", function () {
    if (connected) myBLE.write(myCharacteristic[3], 1);
  })

  menu.addButton("Off", function () {
    if (connected) myBLE.write(myCharacteristic[3], 0);
  })


}

function connectToBle() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid, gotCharacteristics);
}

function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log('characteristics: ', characteristics);
  // Set the first characteristic as myCharacteristic
  myCharacteristic = characteristics;
  connected = true

}