#ifndef DEBOUNCE_H
#define DEBOUNCE_H
// soft and simple debounce by kan
// support arduino platform
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "Arduino.h"
#endif

class DEBOUNCE
{
  private:
  int BUTTON_PIN=0;      
  int DEBOUNCE_DELAY = 50;
  int buton_SteadyState = HIGH;
  int button_laststate = HIGH;
  int button_currentState;            
  unsigned long lastDebounceTime = 0;
  int state_return; 

  public:
  DEBOUNCE(int pin,int time_debounce,int first,int last)
  {
    DEBOUNCE_DELAY = time_debounce;
    BUTTON_PIN = pin;
    buton_SteadyState=first;
    button_laststate=last;
  }
  void setup()
  {
    pinMode(BUTTON_PIN,INPUT_PULLUP);
  }
  boolean ReadState()
  {
    button_currentState = digitalRead(BUTTON_PIN);
    if (button_currentState != button_laststate) 
    {
      lastDebounceTime = millis();
      button_laststate = button_currentState;
    }
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) 
    {
      if(buton_SteadyState == HIGH && button_currentState == LOW)
      {  
        state_return = LOW;
      }
      else
      {
        state_return = HIGH;
      }
      buton_SteadyState = button_currentState;
      return state_return;
    }
  }
};
#endif