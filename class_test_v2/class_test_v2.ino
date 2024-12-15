#include "classes.h"

void setup ()
{
  Serial.begin ( 115200 );

  testClass num11 ( 11, "傅威禮" );
  Serial.println ( num11.num1 );
  Serial.println ( num11.string1 );
}

void loop ()
{
  delay ( 1000 );
}