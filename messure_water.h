void mesure_water_view() {
  /*if ((millis() - preves_time_messure) > timeInterval_messure) {
    distance = ultrasonic_sensor.echo();
    preves_time_messure = millis();
    }*/
  //distance=ultrasonic_sensor.messure();
  lcd.setCursor(0, 0);
  sprintf(lcd_buffer0, "Distance:%03.02f    ", distance);
  lcd.print(lcd_buffer0);

  if (distance > 0) {
    float t_deep = deep - distance;
    
    if (t_deep > 0)
    {
      if (container == cylinder) {
        water_liter = 3.14 * (radius * radius) * t_deep;
      }
      if (container == cube) {
        water_liter = (((float)(widht * length) * t_deep)) / 1000.0;
      }
      if(pre_deep<t_deep)
      {
        water_level_state=water_level_string[0];
        pre_deep=t_deep;
      }
      else
      {
        if(pre_deep>t_deep)
        {
          water_level_state=water_level_string[1];
          pre_deep=t_deep;
        }
      }
      if (t_deep <= deep)
      {
        lcd.setCursor(0, 1);
        sprintf(lcd_buffer1, "WaterLit:%-04.2f", water_liter);
        lcd.print(lcd_buffer1);
      }
      else
      {
        if (t_deep > deep)
        {
          lcd.setCursor(0, 1);
          sprintf(lcd_buffer1, "Water:%-10s", "OverFlow");
          lcd.print(lcd_buffer1);
        }
      }
    }
    else
    {
      if (t_deep <= 0)
      {
        lcd.setCursor(0, 1);
        sprintf(lcd_buffer1, "WaterLit:%-7s", "Emty");
        lcd.print(lcd_buffer1);
      }
    }
  }
  else
  {
    water_liter = 0;
    lcd.setCursor(0, 1);
    sprintf(lcd_buffer1, "WaterLit:%-7s", "Error");
    lcd.print(lcd_buffer1);
  }
}

void wateralarm() {
  if (water_liter >= alarm_water) {
    alarm_state = 1;
    sound1.maketone();
    //digitalWrite(sound_pin, HIGH);
  } else {
    alarm_state = 0;
    sound1.no_sound();
    //digitalWrite(sound_pin, LOW);
  }
}
