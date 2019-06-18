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
    void init(String ssid, String ssidPass, String ssidAP, String ssidApPass, String _ssidStart),
	     init(String ssid, String ssidPass, String ssidAP, String ssidApPass),
         start(), // Подключить к роутеру если не удачно выдти в режим AP
         startSTA(), // Подключится к роутеру
		 isConnect(), // Попыки подключится к роуетеру
         anotherDev(), // Подключение другого модуля к роутеру используя данные этого
		 startAP(),  // Запустить точку доступа
		 DNSRequest(), // Обработка DNS сервера в режиме AP
         stop();

    String scan(boolean Async), // Получить список сетей в эфире
	       network(),
		   StringIP(),
           getURL(String urls);


    boolean modeSTA(),
	        ssidStartOn(),
            ssidOn();

  private:
    uint8_t _cAttempts=120,
            led;
    String _ssid,
           _ssidPass,
		   _ssidAP,
		   _ssidApPass,
		   _ssidStart,
		   _staticIP,
		   _ip,
           _subnet,
           _getway,
           _dns,
           _emptyS,
		   _net;
    Ticker WiFiTimer;
	DNSServer dnsServer;


    boolean  _StaAp,
	         _ssidStartOn = false,
             _ssidOn = false;
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
