#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobot_MLX90614.h>
#include <HardwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <Discord_WebHook.h>
#include <time.h>
#include "ExternVariable.h"
#include "PinMap.h"

/// @brief Class 創建
HardwareSerial mySerial(1); // 使用 ESP32 的第二個串口（UART1）
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd ( LCD_I2C_ADDR, 16, 2 );
DFRobot_MLX90614_I2C sensor ( MLX90614_I2C_ADDR , &Wire );

void setup ()
{
  Serial.begin(115200);
  discordWebHookSetup ();
  fingerprintSetup();  // 初始化指紋傳感器
  Serial.println("System Ready");
  sensor.begin();
  pinMode ( irSensorPin, INPUT );
  bumperSetup ();
  lcdSetup ();
  timeSetup ()
  bumperWork ();
}

void loop ()
{
  lcd.clear();

  int fingerprintID = getFingerprintID();
  if (fingerprintID >= 0)
  {
    Serial.print("識別到指紋，ID: ");
    Serial.println(fingerprintID);

    localTime ();

    lcd.setCursor(0, 0);
    lcd.print("Waiting for IR");

    // 等待IR传感器检测到目标
    while (digitalRead(irSensorPin) != LOW) {
      delay(100); // 每100ms检查一次，避免频繁轮询
    }

    sprintf(tempResult, "%4.2f", sensor.getObjectTempCelsius());

    // 检测到IR信号后执行操作
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detected");
    lcd.setCursor(0, 1);
    lcd.print(tempResult);

    discord.send ( timeResult );
    discord.send ( tempResult );

    delay(2000); // 延迟2秒等待 bumper 检测
    bumperWorked = false;
    while (!bumperWorked) {
    bumperWork();
  }
    delay(1000); // 每秒检查一次
  } else {
    Serial.println("No Match Found");
  }

  delay(500);

}

/// @brief LCD 初始化
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

/// @brief 泵浦初始化
void bumperSetup ()
{
  pinMode ( bumperPin, OUTPUT );
  digitalWrite ( bumperPin, HIGH );
  delay ( 500 );
}

/// @brief 指紋傳感器初始化
void fingerprintSetup() {
  mySerial.begin(57600, SERIAL_8N1, 18, 19); // 使用 GPIO 18 (TX) 和 19 (RX)
  
  if (finger.verifyPassword()) {
    Serial.println("指紋傳感器連接成功！");
  } else {
    Serial.println("無法找到指紋傳感器，請檢查接線和連接。");
    while (1) { delay(1); }
  }

  finger.getTemplateCount(); // 讀取指紋模板數量
  Serial.print("已存儲的指紋數量："); 
  Serial.println(finger.templateCount);
}

/// @brief Discord WebHook 初始化
void discordWebHookSetup ()
{
  discord.begin ( DISCORD_WEBHOOK );
  discord.disableDebug ();
  discord.addWiFi ( ssid, password );
  discord.connectWiFi ();

  bool message_sent = discord.send ( "Discord WebHook Confirm" );

  if ( message_sent )
  {
    Serial.println("Message sent");
  }
  
  else
  {
    Serial.println("I AM ERROR");
  }
}

/// @brief NTP 初始化
void timeSetup ()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

/// @brief 泵浦運作
void bumperWork ()
{
  digitalWrite ( bumperPin, LOW );
  delay ( bumperDelay );
  digitalWrite ( bumperPin, HIGH );

  bumperWorked = true;
}

/// @brief 獲取指紋 ID 的副程式，包含狀態檢查和匹配
int getFingerprintID() {
  uint8_t p = finger.getImage();

  // 檢查是否檢測到手指
  if (p == FINGERPRINT_NOFINGER) {  // 正確的指紋狀態常量
    Serial.println("未檢測到手指");
    return -1;
  } else if (p == FINGERPRINT_OK) {  // 成功狀態
    Serial.println("指紋檢測成功");
  } else {
    Serial.println("指紋檢測失敗"); 
    return -1;
  }

  // 搜尋指紋匹配
  p = finger.fingerFastSearch();  // 使用快速搜尋函數
  if (p == FINGERPRINT_OK) {  // 匹配成功
    Serial.print("找到匹配的指紋，ID #"); 
    Serial.println(finger.fingerID);
    return finger.fingerID;
  } else {
    Serial.println("未找到匹配的指紋");
    return -1;
  }
}

/// @brief NTP 讀取時間
void localTime()
{
  time_t rawTime;
  struct tm *info;

  time(&rawTime);

  info = localtime(&rawTime);

  strftime(timeResult, sizeof(timeResult), "%Y-%m-%d %H:%M", info);
}