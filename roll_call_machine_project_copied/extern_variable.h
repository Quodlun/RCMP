#ifndef EXTERN_VARIABLE_H
#define EXTERN_VARIABLE_H

/*
    Bumper:
        3.3V Power  *1
        GND         *1
        Output Pin  *1
*/
extern const int bumperPin = 13;
extern const int bumperDelay = 500;

/*
    IR Sensor
        3.3V VCC    *1    Brown
        GND         *1    Blue
        Data        *1    Black
*/
extern const int irSensorPin = 18;

// Old temperature sensor setting, use as spare sensor.
extern const int tempSensorPin = 4;

#define sda_pin 21
#define scl_pin 22

#define MLX90614_I2C_ADDR 0x5A
#define LCD_I2C_ADDR 0x27

extern const char* LINE_TOKEN = "MbcvZH9nf1mvsSkXBzEpoQDQNOaNkWpHJ2nmoSBSBpz";
extern const char* ssid = "When Can My Internet Get Better";
extern const char* password = "O00O00O0";

extern const char* ntpServer = "time.google.com";
extern const long gmtOffset_sec = 28800;
extern const int daylightOffset_sec = 0;

#endif