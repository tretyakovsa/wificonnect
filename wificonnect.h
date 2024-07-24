#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#if defined(ESP8266)
  //#pragma message("Compiling for ESP8266")
  #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#else
	
  //#pragma message("Compiling for ESP32")
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif
#include <Ticker.h>

typedef std::function<void()> WIFICONNECTCb;

#if defined(ESP8266)
class WIFICONNECT : public ESP8266WiFiClass {
#else
class WIFICONNECT : public WiFiClass {
#endif
  public:
  // Инициализация
    void init(String ssid, String ssidPass, String ssidAP, String ssidApPass, String ssidStart),
	     init(String ssid, String ssidPass, String ssidAP, String ssidApPass),
         initStaAndAP(),
		 initIP(String staticIP, String ip, String subnet, String getway),
         start(), // Подключить к роутеру, если не удачно - вход в режим AP
         startSTA(), // Подключится к роутеру
		 isConnect(), // Попыки подключится к роуетеру
         anotherDev(), // Подключение другого модуля к роутеру используя данные этого
		 startAP(),  // Запустить точку доступа
		 loop(), // Обработка DNS сервера в режиме AP
         stop(),       // Отключить WiFi
		 searchStart(String ssidStart),
		 setCallback(WIFICONNECTCb pcb),
		 setHostname(String hostname),
		 beginRestore(uint8_t n),
		 endRestore(),
		 restoreCallback(WIFICONNECTCb abc);

    String scan(boolean Async), // Получить список сетей в эфире
	       network(),           // Список сетей json
		   StringIP(),          // Получить IP адрес
		   StringGatewayIP(),
		   StringSubnetMask(),
		   getURL(String urls, boolean norequest),
           getURL(String urls); // Отправить GET запрос по адресу
		   
   uint8_t getChannel();		   


    boolean modeSTA(),          // Вернуть режим WiFi
	        modeETH(),
			modeAP(),
	        ssidStartOn(),      // Вернуть признак стартовая сеть найден
            ssidOn();           // Вернуть признак стандартная сеть найден
			

  private:
    void    restartSTA(), // Подключится к роутеру
            saveRestartCon(uint8_t n),
	        onStart(); // Подключится к внешнему устройству
    uint8_t _cAttempts=120,     // Количество попыток подключения 120 = 60 попыток
            led,                // Cветодиод индикации процесса подключения
			_Channel;
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
    uint32_t calculateCRC32(const uint8_t *data, size_t length);
	WIFICONNECTCb _pcb;
	WIFICONNECTCb _abc;
    Ticker WiFiTimer;
	Ticker WiFiTimer1;
	#if defined(ESP8266)
	DNSServer dnsServer;
	#else
    #endif

    boolean  _StaAp= false,
	         _StaAndAp= false,
	         _apOn= false,
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
    struct {
                 uint32_t crc32;
                 byte data[4];
                 } rtcData;

};

#endif // WC_H
