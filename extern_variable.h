#ifndef EXTERN_VARIABLE_H
#define EXTERN_VARIABLE_H

// 宣告變數，去掉不必要的指標符號，並在 .h 文件中不初始化
extern const int irSensorPin;
extern const int tempSensorPin;

extern const char* LINE_TOKEN;
extern const char* ssid;
extern const char* password;

extern const char* ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;

#endif