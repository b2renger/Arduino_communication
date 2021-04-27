// Run with google chrome
// The serviceUuid must match the serviceUuid of the device you would like to connect

// based on : https://editor.p5js.org/sk/sketches/MFbPLLJg1xs

const serviceUuid = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
let bluetoothDevice;
let myCharacteristic;

let connected = false;

function setup() {


 

  menu = QuickSettings.create(0, 0, 'options')
  menu.setWidth(windowWidth / 2)

  menu.addButton("Connect", connectToBle)




}

async function connectToBle() {

  bluetoothDevice = null;
  try {
    console.log('Requesting any Bluetooth Device...');
    bluetoothDevice = await navigator.bluetooth.requestDevice({
      filters: [{
        name: "ESP32"
      }],
      optionalServices: ['4fafc201-1fb5-459e-8fcc-c5c9c331914b']
    });
    bluetoothDevice.addEventListener('gattserverdisconnected', onDisconnected);
    connect();
  } catch (error) {
    console.log('Argh! ' + error);
  }
}


async function connect() {
  exponentialBackoff(99 /* max retries */ , 1 /* seconds delay */ ,
    async function toTry() {
        time('Connecting to Bluetooth Device... ');
        const server = await bluetoothDevice.gatt.connect();
        console.log('Getting Service...');
        const service = await server.getPrimaryService('4fafc201-1fb5-459e-8fcc-c5c9c331914b');

        console.log('Getting Characteristic...');
        myCharacteristic = await service.getCharacteristic("beb5483e-36e1-4688-b7f5-ea07361b26a8");

        await myCharacteristic.startNotifications();

        console.log('> Notifications started');
        myCharacteristic.addEventListener('characteristicvaluechanged',
          handleNotifications);
      },
      function success() {
        console.log('> Bluetooth Device connected. Try disconnect it now.');
        console.log('> Bluetooth Device connected. Try disconnect it now.');
        console.log('> Bluetooth Device connected. Try disconnect it now.');
        console.log('> Bluetooth Device connected. Try disconnect it now.');
        console.log('> Bluetooth Device connected. Try disconnect it now.');
        console.log('> Bluetooth Device connected. Try disconnect it now.');
        console.log('> Bluetooth Device connected. Try disconnect it now.');
        console.log('> Bluetooth Device connected. Try disconnect it now.');
        console.log('> Bluetooth Device connected. Try disconnect it now.');
      },
      function fail() {
        time('Failed to reconnect.');
      });
}

function onDisconnected() {
  console.log('> Bluetooth Device disconnected');
  //connect();
}

function handleNotifications(event) {
  var blee = decode(event.target.value); //.getUint8(0);
  var nums = blee.split(',');
  nums = nums.map(parseFloat);
  //console.log(JSON.stringify(nums));
 
  // let value = event.target.value;
  // let a = [];
  // // Convert raw data bytes to hex values just for the sake of showing something.
  // // In the "real" world, you'd use data.getUint8, data.getUint16 or even
  // // TextDecoder to process raw data bytes.
  // for (let i = 0; i < value.byteLength; i++) {
  //   a.push('0x' + ('00' + value.getUint8(i).toString(16)).slice(-2));
  // }
  // log('> ' + a.join(' '));
}


/* Utils */

// This function keeps calling "toTry" until promise resolves or has
// retried "max" number of times. First retry has a delay of "delay" seconds.
// "success" is called upon success.
async function exponentialBackoff(max, delay, toTry, success, fail) {
  try {
    const result = await toTry();
    success(result);
  } catch (error) {
    if (max === 0) {
      return fail();
    }
    time('Retrying in ' + delay + 's... (' + max + ' tries left)');
    setTimeout(function () {
      exponentialBackoff(--max, delay, toTry, success, fail);
    }, delay * 1000);
  }
}

function time(text) {
  console.log('[' + new Date().toJSON().substr(11, 8) + '] ' + text);
}

/* helper function to decode message sent from peripheral */
function decode(buf) {
  let dec = new TextDecoder("utf-8");
  return dec.decode(buf);
}