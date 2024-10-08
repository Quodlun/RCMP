#ifndef EXTERN_VARIABLE_H
#define EXTERN_VARIABLE_H

/*
    Bumper:
        5V Power    *1
        3.3V Power  *1
        GND         *2
        Output Pin  *1    13
*/
extern const int bumperPin = 13;
extern const int bumperDelay = 500;

extern const int irSensorPin = 16;
extern const int tempSensorPin = 4;

extern const char* LINE_TOKEN = "MbcvZH9nf1mvsSkXBzEpoQDQNOaNkWpHJ2nmoSBSBpz";
extern const char* ssid = "When Can My Internet Get Better";
extern const char* password = "O00O00O0";

extern const char* ntpServer = "time.google.com";
extern const long gmtOffset_sec = 28800;
extern const int daylightOffset_sec = 0;

#endif