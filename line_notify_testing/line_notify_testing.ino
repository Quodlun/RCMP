#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>

// 修改成上述寄到登入郵箱的 Token權杖號碼
#define LINE_TOKEN "K7YFufx6idzkQiz6mNfiz8oUde7Kp8cT7xte50QQ3kmK7YFufx6idzkQiz6mNfiz8oUde7Kp8cT7xte50QQ3km"

// 設定無線基地台SSID跟密碼
const char* ssid     = "FuNet-UAP";
const char* password = "25785657";

void setup ()
{
  Serial.begin(115200);          // 初始化

  WiFi.mode(WIFI_STA);
  // 連接無線基地台
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop(void)
{
  // 顯示 Line版本
  Serial.println(LINE.getVersion());
 
  LINE.setToken(LINE_TOKEN);

  // 先換行再顯示
  LINE.notify("Debug: Line Notify Confirm");
      
  // 每2分鐘發送一次(delay120000)
  delay(5000);
}