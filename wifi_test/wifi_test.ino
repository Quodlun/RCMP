#include <WiFi.h>
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
}

void loop ()
{
  delay ( 1000 );
}
