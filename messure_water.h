void mesure_water_level() {
  if ((millis() - preves_time_messure) > timeInterval_messure) {
    distance = ultrasonic_sensor.echo();
    preves_time_messure = millis();
  }
  
  lcd.setCursor(0, 0);
  sprintf(lcd_buffer0, "Distance:%03.02f    ", distance);
  lcd.print(lcd_buffer0);
  
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