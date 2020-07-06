/*
   IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
*/

#include <IRremote.h>
#include <FastLED.h>
#define NUM_LEDS 16 // définir le nombre de leds

CRGBArray<NUM_LEDS> leds;
int RECV_PIN = 10;
int on;
int r = 255;
int g = 255;
int b = 255;
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }
  delay(100);

  if ( results.value == 3772833823) {
    for (int i = 0; i < NUM_LEDS; i++) {
      // on change la valeur de la led 'i' du tableau nommé 'leds" en lui donnant une nouvelle valeur RGB
      leds[i] = CRGB(r,g,b);
      on = 1;
    }
    FastLED.show(); // on actualise le ruban de led
  }
    if ( results.value == 2832539636 || results.value == 4060801047) {
    for (int i = 0; i < NUM_LEDS; i++) {
      // on change la valeur de la led 'i' du tableau nommé 'leds" en lui donnant une nouvelle valeur RGB
      leds[i] = CRGB(0,0,0);
      on = 0;
    }
    FastLED.show(); // on actualise le ruban de led
  }


    if ( on == 1 && results.value == 551512215) {
      r = r +50;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(r,r,r);
    }
    FastLED.show(); // on actualise le ruban de led
  }
      if ( on == 1 && results.value == 551544855) {
      r = r -50;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(r,r,r);
    }
    FastLED.show(); // on actualise le ruban de led
  }
  results.value = 0;
}
