void mesure_water_level() {
  if ((millis() - preves_time_messure) > timeInterval_messure) {
    //temperature=temperature();
    //temperature = temprature_sens_read();
    //Serial.println(temperature);
    //temperature=31.5;
    //temp_sensor_read_celsius(&temperature);
    ultrasonic_sensor.echo();
    /*if (temperature < 0 || temperature == 0) {
      //Serial.println("loop v");
      ultrasonic_sensor.echo();
    } else {
      //Serial.println("loop temp");
      ultrasonic_sensor.echo(temperature);
    }*/
    distance = ultrasonic_sensor.to_centimeter();
    //Serial.println(distance);
    preves_time_messure = millis();
  }
  /*
  ultrasonic_sensor.echo();
  distance = ultrasonic_sensor.to_centimeter();*/
  lcd.setCursor(0, 0);
  sprintf(lcd_buffer0, "Distance:%03d    ", distance);
  lcd.print(lcd_buffer0);
  //lcd.print("Distance:");
  //digit_ultrasonic(distance);
  if (distance>0) {
    if (container == cylinder) {
      water_liter = 3.14 * (radius * radius) * (float)abs(deep - distance);
    }
    if (container == cube) {
      water_liter = (((float)(widht * length) * (float)abs(deep - distance))) / 1000.0;
    }
    if (abs(deep - distance) > 2) {
    lcd.setCursor(0, 1);
    sprintf(lcd_buffer1, "WaterLit:%-04.2f", water_liter);
    lcd.print(lcd_buffer1);
  } else {
    if (distance > deep) {
      lcd.setCursor(0, 1);
      sprintf(lcd_buffer1, "WaterLit:%-7s", "Emty");
      lcd.print(lcd_buffer1);
    } else {
      if (abs(deep - distance) < 2) {
        lcd.setCursor(0, 1);
        sprintf(lcd_buffer1, "Water:%-10s", "OverFlow");
        lcd.print(lcd_buffer1);
      }
    }
  }
  }
  else
  {
    water_liter=0;
    lcd.setCursor(0, 1);
    sprintf(lcd_buffer1, "WaterLit:%-7s", "Error");
    lcd.print(lcd_buffer1);
  }
}

void wateralarm() {
  if (water_liter >= alarm_water) {
    alarm_state = 1;
    //sound1.maketone();
    //digitalWrite(sound_pin, HIGH);
  } else {
    alarm_state = 0;
    //noTone(sound_pin);
    //digitalWrite(sound_pin, LOW);
  }
}