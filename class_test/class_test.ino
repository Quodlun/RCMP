// 定義類別資訊結構
struct ClassInfo {
    String className;  // 班級名稱
    int seatNumber;    // 座號
    String name;       // 學生姓名

    // 建構函數，初始化班級名稱、座號和姓名
    ClassInfo(String name, int seat, String studentName) : className(name), seatNumber(seat), name(studentName) {}
};

// 創建一個存儲班級資訊的陣列，包含學號、班級名稱、座號和學生姓名
ClassInfo classArray[] = {
    ClassInfo("1A", 10, "張三"),  // ID 1 對應 1A班，座號 10，姓名 張三
    ClassInfo("1B", 20, "李四"),  // ID 2 對應 1B班，座號 20，姓名 李四
    ClassInfo("2A", 5, "王五")    // ID 3 對應 2A班，座號 5，姓名 王五
};

void setup() {
    // 初始化串口通訊
    Serial.begin(115200);
    // 等待串口準備
    while (!Serial);

    // 輸出提示訊息
    Serial.println("請輸入學號：");
}

void loop() {
    // 檢查是否有資料可讀
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();  // 去除換行符或空格
        int id = input.toInt();  // 將輸入轉換為整數

        Serial.print("您輸入的學號是：");
        Serial.println(id);

        // 檢查學號是否有效
        if (id > 0 && id <= sizeof(classArray) / sizeof(classArray[0])) {
            // 如果找到該學號，顯示班級名稱、座號和姓名
            } else {
            // 如果找不到該學號
            Serial.println("查無此ID，請再試一次！");
        }
    }
}

