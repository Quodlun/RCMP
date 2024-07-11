#include "OneWire.h"
#include "DallasTemperature.h"

#define ONE_WIRE_BUS 2

int IR_SENSOR = 0;
int intSensorResult = 0;
float fltSensorCalc = 0;
float tempC = 0;

OneWire oneWire ( ONE_WIRE_BUS );

DallasTemperature sensors ( &oneWire );

void setup ()
{
  Serial.begin ( 115200 );
  sensors.begin ();
}

void loop ()
{
    
    ir_sensor ();

    if ( fltSensorCalc <= 10 )
    {
        ir_print ();

        temp_sensor ();

        temp_print ();
    }

    else
    {
        ir_print ();
    }

    delay ( 100 );
}

void ir_sensor ()
{
    intSensorResult = analogRead ( IR_SENSOR ); //取得感測器值
    fltSensorCalc = ( 6787.0 / ( intSensorResult - 3.0 ) ) - 4.0; //Calculate distance in cm
}

void temp_sensor ()
{
    sensors.requestTemperatures ();
    tempC = sensors.getTempCByIndex ( 0 );
}

void ir_print ()
{
    Serial.print ( fltSensorCalc );
    Serial.print ( " cm" );
    Serial.print ( "    " );
}

void temp_print ()
{
    Serial.print ( tempC );
    Serial.print ( " \xC2\xB0" );
    Serial.print ( "C" );
}