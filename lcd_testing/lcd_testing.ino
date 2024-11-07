#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd ( 0x27, 16, 2 );

void setup ()
{
  Serial.begin ( 115200 );

  lcd.init ();
  lcd.backlight ();
  lcd.clear ();
  lcd.setCursor ( 0, 0 );

  lcd.print ( "LCD Ready" );
  delay ( 3000 );
  lcd.clear ();
}

void loop ()
{
  lcd.setCursor ( 0, 0 );
  lcd.print ( "0123456789ABCDEF");
  lcd.setCursor ( 0, 1 );
  lcd.print ( "0123456789ABCDEF");
  delay ( 5000 );

  lcd.clear ();
  delay ( 1000 );
}
