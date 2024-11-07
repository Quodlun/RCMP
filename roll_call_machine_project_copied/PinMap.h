#ifndef PINMAP_H
#define PINMAP_H

/*
    Bumper:
        3.3V Power  *1
        GND         *1
        Output Pin  *1
*/
#define bumperPin 17

/*
    IR Sensor
        5V VCC      *1    Brown
        GND         *1    Blue
        Data        *1    Black
*/
#define irSensorPin 16

// Old temperature sensor setting, use as spare sensor.
#define tempSensorPin 4

#define sda_pin 21
#define scl_pin 22

// I2C Address
#define MLX90614_I2C_ADDR 0x5A
#define LCD_I2C_ADDR 0x27

#endif