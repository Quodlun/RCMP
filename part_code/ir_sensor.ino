#define SENSOR 2

void setup ()
{
  Serial.begin ( 115200 );
  pinMode ( SENSOR, INPUT_PULLUP );
}

void loop ()
{
    int L = digitalRead ( SENSOR );
  
    if ( L == 0 )
    {
        Serial.println ( " Obstacle detected" );
    
    }
    
    else
    {
        Serial.println ( "  === All clear" );
    }

    delay(100);
}
