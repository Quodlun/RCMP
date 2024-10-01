#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h> 
#include <DallasTemperature.h> 
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>   // 引入LCD庫

// Pin Definitions
#define irSensorPin 2
#define tempSensorPin 4
#define LINE_TOKEN "MbcvZH9nf1mvsSkXBzEpoQDQNOaNkWpHJ2nmoSBSBpz"

// WiFi Credentials
const char* ssid     = "When Can My Internet Get Better";
const char* password = "O00O00O0";

// LCD Setup (I2C Address is typically 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C 地址 0x27，顯示為 16x2 字符

// Temperature Sensor Setup
OneWire oneWire(tempSensorPin);
DallasTemperature sensors(&oneWire);

// Fingerprint Sensor Setup
HardwareSerial mySerial(2); 
Adafruit_Fingerprint finger(&mySerial);

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C and LCD
  Wire.begin(21, 22); // 使用 GPIO 21 (SDA) 和 GPIO 22 (SCL) 初始化 I2C
  lcd.begin(16,2);        // 初始化 LCD 顯示
  lcd.backlight();    // 開啟 LCD 背光
  lcd.print("Initializing...");

  pinMode(irSensorPin, INPUT_PULLUP);

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