#include <WiFi.h>
#include <time.h>
 
const char* ssid     = "When Can My Internet Get Better";
const char* password = "O00O00O0";
 
const char* ntpServer = "time.google.com";
const long  gmtOffset_sec = 28800; //台灣時區+8hr,28800=8*60*60
const int   daylightOffset_sec = 0;  //台灣無日光節約時間
 
void setup(){
  Serial.begin(115200);
 
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
 
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
 
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
 
void loop(){
  delay(5000);
  printLocalTime();
}
 
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %Y %m %d %H:%M:%S"); //%A-Sunday,%Y-2022,%m-3,%d-27,%H:%M:%S-21:10:02
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S"); //%A-Sunday,%B-March,%d-27,%Y-2022,%H:%M:%S-21:10:02
  Serial.print("Day of week: "); //顯示英文 星期
  Serial.println(&timeinfo, "%A"); //Day of week: Sunday
  Serial.print("Month: "); //顯示英文 月份
  Serial.println(&timeinfo, "%B"); //Month: March
  Serial.print("Day of Month: "); //顯示英文 日
  Serial.println(&timeinfo, "%d"); //Day of Month: 27
  Serial.print("Year: "); //顯示英文 西元年
  Serial.println(&timeinfo, "%Y"); //Year: 2022
  Serial.print("Hour: "); //顯示英文 時 24小時制
  Serial.println(&timeinfo, "%H"); //Hour: 21
  Serial.print("Hour (12 hour format): "); //顯示英文 時 12小時制
  Serial.println(&timeinfo, "%I"); //Hour (12 hour format): 9
  Serial.print("Minute: "); //顯示英文 分
  Serial.println(&timeinfo, "%M"); //Minute: 01
  Serial.print("Second: "); //顯示英文 秒
  Serial.println(&timeinfo, "%S"); //Second: 24
 
  Serial.println("Time variables"); //時間變數Time variables
  char timeHour[3]; //宣告時間小時變數：timeHour
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour); //顯示時數：22
  char timeWeekDay[10]; //宣告時間星期變數：timeWeekDay
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay); //顯示星期：Sunday
  Serial.println("*****************************************************");
 
}

/*
#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//接腳與Token定義
#define irSensorPin 2
#define tempSensorPin 4
#define LINE_TOKEN "Y3nL5gLv1Q7UiDshiv2rPZAXc4jbouEqzt04HmilnZo"

//網路連線內容
const char* ssid     = "When Can My Internet Get Better";
const char* password = "O00O00O0";

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
}

void loop ()
{
    //IR讀取數值變數
    int L = digitalRead ( irSensorPin );

    //偵測到物件
    if ( L == 0 )
    {
        //溫度感測器數值請求
        sensors.requestTemperatures ();

        LINE.notify ( "Obstacle detected" );
        LINE.notify ( sensors.getTempCByIndex ( 0 ) ); //轉換攝氏度並輸出

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
*/