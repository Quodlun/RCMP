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
  lcd.clear();

  int L = digitalRead(irSensorPin);

  if (L == 0)
  {
    tempSensorGet();

    LINE.notify("Obstacle detected");
    LINE.notify(objectTemp);
    lcdDetectedPrint(objectTemp);

    delay(500);
    localTime();
    lcdTimePrint();
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Detected");

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
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print(timeResult);
  lcd.setCursor(0, 1);
}

int getFingerprintID()
{
  uint8_t p = finger.getImage();

  if (p == FINGERPRINT_NOFINGER)
  {
    Serial.println("未檢測到手指");
    return -1;
  }

  else if (p == FINGERPRINT_OK)
  {
    Serial.println("指紋檢測成功");
  }

  else
  {
    Serial.println("指紋檢測失敗");

    return -1;
  }

  p = finger.fingerSearch();

  if (p == FINGERPRINT_OK)
  {
    Serial.print("找到匹配的指紋，ID #");
    Serial.println(finger.fingerID);

    return finger.fingerID;
  }

  else
  {
    Serial.println("未找到匹配的指紋");
    LINE.notify("警告!有未知人士正在使用點名器");

    return -1;
  }
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