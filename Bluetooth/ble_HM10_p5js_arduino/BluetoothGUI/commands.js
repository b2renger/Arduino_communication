function LEDon() {
  sendData("LED Color" + ledColorPicker.value()+ "\n");
}
function LEDoff() {
  sendData("LED OFF\n");
}
