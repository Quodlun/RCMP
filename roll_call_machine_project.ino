#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h> 
#include <DallasTemperature.h> 

#define tempSensorPin 3
#define irSensorPin 2
#define LINE_TOKEN "Y3nL5gLv1Q7UiDshiv2rPZAXc4jbouEqzt04HmilnZo"

const char* ssid     = "FuNet-UAP";
const char* password = "25785657";

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