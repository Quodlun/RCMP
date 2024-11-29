#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include "ExternVariable.h"

void setup ()
{
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  
  while ( WiFi.status () != WL_CONNECTED )
  {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.print ( "\n IP: " );
  Serial.println ( WiFi.localIP () );

  Serial.println ( "WiFi status:" );
  WiFi.printDiag ( Serial );

  lineNotifySetup ()
}

void loop ()
{
  delay ( 1000 );
}

void lineNotifySetup ()
{
  Serial.println ( LINE.getVersion () );
  LINE.setToken ( LINE_TOKEN );
  LINE.notify ( "Line Notify Checked." );
}