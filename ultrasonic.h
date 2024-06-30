#ifndef ULTRASONIC_H
#define ULTRASONIC_H

class ULTRASONIC
{
  private:
  byte trig_pin=0;
  byte echo_pin=0;
  unsigned long duration = 0;
  float temp=0;
  float v=0;

  public:
  ULTRASONIC(byte pingpin,byte inpin)
  {
    trig_pin = pingpin;
    echo_pin = inpin;
  }
  void begin()
  {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
  }
  // echo first and get time duration and convert to centimeter for millimeter 
  //echo ultrasonic to piont and wait sound to return to tranducer for messure time
  float echo()
  {
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig_pin, LOW);
    duration = pulseIn(echo_pin, HIGH);
    return to_centimeter();
    //v=29;
    //return duration;
  }
  void echo(float temp_0)
  {
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig_pin, LOW);
    duration = pulseIn(echo_pin, HIGH);
    //temp = ((temp_0-32)/1.8);
    //v=1.0/(((331.5+(0.61*temp))*100.0)/pow(10,6.0));
    //Serial.println(temp);
     //Serial.println(v);
    //return duration;
  }

  //microsecond to centimeters
  unsigned long to_centimeter()
  {
    //Serial.print("Duration");
    //Serial.println(duration);
    unsigned int centimeter=0;
    if (duration == 0 || duration < 0)
      centimeter = 0;
    else
      centimeter=(duration *0.03492 )/ 2.0;
      //centimeter=duration / v / 2.0;
      //centimeter = duration / 29 / 2;
    return centimeter;
  }
};
#endif
