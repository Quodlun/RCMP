//注意：此程式只適用於一台Arduino連接一個DS18B20的狀況
#include <OneWire.h> 
#include <DallasTemperature.h> 

#define DQ_Pin 2  

OneWire oneWire ( DQ_Pin );
DallasTemperature sensors ( &oneWire );

void setup ()
{
  Serial.begin ( 115200 );
  sensors.begin ();
}

void loop ()
{
  Serial.print ( "Temperatures --> " );
  sensors.requestTemperatures ();
  Serial.println ( sensors.getTempCByIndex ( 0 ) );
  delay ( 100 );
}
