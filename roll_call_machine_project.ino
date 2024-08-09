#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h> 
#include <DallasTemperature.h> 

#define irSensorPin 2
#define tempSensorPin 4
#define LINE_TOKEN "Y3nL5gLv1Q7UiDshiv2rPZAXc4jbouEqzt04HmilnZo"

const char* ssid     = "When Can My Internet Get Better";
const char* password = "O00O00O0";

OneWire oneWire ( tempSensorPin );
DallasTemperature sensors ( &oneWire );

void setup ()
{
  Serial.begin ( 115200 );

  pinMode ( irSensorPin, INPUT_PULLUP );
  sensors.begin ();

  wifi_setup ();
  line_setup ();
}

void loop ()
{
    int L = digitalRead ( irSensorPin );
  
    if ( L == 0 )
    {
        LINE.notify ( "Obstacle detected    " );
        sensors.requestTemperatures ();
        Serial.println ( sensors.getTempCByIndex ( 0 ) );
    
    }
    
    else
    {
        LINE.notify ( "=== All clear" );
    }

    delay(100);
}

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

void line_setup ()
{
    LINE.setToken ( LINE_TOKEN );
    LINE.notify ( "Line Notify Link Confirm." );
}

/*
Sketch 使用 1008329 位元組（76%）的程式儲存空間。最大為 1310720 位元組
全域變數使用 45364 位元組 (13%) 的動態記憶體, 保留 282316 位元組給區域變數. 最大 327680 位元組
esptool.py v4.6
Serial port COM8
Connecting.........Traceback (most recent call last):
  File "esptool.py", line 37, in <module>
  File "esptool\__init__.py", line 1064, in _main
  File "esptool\__init__.py", line 859, in main
  File "esptool\cmds.py", line 466, in write_flash
  File "esptool\util.py", line 37, in flash_size_bytes
TypeError: argument of type 'NoneType' is not iterable

Chip is ESP32-D0WD-V3 (revision v3.1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: ec:64:c9:5d:60:60
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 921600
Changed.
WARNING: Failed to communicate with the flash chip, read/write operations will fail. Try checking the chip connections or removing any other hardware connected to IOs.
Configuring flash size...
[10508] Failed to execute script 'esptool' due to unhandled exception!
上傳失敗: 上傳錯誤: exit status 1
*/