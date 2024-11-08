//紅色接地 黑色接17 黃16 綠and白接5V 型號 HLK-ZW0632 接腳要改自己改
#include <CapacitiveFingerprint.h>  // 假設電容式指紋傳感器庫

// 指紋傳感器設置
HardwareSerial mySerial(2); 
CapacitiveFingerprint finger(&mySerial);  // 使用電容式指紋模組的庫

void setup() {
  Serial.begin(115200);
  fingerprint_setup();  // 初始化指紋傳感器
  Serial.println("System Ready");
}

void loop() {
  // 檢查指紋
  int fingerprintID = getFingerprintID();
  if (fingerprintID >= 0) {
    Serial.print("識別到指紋，ID: ");
    Serial.println(fingerprintID);
  } else {
    Serial.println("No Match Found");
  }
  delay(1000); // 每秒檢查一次
}

// 指紋傳感器初始化
void fingerprint_setup() {
  mySerial.begin(57600, SERIAL_8N1, 16, 17); // 初始化串口，用於指紋傳感器
  finger.begin(57600);
  delay(5);

  if (finger.verifyPassword()) {
    Serial.println("找到電容式指紋傳感器！");
  } else {
    Serial.println("未找到指紋傳感器 :(");
    while (1) delay(1); // 無限等待，因為未找到傳感器
  }
}

// 檢查是否有檢測到指紋
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
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {  // 匹配成功
    Serial.print("找到匹配的指紋，ID #"); 
    Serial.println(finger.fingerID);
    return finger.fingerID;
  } else {
    Serial.println("未找到匹配的指紋");
    return -1;
  }
}
