#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h> 
#include <DallasTemperature.h> 

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

void setup ()
{
  //距離setup
  Serial.begin ( 115200 );

  pinMode ( irSensorPin, INPUT_PULLUP );
  sensors.begin ();

  //呼叫設定副程式
  wifi_setup ();
  line_setup ();
}

void loop ()
{
    //IR讀取數值變數
    int L = digitalRead ( irSensorPin );

    //偵測到物件
    if ( L == 0 )
    {
        LINE.notify ( "Obstacle detected" );
        sensors.requestTemperatures (); //IR數值請求
        LINE.notify ( sensors.getTempCByIndex ( 0 ) ); //轉換攝氏度並輸出
    
    }
    
    else
    {
        LINE.notify ( "=== All clear" );
    }

    delay(100);
}

//網路設定
void wifi_setup ()
{
    WiFi.mode ( WIFI_STA );
    WiFi.begin ( ssid, password );

    while ( WiFi.status () != WL_CONNECTED )
    {
      Serial.print ( "." );
      delay ( 500 );
    }

    Serial.println ( "" );
    Serial.print ( "Connected to " );
    Serial.println ( ssid );
    Serial.print ( "IP address: " );
    Serial.println ( WiFi.localIP () );
}

//Line Notify設定
void line_setup ()
{
    LINE.setToken ( LINE_TOKEN );
    LINE.notify ( "Line Notify Link Confirm." );
}