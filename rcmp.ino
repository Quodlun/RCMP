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

/// @section Class Setup & Class Array Create
struct ClassInfo
{
  int seatNumber;
  String name;

  ClassInfo(int seat, String studentName) : seatNumber(seat), name(studentName) {}
};

ClassInfo classArray[] =
    {
        ClassInfo(8, "呂吉堂"),
        ClassInfo(9, "曹銘洲"),
        ClassInfo(11, "傅威禮"),
        ClassInfo(12, "李佳諺")};

/// @section Library Class Create
Discord_Webhook discord;
HardwareSerial mySerial(1); /// @brief 使用 ESP32 的第二個串口（UART1）
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, 16, 2);
DFRobot_MLX90614_I2C sensor(MLX90614_I2C_ADDR, &Wire);

/// @section Variable Create
char tempResult[7];
char timeResult[17];
char seatNumberResult[7];
char nameResult[8];

/// @section Setup
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

  /// @brief Debugger
  debug();
}

/// @section Main Code
void loop()
{
  lcd.clear();

  /// @brief Fingerprint Sensor Work
  int fingerprintID = getFingerprintID();

  if (fingerprintID >= 0)
  {
    Serial.print("識別到指紋，ID: ");
    Serial.println(fingerprintID);

    lcd.setCursor(0, 0);

    /// @brief Get Current Time
    localTime();

    /// @brief Wait For IR Signal
    while (digitalRead(irSensorPin) != LOW)
    {
      delay(100);
    }

    /// @brief Other Component Work
    functionAfterIR();

    delay(1000);
  }

  else
  {
    Serial.println("No Match Found");
  }

  delay(500);
}

/// @section Debugger
void debug(int debugCode)
{
  switch (debugCode)
  {
  /// @brief Serial
  case 1:
    Serial.print("Serial Test");

    break;

  /// @brief Bumper
  case 2:
    Serial.print(bumperDelay);

    while (true)
    {
      digitalWrite(bumperPin, LOW);
      delay(bumperDelay);
      digitalWrite(bumperPin, HIGH);
    }

    break;

  /// @brief LCD
  case 3:
    while (true)
    {
      lcd.setCursor(0, 0);
      lcd.print("1234567890ABCDEF");
      lcd.setCursor(0, 1);
      lcd.print("1234567890ABCDEF");

      delay(3000);
      lcd.clear();
    }

    break;

  /// @brief Discord WebHook
  case 4:
    discord.send("Discord WebHook Test");

    break;

  /// @brief IR Sensor
  case 5:
    while (true)
    {
      if (digitalRead(irSensorPin) == LOW)
      {
        Serial.println("IR Detected");
      }

      else
      {
        Serial.println("IR Clear");
      }
    }

    break;

  /// @brief NTP
  case 6:
    while (true)
    {
      localTime();
      Serial.println(timeResult);
    }

    break;

  /// @brief Temperature Sensor
  case 7:
    while (true)
    {
      sprintf(tempResult, "%4.2f", sensor.getObjectTempCelsius());

      Serial.println(tempResult);
    }

    break;

  default:
    break;
  }
}

/// @section Parts Setup Subroutines

/// @subsection LCD I2C Setup
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

/// @subsection Bumper Setup
void bumperSetup()
{
  pinMode(bumperPin, OUTPUT);
  digitalWrite(bumperPin, HIGH);
  delay(500);
}

/// @subsection Fingerprtint Sensor Setup
void fingerprintSetup()
{
  mySerial.begin(57600, SERIAL_8N1, 18, 19); // 使用 GPIO 18 (TX) 和 19 (RX)

  if (finger.verifyPassword())
  {
    Serial.println("指紋傳感器連接成功！");
  }
  else
  {
    Serial.println("無法找到指紋傳感器，請檢查接線和連接。");
  }
}

/// @subsection Discord WebHook Setup
void discordWebHookSetup()
{
  discord.begin(discordWebhook);
  discord.disableDebug();
  discord.addWiFi(ssid, password);
  discord.connectWiFi();

  bool message_sent = discord.send("Discord WebHook Confirm");
}

/// @subsection NTP Setup
void timeSetup()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

/// @section Parts Function Subroutines

/// @subsection Bumper Work
void bumperWork()
{
  digitalWrite(bumperPin, LOW);
  delay(bumperDelay);
  digitalWrite(bumperPin, HIGH);

  bumperWorked = true;
}

/// @subsection Fingerprint Work
int getFingerprintID()
{
  uint8_t p = finger.getImage();

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

    sprintf(seatNumberResult, "座號: %d", info.seatNumber);
    Serial.print(seatNumberResult);
    discord.send(seatNumberResult);

    sprintf(nameResult, "姓名: %s", info.name);
    Serial.println(nameResult);
    discord.send(nameResult); // 顯示學生姓名

    return finger.fingerID;
  }
  else
  {
    Serial.println("未找到匹配的指紋");
    return -1;
  }
}

/// @subsection NTP Get Time
void localTime()
{
  time_t rawTime;
  struct tm *info;

  time(&rawTime);

  info = localtime(&rawTime);

  strftime(timeResult, sizeof(timeResult), "%Y-%m-%d %H:%M", info);
}

/// @subsection Function After IR Sensor Detected
void functionAfterIR()
{
  /// @brief Make Temperature Result A F-String
  sprintf(tempResult, "%4.2f", sensor.getObjectTempCelsius());

  /// @brief LCD Print Temperature Result
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("體溫");
  lcd.setCursor(0, 1);
  lcd.print(tempResult);

  /// @brief Discord Send Time & Temperature Reesult
  discord.send(timeResult);
  discord.send(tempResult);

  delay(50);

  /// @brief Make Sure Bumper Will Work Only One Time Per Detect
  bumperWorked = false;

  while (!bumperWorked)
  {
    bumperWork();
  }
}