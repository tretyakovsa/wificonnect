#include <wificonnect.h>
WIFICONNECT MyWiFi;
const String ssid       = "OpenWrt";
const String ssidPass   = "i12345678";
const String ssidAP     = "WiFi-Start";
const String ssidApPass = "";

void setup() {
  Serial.begin(115200);
  Serial.println();
  MyWiFi.init(ssid, ssidPass, ssidAP, ssidApPass); // Оснавные данные для wifi
  MyWiFi.start();        // Запустим WiFi
  if (MyWiFi.modeSTA()) {
    Serial.println("Connect "+ssid);
  } else  Serial.println("AP ="+ssidAP);
}

void loop() {
MyWiFi.loop(); // DNS в режиме AP
}

