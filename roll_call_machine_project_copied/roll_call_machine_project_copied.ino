#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <Adafruit_Fingerprint.h>
#include <DFRobot_MLX90614.h>
#include "extern_variable.h"

DFRobot_MLX90614_I2C sensor(MLX90614_I2C_ADDR, &Wire);

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger(&mySerial);

char timeResult[20];
float objectTemp;

void setup()
{
  Serial.begin(115200);

  pinMode(irSensorPin, INPUT_PULLUP);
  sensor.begin();

  tempSensorSetup();
  wifiSetup();
  lineSetup();
  lcdSetup();
  timeSetup();
  bumperSetup();
}

void loop()
{
  int L = digitalRead(irSensorPin);

  if (digitalRead(irSensorPin) == LOW)
  {
    tempSensorGet();

    LINE.notify("Obstacle detected");
    LINE.notify(objectTemp);
    localTime();
    lcdTimePrint();
    lcdDetectedPrint(objectTemp);
    LINE.notify(timeResult);

    bumperWork();
    delay(2000);
  }

  else
  {
    LINE.notify("=== All clear");
    lcdUndetectedPrint();
  }
}

void wifiSetup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void lineSetup()
{
  LINE.setToken(LINE_TOKEN);
  LINE.notify("Line Notify Link Confirm.");
}

void lcdSetup()
{
  lcd.init();
  lcd.backlight();

  lcd.print("LCD Ready");
  delay(3000);
  lcd.clear();
}

void timeSetup()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void tempSensorSetup()
{
  while (NO_ERR != sensor.begin())
  {
    Serial.println("Temp sensor connect failed.");
    delay(3000);
  }

  sensor.enterSleepMode();
  delay(50);
  sensor.enterSleepMode(false);
  delay(200);
}

void bumperSetup()
{
  pinMode(bumperPin, OUTPUT);
  digitalWrite(bumperPin, LOW);
}

void lcdDetectedPrint(float temp)
{
  lcd.clear ();
  lcd.setCursor(0, 1);
  lcd.print(temp);
}

void lcdUndetectedPrint()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("=== All clear");
}

void localTime()
{
  time_t rawTime;
  struct tm *info;

  time(&rawTime);

  info = localtime(&rawTime);

  strftime(timeResult, sizeof(timeResult), "%Y-%m-%d %H:%M", info);
}

void lcdTimePrint()
{
  lcd.setCursor(0, 0);
  lcd.print(timeResult);
}

void bumperWork()
{
  digitalWrite(bumperPin, HIGH);
  delay(bumperDelay);
  digitalWrite(bumperPin, LOW);
}

void tempSensorGet()
{
  objectTemp = sensor.getObjectTempCelsius();
}