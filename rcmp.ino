#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobot_MLX90614.h>
#include <HardwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <Discord_WebHook.h>
#include <time.h>
#include "ExternVariable.h"
#include "PinMap.h"
#include "Settings.h"
#include "FingerPrintClass.h"

/// @section Class 創建
Discord_Webhook discord;
HardwareSerial mySerial(1); /// @brief 使用 ESP32 的第二個串口（UART1）
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);
DFRobot_MLX90614_I2C sensor(MLX90614_I2C_ADDR, &Wire);

char tempResult [ 7 ];
char timeResult [17];

void setup()
{
  Serial.begin(115200);

  sensor.begin();
  pinMode(irSensorPin, INPUT);

  discordWebHookSetup();
  timeSetup();
  fingerprintSetup();
  bumperSetup();
  lcdSetup();
  bumperWork();
}

void loop()
{
  lcd.clear();

  int fingerprintID = getFingerprintID();

  if (fingerprintID >= 0)
  {
    Serial.print("識別到指紋，ID: ");
    Serial.println(fingerprintID);

    localTime();

    lcd.setCursor(0, 0);
    lcd.print("Waiting for IR");

    // 等待IR传感器检测到目标
    while (digitalRead(irSensorPin) != LOW)
    {
      delay(100); // 每100ms检查一次，避免频繁轮询
    }

    functionAfterIR();

    delay(1000); // 每秒检查一次
  }

  else
  {
    Serial.println("No Match Found");
  }

  delay(500);
}

void classCreate()
{
  Serial.begin(115200);

  sensor.begin();
  pinMode(irSensorPin, INPUT);

  discordWebHookSetup();
  timeSetup();
  fingerprintSetup();
  bumperSetup();
  lcdSetup();
  bumperWork();
}



void lcdSetup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("LCD Ready");
  delay(3000);
  lcd.clear();
}

void bumperSetup()
{
  pinMode(bumperPin, OUTPUT);
  digitalWrite(bumperPin, HIGH);
  delay(500);
}

void bumperWork()
{
  digitalWrite(bumperPin, LOW);
  delay(bumperDelay);
  digitalWrite(bumperPin, HIGH);

  bumperWorked = true;
}

// 指紋傳感器初始化
void fingerprintSetup()
{
  Serial.begin(57600, SERIAL_8N1, 18, 19); // 使用 GPIO 18 (TX) 和 19 (RX)

  if (finger.verifyPassword())
  {
    Serial.println("指紋傳感器連接成功！");
  }
  else
  {
    Serial.println("無法找到指紋傳感器，請檢查接線和連接。");
    while (1)
    {
      delay(1);
    }
  }

  finger.getTemplateCount(); // 讀取指紋模板數量
  Serial.print("已存儲的指紋數量：");
  Serial.println(finger.templateCount);
}

/// @subsection Discord WebHook 初始化
void discordWebHookSetup()
{
  discord.begin(discordWebhook);
  discord.disableDebug();
  discord.addWiFi(ssid, password);
  discord.connectWiFi();

  bool message_sent = discord.send("Discord WebHook Confirm");

  if (message_sent)
  {
    Serial.println("Message sent");
  }

  else
  {
    Serial.println("I AM ERROR");
  }
}

/// @subsection NTP 初始化
void timeSetup()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

// 獲取指紋 ID 的副程式，包含狀態檢查和匹配
int getFingerprintID()
{
  uint8_t p = finger.getImage();

  // 檢查是否檢測到手指
  if (p == FINGERPRINT_NOFINGER)
  { // 正確的指紋狀態常量
    Serial.println("未檢測到手指");
    return -1;
  }
  else if (p == FINGERPRINT_OK)
  { // 成功狀態
    Serial.println("指紋檢測成功");
  }
  else
  {
    Serial.println("指紋檢測失敗");
    return -1;
  }

  // 搜尋指紋匹配
  p = finger.fingerFastSearch(); // 使用快速搜尋函數
  if (p == FINGERPRINT_OK)
  { // 匹配成功
    Serial.print("找到匹配的指紋");

    ClassInfo info = classArray[finger.fingerID - 1]; // 陣列從 0 開始，ID 從 1 開始

    Serial.print("座號: ");
    Serial.println(info.seatNumber);
    Serial.print("姓名: ");
    Serial.println(info.name); // 顯示學生姓名

    return finger.fingerID;
  }
  else
  {
    Serial.println("未找到匹配的指紋");
    return -1;
  }
}

/// @subsection NTP 讀取時間
void localTime()
{
  time_t rawTime;
  struct tm *info;

  time(&rawTime);

  info = localtime(&rawTime);

  strftime(timeResult, sizeof(timeResult), "%Y-%m-%d %H:%M", info);
}

/// @subsection 檢測到IR後動作
void functionAfterIR()
{
  sprintf(tempResult, "%4.2f", sensor.getObjectTempCelsius());

  /// @brief 检测到IR信号后执行操作
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Detected");
  lcd.setCursor(0, 1);
  lcd.print(tempResult);

  discord.send(timeResult);
  discord.send(tempResult);

  delay(50); /// @brief 延迟2秒等待 bumper 检测
  bumperWorked = false;

  while (!bumperWorked)
  {
    bumperWork();
  }
}