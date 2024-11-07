#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobot_MLX90614.h>

#include "ExternVariable.h"
#include "PinMap.h"

LiquidCrystal_I2C lcd ( LCD_I2C_ADDR, 16, 2 );
DFRobot_MLX90614_I2C sensor( MLX90614_I2C_ADDR , &Wire );

int objectTemp = 0;

void setup ()
{
  Serial.begin ( 115200 );

  sensor.begin();
  pinMode(irSensorPin, INPUT);

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
  objectTemp = sensor.getObjectTempCelsius();
  lcd.clear ();

  if ( digitalRead ( irSensorPin ) == LOW )
  {
    lcd.setCursor ( 0, 0 );
    lcd.print ( objectTemp );
  }

  else
  {
    lcd.setCursor ( 0, 0 );
    lcd.print ( "Undetected" );
  }
  
  delay ( 500 );
}