#include "arduino_secrets.h"
/*
  Discord WebHook Example for ESP32
*/
#include "discord.h"

void setup() {
  Serial.begin(115200);
  connectWIFI();
  sendDiscord("Hello World!");
}

void loop() {
}
