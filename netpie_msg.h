
// no parameter to fit callback
void netpie_msg() {
  if (client.connected()) {
    client.loop();
    if ((millis() - preves_time_msg) > timeInterval_send_msg) {
      char buffer[16];
      //pading headder
      strcat(msg, "{\"data\":{");

      strcat(msg, "\"Temperatur\":\"");
      strcat(msg, dtostrf(temperature, 3, 2, buffer));
      //Serial.println(temperature);
      strcat(msg, "\",\"Humidity\":\"");
      strcat(msg, dtostrf(humidity, 3, 2, buffer));
      //Serial.println(humidity);
      strcat(msg, "\",\"Water_level\":\"");
      strcat(msg, dtostrf(water_liter, 3, 2, buffer));
      //Serial.println(water_liter);
      strcat(msg, "\",\"Widht\":\"");
      strcat(msg, itoa(widht, buffer, 10));
      //Serial.println(widht);
      strcat(msg, "\",\"Length\":\"");
      strcat(msg, itoa(length, buffer, 10));
      //Serial.println(length);
      strcat(msg, "\",\"Raduis\":\"");
      strcat(msg, itoa(radius, buffer, 10));
      //Serial.println(radius);
      strcat(msg, "\",\"Deep\":\"");
      strcat(msg, itoa(deep, buffer, 10));
      //Serial.println(deep);
      strcat(msg, "\",\"Container\":\"");
      strcat(msg, shape_name[(byte)container].c_str());
      //strcat(msg, "cube");
      //Serial.println(shape_name[(byte)container]);
      strcat(msg, "\",\"Alarm\":\"");
      strcat(msg, alarm_string[alarm_state].c_str());
      //Serial.println(alarm_string[alarm_state]);
      //pad ending msg
      strcat(msg, "\"}}");
      client.publish("@shadow/data/update", msg);
      ////Serial.println(msg);
      //Serial.println(msg);
      memset(msg, 0, MAX_MSG * (sizeof msg[0]));
      //Serial.println(msg);
      /*lcd.setCursor(0, 1);
      sprintf(lcd_buffer1, "%-16s","PUSH MESSAGE");
      lcd.print(lcd_buffer1);*/
      preves_time_msg = millis();
    }
  } else {
    reconnect();
  }
}