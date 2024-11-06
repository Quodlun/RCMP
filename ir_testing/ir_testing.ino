int irSensorPin = 12;

void setup ()
{
  Serial.begin( 115200 );
  pinMode ( irSensorPin, INPUT );
}

void loop ()
{
  if ( digitalRead ( irSensorPin ) == LOW )
  {
    Serial.println ( "Object Detected." );
  }
  else
  {
    Serial.println ( "IR Sensor Clear" );
  }

  delay ( 500 );
}

