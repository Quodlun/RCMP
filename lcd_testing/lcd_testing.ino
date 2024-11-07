#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobot_MLX90614.h> 

LiquidCrystal_I2C lcd ( 0x27, 16, 2 );
DFRobot_MLX90614_I2C sensor(0x5A, &Wire);

void setup ()
{
  Serial.begin ( 115200 );

  sensor.begin();

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
  int objectTemp = sensor.getObjectTempCelsius();

  lcd.setCursor ( 0, 0 );
  lcd.print ( objectTemp );
  delay ( 1000 );
}
