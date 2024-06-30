#ifndef MELODY_H
#define MELODY_H
// soft and simple debounce by kan
// support arduino platform
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "Arduino.h"
#endif

class MELODY {
private:
  int tonePin = 13;
  unsigned long previousMillis = 0;
  unsigned long note = 0;
  const long pauseBetweenNotes = 250;
  const long noteDuration = 400; 
  boolean outputTone = false;

public:
  MELODY(int pin, int frequency) {
    tonePin = pin;
    note = frequency;
  }
  void setup()
  {
    pinMode(tonePin,OUTPUT);
  }
  boolean maketone() {
    unsigned long currentMillis = millis();

    if (outputTone) {
      // We are currently outputting a tone
      // Check if it's been long enough and turn off if so
      if (currentMillis - previousMillis >= noteDuration) {
        previousMillis = currentMillis;
        noTone(tonePin);
        outputTone = false;
      }
    } else {
      // We are currently in a pause
      // Check if it's been long enough and turn on if so
      if (currentMillis - previousMillis >= pauseBetweenNotes) {
        previousMillis = currentMillis;
        tone(tonePin, note);
        outputTone = true;
      }
    }
  }
};
#endif