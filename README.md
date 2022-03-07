# wificonnect
Arduino библиотека для ESP8266 и ESP32 (не все функции пока работают на ESP32) для легкой работы с WI-FI подключением.

#include <wificonnect.h>
WIFICONNECT MyWiFi;
const String ssid       = "OpenWrt";
const String ssidPass   = "i12345678";
const String ssidAP     = "WiFi-Start";
const String ssidApPass = "";

void setup() {
  Serial.begin(115200);
  Serial.println();
  MyWiFi.restoreCallback( []() { // будет выполнен как только кнопка Reset будет нажата n раз
    Serial.println("Rerstore" ); // запустите свою функцию сброса к заводским настройкам
    });
	MyWiFi.beginRestore(5); // запустим проверку многократного нажатия Reset n раз
  MyWiFi.init(ssid, ssidPass, ssidAP, ssidApPass); // Основные данные для wifi
  MyWiFi.start();        // Запустим WiFi
  if (MyWiFi.modeSTA()) { // Если подключились
    Serial.println("Connect "+ssid);
  } else  Serial.println("AP ="+ssidAP);
  MyWiFi.endRestore(); // сбросим счетчик Restore в ноль
}

void loop() {
MyWiFi.loop(); // DNS в режиме AP
}

Обеспечивает следующий алгоритм работы:
Проверяет наличие сети с заданым именем и подключается к ней если сеть найдена.
Если cети нет, или попытка подключения провалилась из за неправельно указанного пароля переводит модуль в режим AP.
Находясь в режиме AP постоянно ищет целевую сеть и при обнаружении перезагружает модуль для подключения к найденой точке доступа.

