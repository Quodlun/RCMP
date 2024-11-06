#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <Adafruit_Fingerprint.h>
#include <DFRobot_MLX90614.h>
#include "ExternVariable.h"
#include "PinMap.h"

DFRobot_MLX90614_I2C sensor(MLX90614_I2C_ADDR, &Wire);

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger(&mySerial);

char timeResult[20];
float objectTemp;

void setup()
{
  Serial.begin(115200);

  pinMode(irSensorPin, INPUT);
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
  /*
    Fill the code of the thing you want to debug, blank or 1 if you don't want any.
    1: Wi-Fi
    2: IR Sensor
    3: Line Notify
    4: LCD
    5: Bumper
    6: Temperature Sensor
  */
  debug ( 3 );

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
  int offline_count = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    offline_count ++;
    Serial.print ( "Debug: WiFi Setup Failed (" ); Serial.print ( offline_count ); Serial.println ( ")" );
    delay(500);
  }

  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP () );
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

// Debug Code
void debug ( int debug_code )
{
  switch ( debug_code )
  {
    case 1:
      wifiDebug ();
      break;

    case 2:
      irSensorDebug ();
      break;

    case 3:
      lineNotifyDebug ();
      break;

    case 4:
      lcdDebug ();
      break;

    case 5:
      bumperDebug ();
      break;

    case 6:
      tempSensorDebug ();
      break;
  
    default:
      wifiDebug ();
      break;
  }
}
void irSensorDebug ()
{
  while ( true )
  {
    if ( digitalRead ( irSensorPin ) == LOW )
    {
      Serial.println ( "Object Detected." );
    }

    else
    {
      Serial.println ( "IR Sensor Clear" );
    }

    delay ( 500 );
  }
  
}

void lineNotifyDebug ()
{
  LINE.notify ( "Debug: Line Notify Okay." );

  delay ( 1000 );
}

void wifiDebug ()
{
  int dropped_count = 0;

  while (WiFi.status() != WL_CONNECTED)
  {
    dropped_count ++;
    Serial.print ( "Debug: WiFi Dropped (" ); Serial.print ( dropped_count ); Serial.println ( ")" );

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    delay(500);
  }

  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP () );
}

void lcdDebug ()
{
  lcd.clear ();

  for ( int i = 0; i < 3; i ++ );
  {
    lcd.setCursor ( 0, 0 );
    lcd.print ( "LCD Debug Mode" );
    delay ( 1000 );
    
    lcd.clear ();
    delay ( 1000 );
  }

  while ( true )
  {
    lcd.setCursor ( 0, 0 );
    lcd.print ( "0123456789abcdef" );
    lcd.setCursor ( 0, 1 );
    lcd.print ( "0123456789abcdef" );
    delay ( 5000 );
    
    lcd.clear ();
    delay ( 1000 );
  }
}

void bumperDebug ()
{
  digitalWrite ( bumperPin, HIGH );
  delay ( 5000 );

  digitalWrite ( bumperPin, LOW );
  delay ( 5000 );
}

void tempSensorDebug ()
{
  float ambientTemp = sensor.getAmbientTempCelsius();
  float objectTemp = sensor.getObjectTempCelsius();

  Serial.print("Ambient celsius : "); Serial.print(ambientTemp); Serial.println(" °C");
  Serial.print("Object celsius : ");  Serial.print(objectTemp);  Serial.println(" °C");

  delay ( 1000 );
}