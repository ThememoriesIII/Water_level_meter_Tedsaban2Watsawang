//define pin section
#define set_pin 15
#define up_pin 27
#define down_pin 25
#define left_pin 26
#define right_pin 16
#define exit_pin 17
#define button_debounce_time 50
#define sound_pin 13
#define sound_frequency 10000
#define EEPROM_SIZE 7
unsigned char LED = 2;
#define pingPin 5
#define inPin 18

//include section
#include <Wire.h>

#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
// include wifi section
#include <DNSServer.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
//#include <ArduinoJson.h>
/*
#ifdef __cplusplus
extern "C" {
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();
*/
/*
#include "driver/temp_sensor.h"

void initTempSensor(){
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac_offset = TSENS_DAC_L2;  // TSENS_DAC_L2 is default; L4(-40°C ~ 20°C), L2(-10°C ~ 80°C), L1(20°C ~ 100°C), L0(50°C ~ 125°C)
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
}
*/
//include debounce button and ultrasonic
#include "debounce.h"
#include "ultrasonic.h"
#include "melody_kan.h"

MELODY sound1(sound_pin,sound_frequency);

//Wifi Manager

//WiFiManager wifimanager;
WiFiManager wifimanager;
#define MAX_MSG 512
char msg[MAX_MSG];
//netpie

const char* mqtt_server = "mqtt.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "c040a1fb-170d-48f7-b6a1-19e03c37b11f";
const char* mqtt_username = "Mb7gTnyPtmo2ZtWMzU99iyAxewA55MtG";
const char* mqtt_password = "O67U2XKRbB_Q4HfhA9TSRlz0n3RHsmZB";



WiFiClient espClient;
PubSubClient client(espClient);



//lcd section
LiquidCrystal_I2C lcd(0x27, 16, 2);
char lcd_buffer0[17];
char lcd_buffer1[17];



//ultrasonic section
ULTRASONIC ultrasonic_sensor(pingPin, inPin);

long num_count = 0;
unsigned char led_state = 0;

unsigned long preves_time_msg = 0;
unsigned int timeInterval_send_msg = 2000;

unsigned long preves_time_messure = 0;
unsigned int timeInterval_messure = 1000;

float water_liter = 0;

//menu section
enum mode {
  messure = 0,
  seting = 1,
  set_wide = 2,
  set_deep = 3,
  set_length = 4,
  set_radius = 5,
  show_ip = 6,
  water_alarm = 7
};
/*enum seting_parameter{
  set_wide=1,
  set_deep=2,
  set_length=3,
  set_radius=4
};*/
enum shape {
  cylinder = 0,
  cube = 1,
  thetradom = 2,
  cone = 3
};
String shape_name[4] = { "cylinder", "cube", "thetradom", "cone" };
shape container = cube;
shape container_buffer = cube;
byte shape_index_eeeprom = 6;
byte shape_count = 0;

byte menu_count = 0;
mode opration = messure;
byte press = LOW;
byte not_press = HIGH;

//water level section
float temperature = 0.0;
float humidity = 0.0;
long distance = 0;
byte widht = 0;
byte deep = 0;
byte radius = 0;
byte length = 0;
byte alarm_water = 0;
byte widht_buffer = 0;
byte deep_buffer = 0;
byte radius_buffer = 0;
byte length_buffer = 0;
byte alarm_buffer = 0;

byte alarm_state = 0;
String alarm_string[2] = { "not alarm", "alarm" };

byte some_limiter = 0;

byte relay[4] = { 0, 0, 0, 0 };

void configModeCallback(WiFiManager* mywifimanager) {
  lcd.setCursor(0, 0);
  sprintf(lcd_buffer0, "%-16s", "Enter Config");
  lcd.print(lcd_buffer0);
  /*lcd.setCursor(0,1);
  sprintf(lcd_buffer1,"IP %-12s",WiFi.localIP());
  lcd.print(lcd_buffer1);*/
  lcd.setCursor(0, 1);
  sprintf(lcd_buffer1, "%-16s", mywifimanager->getConfigPortalSSID());
  lcd.print(lcd_buffer1);
}

//function call on menu section
void Option_base(char* string_col0, char* string_col1, byte* buffer_variable, byte* true_variable, byte index);
typedef void (*Option)(char* string_col0, char* string_col1, byte* buffer_variable, byte* true_variable, byte index);
Option opstion1_setting_time = Option_base;
Option opstion2_setting_temp = Option_base;

//
DEBOUNCE button_Set_pin(set_pin, button_debounce_time, HIGH, HIGH);
DEBOUNCE button_up_pin(up_pin, button_debounce_time, HIGH, HIGH);
DEBOUNCE button_down_pin(down_pin, button_debounce_time, HIGH, HIGH);
DEBOUNCE button_left_pin(left_pin, button_debounce_time, HIGH, HIGH);
DEBOUNCE button_right_pin(right_pin, button_debounce_time, HIGH, HIGH);
DEBOUNCE button_exit_pin(exit_pin, button_debounce_time, HIGH, HIGH);

