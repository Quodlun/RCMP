#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include "extern_variable.h"

//接腳與Token定義
#define irSensorPin 16
#define tempSensorPin 4

//RTC時間設定
const char* ntpServer = "time.google.com";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 0;

//溫度setup
OneWire oneWire ( tempSensorPin );
DallasTemperature sensors ( &oneWire );

//LCD Setup
LiquidCrystal_I2C lcd ( 0x27, 16, 2 );

void setup ()
{
  //距離setup
  Serial.begin ( 115200 );

  pinMode ( irSensorPin, INPUT_PULLUP );
  sensors.begin ();

  //呼叫設定副程式
  wifiSetup ();
  lineSetup ();
  lcdSetup ();
  timeSetup ();
}

void loop ()
{
    printLocalTime ();
    
    //IR讀取數值變數
    int L = digitalRead ( irSensorPin );

    //偵測到物件
    if ( L == 0 )
    {
        //溫度感測器數值請求
        sensors.requestTemperatures ();

        LINE.notify ( "Obstacle detected" );
        LINE.notify ( sensors.getTempCByIndex ( 0 ) ); //轉換攝氏度並輸出
        printLocalTime ();

        lcdDetectedPrint ( sensors.getTempCByIndex ( 0 ) );
    
    }
    
    else
    {
        LINE.notify ( "=== All clear" );

        lcdUndetectedPrint ();
    }

    delay ( 1000 );
}

//網路設定
void wifiSetup ()
{
    WiFi.mode ( WIFI_STA );
    WiFi.begin ( ssid, password );

    while ( WiFi.status () != WL_CONNECTED )
    {
      delay ( 500 );
    }
    /*
    WiFi debug:
    Serial.println ( "" );
    Serial.print ( "Connected to " );
    Serial.println ( ssid );
    Serial.print ( "IP address: " );
    Serial.println ( WiFi.localIP () );
    */
}

//Line Notify設定 
void lineSetup ()
{
    LINE.setToken ( LINE_TOKEN );
    LINE.notify ( "Line Notify Link Confirm." );
}

//LCD I2C設定
void lcdSetup ()
{
    lcd.init ();
    lcd.backlight ();

    lcd.print ( "LCD Ready" );
    delay ( 3000 );
    lcd.clear ();
}

void timeSetup ()
{
    configTime ( gmtOffset_sec, daylightOffset_sec, ntpServer );
}

//LCD輸出(偵測到物件:是)
void lcdDetectedPrint ( float temp )
{
    lcd.clear ();
    lcd.setCursor ( 0, 0 );
    lcd.print ( "Detected" );

    lcd.setCursor ( 0, 1 );
    lcd.print ( temp );
}

//LCD輸出(偵測到物件:否)
void lcdUndetectedPrint ()
{
    lcd.clear ();
    lcd.setCursor ( 0, 0 );
    lcd.print ( "=== All clear" );
}

void printLocalTime ()
{
    time_t rawtime;
    struct tm *info;
    char buffer [ 80 ];
 
    time( &rawtime );
 
    info = localtime ( &rawtime );
 
    strftime ( buffer, 80, "%Y-%m-%d %H:%M:%S", info );
    LINE.notify ( buffer );
}