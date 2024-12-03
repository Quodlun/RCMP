#include <map>
#include <string>

// 定義 ClassInfo 結構
struct ClassInfo {
    String className;
    int seatNumber;

    ClassInfo(String name = "", int seat = 0) : className(name), seatNumber(seat) {}
};

// 宣告全域 map
std::map<int, ClassInfo> classMap;

void setup() {
    Serial.begin(115200);

    // 初始化資料
    classMap[1] = ClassInfo("1A", 10); // 1號對應1A班，座號10
    classMap[2] = ClassInfo("2B", 20); // 2號對應2B班，座號20
    classMap[3] = ClassInfo("3C", 30); // 3號對應3C班，座號30

    Serial.println("輸入ID來查詢資料...");
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n'); // 讀取一行輸入
        input.trim(); // 去除多餘的空白與換行符
        int id = input.toInt(); // 將輸入轉換為數字

        // 檢查輸入是否有效
        if (input.length() > 0 && id != 0) { 
            if (classMap.find(id) != classMap.end()) {
                ClassInfo info = classMap[id];
                Serial.print("Class: ");
                Serial.print(info.className);
                Serial.print(", Seat Number: ");
                Serial.println(info.seatNumber);
            } else {
                Serial.println("查無此ID");
            }
        } else {
            Serial.println("無效輸入，請輸入數字ID");
        }
    }
}