//base function in menu
void Option_base(char* string_col0, char* string_col1, byte* buffer_variable, byte* true_variable, byte index) {
  //*buffer_variable=*true_variable;
  if (button_up_pin.ReadState() == press) {
    ++(*buffer_variable);
    Serial.print(string_col0);
    Serial.println("press up");
  }
  if (button_down_pin.ReadState() == press) {
    --(*buffer_variable);
    Serial.print(string_col0);
    Serial.println("press down");
  }
  if (*buffer_variable > 255 || *buffer_variable < 0) {
    *buffer_variable = 0;
  }
  if(button_right_pin.ReadState()==press)
  {
    *buffer_variable=(byte)0;
    *true_variable=(byte)0;
  }
  if (button_left_pin.ReadState() == press) {
    if (*true_variable != *buffer_variable) {
      byte check_eeeprom = EEPROM.read(index);
      if (check_eeeprom != *buffer_variable) {
        Serial.println("Commit");
        EEPROM.write(index, *buffer_variable);
        EEPROM.commit();
      }
      *true_variable = *buffer_variable;
    }
    opration = messure;
  }
  //กำหนดcusorการเขียนข้อมูลลงจอlcdให้เป็นบรรทัดแรกตัวอกษรแรกั
  lcd.setCursor(0, 0);
  //แสผงผลข้้อความ TEMP Limiter Set ที่บรรทัดแรก
  sprintf(lcd_buffer0, "%-16s", string_col0);
  lcd.print(lcd_buffer0);
  //กำหนดcusorการเขียนข้อมูลลงจอlcdให้เป็นบรรทัดที่สองตัวอกษรแรกั
  lcd.setCursor(0, 1);
  //แสผงผลข้้อความ Value = และค่าที่คั้งใหม่ ที่บรรทัดแรก ตัวอย่าง Value = 001
  //char lcd_buffer[16];
  sprintf(lcd_buffer1, "%s%-08d", string_col1, *buffer_variable);
  lcd.print(lcd_buffer1);
  //lcd.printf("%s%03d", string_col1, buffer_setMole_Temp);
}

// normal function to select shape
void shape_setting() {
  byte start_select = container_buffer;
  byte end_select = cube;
  if (button_up_pin.ReadState() == press) {
    shape_count++;
  }
  if (button_down_pin.ReadState() == press) {
    shape_count--;
  }
  if (shape_count > end_select || shape_count < 0) {
    shape_count = 0;
  }
  container_buffer = (shape)shape_count;
  if (container_buffer > end_select)
    container_buffer = (shape)start_select;
  if(button_right_pin.ReadState()==press)
  {
    container_buffer=(shape)0;
    container=(shape)0;
  }
  if (button_left_pin.ReadState() == press) {
      byte check_eeeprom = EEPROM.read(shape_index_eeeprom);
      if (check_eeeprom != container_buffer) {
        EEPROM.write(shape_index_eeeprom, (byte)container_buffer);
        EEPROM.commit();
      }
      container = container_buffer;
    opration = messure;
  }
  //char lcd_buffer[16];
  //กำหนดcusorการเขียนข้อมูลลงจอlcdให้เป็นบรรทัดแรกตัวอกษรแรกั
  lcd.setCursor(0, 0);
  //แสผงผลข้้อความ TEMP Limiter Set ที่บรรทัดแรก
  sprintf(lcd_buffer0, "shape Mode:%05d ", shape_count);
  lcd.print(lcd_buffer0);
  //กำหนดcusorการเขียนข้อมูลลงจอlcdให้เป็นบรรทัดที่สองตัวอกษรแรกั
  lcd.setCursor(0, 1);
  //แสผงผลข้้อความ Value = และค่าที่คั้งใหม่ ที่บรรทัดแรก ตัวอย่าง Value = 001

  sprintf(lcd_buffer1, "shape:%-9s", shape_name[shape_count]);
  lcd.print(lcd_buffer1);
  //lcd.printf("%s%03d", string_col1, buffer_setMole_Temp);
}

void reconnect() {
  while (!client.connected()) {
    lcd.setCursor(0, 0);
    sprintf(lcd_buffer0, "%-16s", "cant CONNECT");
    lcd.print(lcd_buffer0);
    //Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      lcd.setCursor(0, 1);
      sprintf(lcd_buffer1, "%-16s", "Connect DONE");
      lcd.print(lcd_buffer1);
      //client.subscribe("@msg/led");
    } else {
      lcd.setCursor(0, 0);
      sprintf(lcd_buffer0, "%-16s", "FAIL RECONECT");
      lcd.print(lcd_buffer0);
      lcd.setCursor(0, 1);
      sprintf(lcd_buffer1, "%-16s", "TRY CONECT AGAIN");
      lcd.print(lcd_buffer1);
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  //Serial.println(message);
  if (String(topic) == "@msg/led") {
    if (message == "on") {
      client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"on\"}}");
      lcd.setCursor(0, 1);
      sprintf(lcd_buffer1, "%-16s", "RECIVE DATA ON");
      lcd.print(lcd_buffer1);
    } else if (message == "off") {
      client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"off\"}}");
      lcd.setCursor(0, 1);
      sprintf(lcd_buffer1, "%-16s", "RECIVE DATA OFF");
      lcd.print(lcd_buffer1);
    }
  }
}