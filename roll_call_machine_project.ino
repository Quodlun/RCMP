#include <OneWire.h> 
#include <DallasTemperature.h> 

#define tempSensorPin 3
#define irSensorPin 2

OneWire oneWire ( tempSensorPin );
DallasTemperature sensors ( &oneWire );

void setup ()
{
  Serial.begin ( 115200 );

  pinMode ( irSensorPin, INPUT_PULLUP );
  sensors.begin ();
}

void loop ()
{
    int L = digitalRead ( irSensorPin );
  
    if ( L == 0 )
    {
        Serial.print ( " Obstacle detected    " );
        sensors.requestTemperatures ();
        Serial.println ( sensors.getTempCByIndex ( 0 ) );
    
    }
    
    else
    {
        Serial.println ( "  === All clear" );
    }

    delay(100);
}