#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <Adafruit_Fingerprint.h>
#include "extern_variable.h"

//溫度setup
OneWire oneWire ( tempSensorPin );
DallasTemperature sensors ( &oneWire );

//LCD Setup
LiquidCrystal_I2C lcd ( 0x27, 16, 2 );

// 指紋模組 Setup
HardwareSerial mySerial ( 2 ); 
Adafruit_Fingerprint finger ( &mySerial );

//共用變數
char timeResult [ 20 ];

void setup ()
{
  //距離setup
  Serial.begin ( 115200 );

  //接角及感測器設定
  pinMode ( irSensorPin, INPUT_PULLUP );
  sensors.begin ();

  //呼叫設定副程式
  wifiSetup ();
  lineSetup ();
  lcdSetup ();
  timeSetup ();
}

void loop ()
{  
  //IR讀取數值變數
  int L = digitalRead ( irSensorPin );

  //偵測到物件
  if ( L == 0 )
  {
    //溫度感測器數值請求
    sensors.requestTemperatures ();

    //溫度讀取數值輸出
    LINE.notify ( "Obstacle detected" );
    LINE.notify ( sensors.getTempCByIndex ( 0 ) ); //轉換攝氏度並輸出
    lcdDetectedPrint ( sensors.getTempCByIndex ( 0 ) );

    //NTP輸出
    delay ( 500 );
    localTime ();
    lcdTimePrint ();
    LINE.notify ( timeResult );
  }
  
  else
  {
    //無偵測數值輸出
    LINE.notify ( "=== All clear" );
    lcdUndetectedPrint ();
  }

  delay ( 1000 );
}

//網路設定
void wifiSetup ()
{
  WiFi.mode ( WIFI_STA );
  WiFi.begin ( ssid, password );

  while ( WiFi.status () != WL_CONNECTED )
  {
    delay ( 500 );
  }
}

//Line Notify設定 
void lineSetup ()
{
  LINE.setToken ( LINE_TOKEN );
  LINE.notify ( "Line Notify Link Confirm." );
}

//LCD I2C設定
void lcdSetup ()
{
  lcd.init ();
  lcd.backlight ();

  lcd.print ( "LCD Ready" );
  delay ( 3000 );
  lcd.clear ();
}

//NTP 設定
void timeSetup ()
{
  configTime ( gmtOffset_sec, daylightOffset_sec, ntpServer );
}

//LCD 輸出(偵測到物件:是)
void lcdDetectedPrint ( float temp )
{
  lcd.clear ();
  lcd.setCursor ( 0, 0 );
  lcd.print ( "Detected" );

  lcd.setCursor ( 0, 1 );
  lcd.print ( temp );
}

//LCD 輸出(偵測到物件:否)
void lcdUndetectedPrint ()
{
    lcd.clear ();
    lcd.setCursor ( 0, 0 );
    lcd.print ( "=== All clear" );
}

//讀取及時時間
void localTime ()
{
  time_t rawTime;
  struct tm *info;
 
  time( &rawTime );
 
  info = localtime ( &rawTime );
 
  strftime ( timeResult, sizeof ( timeResult ), "%Y-%m-%d %H:%M", info );
}

//LCD NTP 輸出
void lcdTimePrint ()
{
  lcd.clear ();
  lcd.setCursor ( 0, 0 );
    
  lcd.print ( timeResult );
  lcd.setCursor ( 0, 1 );
}

======

void setup() {
  // Initialize Sensors
  sensors.begin();
  wifi_setup();
  line_setup();
  fingerprint_setup();  // 初始化指紋傳感器
  
  lcd.clear();
  lcd.print("System Ready");
}

void loop() {
  
  // 檢查指紋
  int fingerprintID = getFingerprintID();
  lcd.clear();
  if (fingerprintID >= 0) {
    Serial.print("識別到指紋，ID: ");
    Serial.println(fingerprintID);
    LINE.notify("Fingerprint ID: " + String(fingerprintID));

    lcd.setCursor(0, 0);  // 設定光標在第一行
    lcd.print("Fingerprint found");
    lcd.setCursor(0, 1);  // 設定光標在第二行顯示ID
    lcd.print("ID: " + String(fingerprintID));
  } else {
    lcd.setCursor(0, 0);
    lcd.print("No Match Found");
  }

  delay(1000); // 每秒檢查一次
}

// WiFi 設定
void wifi_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Line Notify 設定
void line_setup() {
  LINE.setToken(LINE_TOKEN);
  LINE.notify("Line Notify Link Confirm.");
}

// 指紋傳感器初始化
void fingerprint_setup() {
  mySerial.begin(57600, SERIAL_8N1, 16, 17); // 初始化指紋傳感器
  finger.begin(57600);
  delay(5);

  if (finger.verifyPassword()) {
    Serial.println("找到指紋傳感器！");
    lcd.setCursor(0, 1);
    lcd.print("Fingerprint OK");
  } else {
    Serial.println("未找到指紋傳感器 :(");
    lcd.setCursor(0, 1);
    lcd.print("No Fingerprint");
    while (1) delay(1); // 無限等待，因為未找到傳感器
  }
}

// 檢查是否有檢測到指紋
int getFingerprintID() {
  uint8_t p = finger.getImage();
  
  // 檢查是否檢測到手指
  if (p == FINGERPRINT_NOFINGER) {
    Serial.println("未檢測到手指");
    return -1;
  } else if (p == FINGERPRINT_OK) {
    Serial.println("指紋檢測成功");
  } else {
    Serial.println("指紋檢測失敗");
    return -1;
  }

  // 搜尋指紋匹配
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.print("找到匹配的指紋，ID #"); 
    Serial.println(finger.fingerID);
    return finger.fingerID;
  } else {
    Serial.println("未找到匹配的指紋");
    LINE.notify("警告!有未知人士正在使用點名器");
    return -1;
  }
}
