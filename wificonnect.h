#ifndef WIFICONNECT_H
#define WIFICONNECT_H
#include <Ticker.h>
#include <DNSServer.h>
#if defined(ESP8266)
  //#pragma message("Compiling for ESP8266")
  #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#else
  //#pragma message("Compiling for ESP32")
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif


class WIFICONNECT : public ESP8266WiFiClass {
  public:
  // Инициализация
    void init(String ssid, String ssidPass, String ssidAP, String ssidApPass, String ssidStart),
	     init(String ssid, String ssidPass, String ssidAP, String ssidApPass),
		 initIP(String staticIP, String ip, String subnet, String getway),
         start(), // Подключить к роутеру если не удачно выход в режим AP
         startSTA(), // Подключится к роутеру
		 isConnect(), // Попыки подключится к роуетеру
         anotherDev(), // Подключение другого модуля к роутеру используя данные этого
		 startAP(),  // Запустить точку доступа
		 loop(), // Обработка DNS сервера в режиме AP
         stop(),       // Отключить WiFi
		 setHostname(String hostname);

    String scan(boolean Async), // Получить список сетей в эфире
	       network(),           // Список сетей json
		   StringIP(),          // Получить IP адрес
           getURL(String urls); // Отправить GET запрос по адресу


    boolean modeSTA(),          // Вернуть режим WiFi
	        ssidStartOn(),      // Вернуть признак стартовая сеть найден
            ssidOn();           // Вернуть признак стандартная сеть найден

  private:
    void    restartSTA(); // Подключится к роутеру
    uint8_t _cAttempts=120,     // Количество попыток подключения 120 = 60 попыток
            led;                // Cветодиод индикации процесса подключения
    String _ssid,               // SSID сети
           _ssidPass,           // Пароль SSID
		   _ssidAP,             // SSID для точки доступа
		   _ssidApPass,         // Пароль точки доступа
		   _ssidStart,          // Стартовая сеть найдена
		   _staticIP,           // Флаг статический IP
		   _ip,                 // IP адрес
           _subnet,             // Маска сети
           _getway,             // Шлюз
           _dns,                // DNS сервер
           _emptyS,             // Пустая строка
		   _hostname,           //
		   _net;                // Список сетей в формате JSON

    Ticker WiFiTimer;
	DNSServer dnsServer;


    boolean  _StaAp,
	         _ssidStartOn = false,
             _ssidOn = false,
			 _ssidFound = false,
			 _ssidPassEr = false;
    const String ssidS = "ssid",
                 ssidPassS   = "ssidPass",
                 checkboxIPS   = "checkboxIP",
                 ipS   = "ip",
                 subnetS   = "subnet",
                 getwayS   = "getway",
                 dnsS   = "dns",
                 ssidAPS   = "ssidAP",
                 ssidApPassS   = "ssidApPass",
                 ssidStartS = "ssidStart";

};

#endif // WC_H
