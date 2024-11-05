#include <LiquidCrystal_I2C.h>
#include <DFRobot_MLX90614.h>

#define sda_pin 21
#define scl_pin 22
#define MLX90614_I2C_ADDR 0x5A
#define LCD_I2C_ADDR 0x27

   // mlx9614 default I2C communication address
DFRobot_MLX90614_I2C sensor(MLX90614_I2C_ADDR, &Wire);   // instantiate an object to drive the sensor
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);

void setup()
{
  Serial.begin(115200);
  lcdSetup ();

  // initialize the sensor
  while( NO_ERR != sensor.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  sensor.setEmissivityCorrectionCoefficient(1.0);
  sensor.setI2CAddress(0x5A);
  sensor.setMeasuredParameters(sensor.eIIR100, sensor.eFIR1024);

  sensor.enterSleepMode();
  delay(50);
  sensor.enterSleepMode(false);
  delay(200);
}

void loop()
{
  float ambientTemp = sensor.getAmbientTempCelsius();
  float objectTemp = sensor.getObjectTempCelsius();

  lcd.clear ();
  lcd.setCursor ( 0, 0 );

  Serial.print("Object celsius : ");  Serial.print(objectTemp);  Serial.println(" °C");
  
  lcd.print ( objectTemp );

  delay(3000);
}

void lcdSetup()
{
  lcd.init();
  lcd.backlight();

  lcd.print("LCD Ready");
  delay(3000);
  lcd.clear();
}