#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Fingerprint.h>

// Pin Definitions
#define irSensorPin 2
#define tempSensorPin 4
#define LINE_TOKEN "Y3nL5gLv1Q7UiDshiv2rPZAXc4jbouEqzt04HmilnZo"

// WiFi Credentials
const char* ssid     = "When Can My Internet Get Better";
const char* password = "O00O00O0";

// Temperature Sensor Setup
OneWire oneWire(tempSensorPin);
DallasTemperature sensors(&oneWire);

// Fingerprint Sensor Setup
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger(&mySerial);

void setup()
{
  Serial.begin(115200);
  pinMode(irSensorPin, INPUT_PULLUP);

  // Initialize Sensors
  sensors.begin();
  wifi_setup();
  line_setup();
  fingerprint_setup();
}

void loop()
{
  checkIRSensorAndSendData();
  int fingerprintID = getFingerprintID();

  // 如果识别到指纹，发送指纹ID和信心指数到Line Notify
  if (fingerprintID >= 0) {
    LINE.notify("Fingerprint ID: " + String(fingerprintID) + 
                ", Confidence: " + String(finger.confidence));
  }

  delay(100);
}

void wifi_setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void line_setup()
{
  LINE.setToken(LINE_TOKEN);
  LINE.notify("Line Notify Link Confirm.");
}

void checkIRSensorAndSendData() {
  int L = digitalRead(irSensorPin);
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (L == 0) {
    LINE.notify("Obstacle detected");
    LINE.notify("Temperature: " + String(tempC) + " °C");
  } else {
    LINE.notify("=== All clear");
    LINE.notify("Temperature: " + String(tempC) + " °C");
  }
}

void fingerprint_setup()
{
  mySerial.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);
  delay(5);

  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println("Reading sensor parameters");
  finger.getParameters();
  Serial.print("Status: 0x"); Serial.println(finger.status_reg, HEX);
  Serial.print("Sys ID: 0x"); Serial.println(finger.system_id, HEX);
  Serial.print("Capacity: "); Serial.println(finger.capacity);
  Serial.print("Security level: "); Serial.println(finger.security_level);
  Serial.print("Device address: "); Serial.println(finger.device_addr, HEX);
  Serial.print("Packet len: "); Serial.println(finger.packet_len);
  Serial.print("Baud rate: "); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0)
  {
    Serial.println("Sensor doesn't contain any fingerprint data.");
  }
  else
  {
    Serial.print("Sensor contains "); 
    Serial.print(finger.templateCount); 
    Serial.println(" templates");
  }
}

uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return -1;
    default:
      Serial.println("Unknown error");
      return -1;
  }

  p = finger.image2Tz();
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return -1;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return -1;
    default:
      Serial.println("Unknown error");
      return -1;
  }

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Found a print match!");
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    return finger.fingerID;
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return -1;
  }
  else if (p == FINGERPRINT_NOTFOUND)
  {
    Serial.println("Did not find a match");
    return -1;
  }
  else
  {
    Serial.println("Unknown error");
    return -1;
  }
}