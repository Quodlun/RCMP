#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobot_MLX90614.h>

#include "ExternVariable.h"
#include "PinMap.h"

LiquidCrystal_I2C lcd ( LCD_I2C_ADDR, 16, 2 );
DFRobot_MLX90614_I2C sensor( MLX90614_I2C_ADDR , &Wire );

int objectTemp = 0;
bool bumperWorked = false;

void setup ()
{
  Serial.begin ( 115200 );

  sensor.begin();
  pinMode ( irSensorPin, INPUT );

  bumperSetup ();
  lcdSetup ();

  bumperWork ();
}

void loop ()
{
  objectTemp = sensor.getObjectTempCelsius();
  lcd.clear ();

  if ( digitalRead ( irSensorPin ) == LOW )
  {
    lcd.setCursor ( 0, 0 );
    lcd.print ( objectTemp );

    if ( !bumperWorked )
    {
      bumperWork ();
    }
  }

  else
  {
    
    lcd.setCursor ( 0, 0 );
    lcd.print ( "Undetected" );

    bumperWorked = false;
  }
  
  delay ( 500 );
}

void lcdSetup ()
{
  lcd.init ();
  lcd.backlight ();
  lcd.clear ();
  lcd.setCursor ( 0, 0 );

  lcd.print ( "LCD Ready" );
  delay ( 3000 );
  lcd.clear ();
}

void bumperSetup ()
{
  pinMode ( bumperPin, OUTPUT );
  digitalWrite ( bumperPin, HIGH );
  delay ( 500 );
}

void bumperWork ()
{
  digitalWrite ( bumperPin, LOW );
  delay ( bumperDelay );
  digitalWrite ( bumperPin, HIGH );

  bumperWorked = true;
}
