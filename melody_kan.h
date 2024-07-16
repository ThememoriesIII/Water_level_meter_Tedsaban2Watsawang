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
    int PWM_Res = 8;
    int tonePin = 13;
    int chanel = 0;
    unsigned long previousMillis = 0;
    unsigned long previousMillis2 = 0;
    unsigned long note = 0;
    const long pauseBetweenNotes = 250;
    const long noteDuration = 400;
    boolean outputTone = false;
    unsigned long currentMillis = 0;
    unsigned long currentMillis2 = 0;
  public:
    MELODY(int pin, int frequency, int chanel_in) {
      tonePin = pin;
      note = frequency;
      chanel  = chanel_in;
      ledcAttachPin( tonePin, chanel );
    }
    void setup()
    {
      pinMode(tonePin, OUTPUT);
      ledcAttachPin( tonePin, chanel );
      ledcSetup( chanel, note, PWM_Res );
    }
    void maketone() {
      currentMillis = millis();
      if (outputTone) {
        if (currentMillis - previousMillis >= 1000) {
          ledcWrite(chanel, 0);
          previousMillis = currentMillis;
          outputTone = false;
        }
      }
      else {
          if (currentMillis - previousMillis >= 1000) {
            ledcWrite(chanel, 127);
            previousMillis = currentMillis;
            outputTone = true;
        }
      }
      /*if (outputTone) {
        // We are currently outputting a tone
        // Check if it's been long enough and turn off if so
        if (currentMillis - previousMillis >= noteDuration) {
          previousMillis = currentMillis;
          //ledcAttachPin( tonePin, chanel );
          //ledcWrite(chanel, 0);
          ledcWrite(chanel,0);
          outputTone = false;
        }
        } else {
        // We are currently in a pause
        // Check if it's been long enough and turn on if so
        if (currentMillis - previousMillis >= pauseBetweenNotes) {
          previousMillis = currentMillis;
          //ledcAttachPin( tonePin, chanel );
          //ledcWrite( chanel, 127 );
          ledcWrite( chanel, 127 );
          outputTone = true;
        }
        }*/
    }
    void no_sound()
    {
      ledcDetachPin(tonePin);
    }
};
#endif
