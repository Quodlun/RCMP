void setup ()
{
  Serial.begin ( 115200 );

  int i = 1;
  char j [ 5 ];
  sprintf ( j, "j: %d", i );
  Serial.print ( j );
  }

void loop ()
{
  delay ( 1000 );
}