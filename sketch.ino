#include "headder.h"
#include "messure_water.h"
#include "netpie_msg.h"


void setup() {
  //pinMode(sound_pin,OUTPUT);
  //initTempSensor();
  ultrasonic_sensor.begin();
  EEPROM.begin(EEPROM_SIZE);
  sound1.setup();
  Serial.begin(115200);
  //ultrasonic initial
  //lcd initial
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Start");

  //wifi initial
  wifimanager.setTimeout(30);
  wifimanager.setBreakAfterConfig(true);
  wifimanager.setAPCallback(configModeCallback);
  //wifimanager.autoConnect();
  if (!wifimanager.autoConnect("SmartDevice")) {
    lcd.setCursor(0, 1);
    sprintf(lcd_buffer1, "%-16s", "failconnect-wifi");
    lcd.print(lcd_buffer1);
    delay(3000);
    ESP.restart();
    delay(5000);
  }
  lcd.setCursor(0, 1);
  sprintf(lcd_buffer1, "%-16s", "connect-wifiRedy");
  lcd.print(lcd_buffer1);
  //button initial
  button_Set_pin.setup();
  button_up_pin.setup();
  button_down_pin.setup();
  button_left_pin.setup();
  button_right_pin.setup();
  button_exit_pin.setup();
  //led indicator
  pinMode(LED, OUTPUT);
  lcd.setCursor(0, 1);
  sprintf(lcd_buffer1, "IP:%-13s", WiFi.localIP().toString());
  lcd.print(lcd_buffer1);
  delay(1000);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(revc_callback);

  widht = (byte)EEPROM.read(0);
  deep = (byte)EEPROM.read(1);
  radius = (byte)EEPROM.read(3);
  length = (byte)EEPROM.read(2);
  container = (shape)EEPROM.read(4);
  alarm_water = (byte)EEPROM.read(5);
  container = (shape)EEPROM.read(6);
  
  widht_buffer = widht;
  deep_buffer = deep;
  radius_buffer = radius;
  length_buffer = length;

  alarm_buffer = alarm_water;
  shape_count = container;


  if (container > cube) {
    container = cube;
  }
  preves_time_msg = millis();
}

void loop() {

  if (button_Set_pin.ReadState() == press) {
    menu_count++;
    //Serial.print(menu_count);
    if (container == cylinder) {
      if (menu_count == set_wide) {
        menu_count = set_deep;
      } else {
        if (menu_count == set_length) {
          menu_count = set_radius;
        }
      }
      if (menu_count > water_alarm) {
        menu_count = messure;
      }
    } else {
      if (container == cube) {
        if (menu_count == set_radius) {
          menu_count = show_ip;
        }
        if (menu_count > water_alarm) {
          menu_count = messure;
        }
      }
    }
    opration = (mode)menu_count;
  }
  if (button_exit_pin.ReadState() == press) {
    if (opration != messure) {
      opration = messure;
      widht_buffer = widht;
      deep_buffer = deep;
      radius_buffer = radius;
      length_buffer = length;
      alarm_buffer = alarm_water;
      shape_count = container;
    }
  }

  distance=ultrasonic_sensor.messure();

  if (opration == messure) {
    //temperature=temprature_sens_read();
    mesure_water_view();
  } else {
    if (opration == seting) {
      shape_setting();
    } else {
      if (opration == set_wide) {
        opstion2_setting_temp("Widht shape Set", "Value = ", &widht_buffer, &widht, 0);
      } else {
        if (opration == set_deep) {
          opstion2_setting_temp("Deep shape Set", "Value = ", &deep_buffer, &deep, 1);
        } else {
          if (opration == set_length) {
            opstion2_setting_temp("Lengt shape Set", "Value = ", &length_buffer, &length, 2);
          } else {
            if (opration == set_radius) {
              opstion2_setting_temp("raduis shape Set", "Value = ", &radius_buffer, &radius, 3);
            } else {
              if (opration == show_ip) {
                lcd.setCursor(0, 0);
                sprintf(lcd_buffer0, "%-16s", WiFi.SSID());
                lcd.print(lcd_buffer0);
                lcd.setCursor(0, 1);
                sprintf(lcd_buffer1, "IP:%-13s", WiFi.localIP().toString());
                lcd.print(lcd_buffer1);
              } else {
                if (opration == water_alarm) {
                  opstion2_setting_temp("Water Alarm Set", "Value = ", &alarm_buffer, &alarm_water, 5);
                }
              }
            }
          }
        }
      }
    }
  }
  wateralarm();
  netpie_msg();
}