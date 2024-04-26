#include "wificonnect.h"
// Инициализация данных сети

void WIFICONNECT::init(String ssid, String ssidPass, String ssidAP, String ssidApPass) {
  WiFi.mode(WIFI_OFF);
  _ssid = ssid;
  _ssidPass = ssidPass;
  _ssidAP = ssidAP;
  _ssidApPass = ssidApPass;
  _ssidStart = "";
}
// Инициализация данных сети с автоподключением
void WIFICONNECT::init(String ssid, String ssidPass, String ssidAP, String ssidApPass, String ssidStart) {
  WiFi.mode(WIFI_OFF);
  _ssid = ssid;
  _ssidPass = ssidPass;
  _ssidAP = ssidAP;
  _ssidApPass = ssidApPass;
  _ssidStart = ssidStart;
}
// Инициализация параметров сети с ручными настройками
void WIFICONNECT::initIP(String staticIP, String ip, String subnet, String getway) {
  _staticIP = staticIP;         // Флаг статический IP
  _ip = ip;                     // IP адрес
  _subnet = subnet;             // Маска сети
  _getway = getway;            // Шлюз
}
// задать имя устройства в сети
void WIFICONNECT::setHostname(String hostname) {
  _hostname = hostname;
}
void WIFICONNECT::setCallback(WIFICONNECTCb pcb) {
  _pcb = pcb;
}
void WIFICONNECT::start() {
  //scan(false);
  scan(false);
  if (ssidOn()) { // если в эфире найдена сеть _ssid
    startSTA();      // подключится к сети
    if (_ssidStart != _emptyS) { // Включить таймер поиска сети _ssidStart 
#if defined(ESP8266)
        WiFiTimer1.attach_ms(10000, std::bind(&WIFICONNECT::onStart, this));
#else
      //WiFiTimer1.attach(10, std::bind(&WIFICONNECT::onStart, this));
#endif
    }
  } else {
    startAP(); // Иначе включить точку доступа
  }
  if (!_StaAp) startAP();

}

void WIFICONNECT::startSTA() {
#if defined(ESP8266)
  if (_hostname != "") WiFi.hostname (_hostname);
#else
  if (_hostname != "") WiFi.setHostname (_hostname.c_str());
#endif
  if (_staticIP == "1") {
    IPAddress staticIP;
    if (_ip != _emptyS) staticIP.fromString(_ip);
    IPAddress staticGateway;
    if (_getway != _emptyS) staticGateway.fromString(_getway);
    IPAddress staticSubnet;
    if (_subnet != _emptyS) staticSubnet.fromString(_subnet);
    WiFi.config (staticIP, staticGateway, staticSubnet);
  }
  WiFi.mode(WIFI_OFF);
#if defined(ESP8266)
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
#else
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
#endif
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  if (_ssid == _emptyS && _ssidPass == _emptyS) {
    WiFi.begin();
  }
  else {
    WiFi.begin(_ssid.c_str(), _ssidPass.c_str());
  }
  isConnect();
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  //sendSetup(ipS, WiFi.localIP().toString());
  //sendSetup(getwayS, WiFi.gatewayIP().toString());
  //sendSetup(subnetS, WiFi.subnetMask().toString());


}
// Запустить точку доступа
void WIFICONNECT::startAP() {
	
  IPAddress apIP(192, 168, 4, 1);
  //IPAddress apIP(8,8,4,4);
  IPAddress staticGateway(192, 168, 4, 1);
  //IPAddress staticGateway(8,8,4,4);
  IPAddress staticSubnet(255, 255, 255, 0);
  IPAddress apIPdns(8,8,4,4);
  WiFi.mode(WIFI_AP);
  _Channel = random(1, 11);
  WiFi.softAP(_ssidAP.c_str(), _ssidApPass.c_str(),_Channel);
  WiFi.softAPConfig(apIP, staticGateway, staticSubnet);
  dnsServer.start(53, "*", apIPdns);  
  _ip = WiFi.softAPIP().toString();
  
  if (_ssid != _emptyS) {
#if defined(ESP8266)
    if (!_ssidPassEr) WiFiTimer.attach_ms(60000 * 2, std::bind(&WIFICONNECT::restartSTA, this));
#else
    //if (!_ssidPassEr)  WiFiTimer.attach_ms(60000*5, std::bind(&WIFICONNECT::restartSTA, this));
#endif
    _StaAp = false;
	_apOn = true;
  }
}
// Обработка DNS сервера в режиме AP
void WIFICONNECT::loop() {
  if (_ssidFound ) { // если в эфире найдена сеть _ssid
    ESP.restart();
  }
  dnsServer.processNextRequest();
}

void WIFICONNECT::stop() {

}
void WIFICONNECT::restartSTA() {
  scan(true);
  if (ssidOn()) { // если в эфире найдена сеть _ssid
    WiFi.mode(WIFI_OFF);
    _ssidFound = true;
    WiFiTimer.detach();
  }
}

void WIFICONNECT::searchStart(String ssidStart) {
_ssidStart = ssidStart;
if (_ssidStart != _emptyS) { // Включить таймер поиска сети _ssidStart 
#if defined(ESP8266)
        WiFiTimer1.attach_ms(10000, std::bind(&WIFICONNECT::onStart, this));
#else
       //WiFiTimer1.attach_ms(10000, std::bind(&WIFICONNECT::onStart, this));
#endif
    }
}
void WIFICONNECT::onStart() {
  scan(true);
  if (ssidStartOn()) { // если в эфире найдена сеть ssidStartXXXXXXXX
    WiFiTimer1.detach();
	_ssid = _ssidStart;
    _ssidPass = "";  
	if (_pcb) _pcb();
  }
}

String WIFICONNECT::StringIP() {
  if (modeSTA()) {
    _ip = localIP().toString();
  } else
    _ip = WiFi.softAPIP().toString();

  return _ip;
}

uint8_t WIFICONNECT::getChannel() {
  return _Channel;
}

String WIFICONNECT::StringGatewayIP() {
  if (modeSTA()) {
    _getway = gatewayIP().toString();
  } else
    _getway = WiFi.softAPIP().toString();

  return _getway;
}
String WIFICONNECT::StringSubnetMask() {
  if (modeSTA()) {
    _subnet = subnetMask().toString();
  } else
    _subnet = "255.0.0.0";

  return _subnet;
}
// Отправить данные роутера другому модулю ssid ssidPass хранят данные роутера
// Выполнять только когда модуль подключен к нужной сети
// В окружении появляется открытая сеть с настройками по умолчанию.
// ssidStart определяет имя открытой сети
// Настраиваемое устройство реагирует на запросы вида:
// http://192.168.4.1/ssid?ssid=home&ssidPass=i12345678
// http://192.168.4.1/restart?device=ok
// После выполнения модуль подключится к роутеру
void WIFICONNECT::anotherDev() {
	Serial.println("anotherDev");
	WiFi.mode(WIFI_OFF);
  //_ssidStartOn = false;
  //String ssid = _ssid;
  //String pass = _ssidPass;
  _ssid = _ssidStart;
  _ssidPass = "";
  startSTA();
  if (_pcb) _pcb();

  //String gatewayIP = WiFi.gatewayIP().toString();
  //Serial.println("gatewayIP=");
  //Serial.println(gatewayIP);
  //String URL = "http://192.168.4.1/ssid?ssid=" + ssid + "&ssidPass=" + pass;
  //Serial.println(URL);
  //Serial.println(getURL(URL));
  //URL = "http://192.168.4.1/restart?device=ok";
  //Serial.println(URL);
  //Serial.println(getURL(URL));
  //ESP.restart();
 // _ssid = ssid;
 // _ssidPass = pass;
  //startSTA();
  //isConnect();



}

void WIFICONNECT::isConnect() {
  int tries = _cAttempts;
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
	  //Serial.println(tries);
    delay(500);
    if (WiFi.status() == WL_CONNECT_FAILED) {
      _ssidPassEr = true;
      tries = 1;
    }

  }
  if (WiFi.status() == 3) {
    _ip = localIP().toString();
    _StaAp = true;
  }
}

boolean WIFICONNECT::modeSTA() {
  return _StaAp;
}

String WIFICONNECT::network() {
  return _net;
}

String WIFICONNECT::scan(boolean Async) {
  int8_t n;
  if (!Async) {
    n = WiFi.scanNetworks();
  } else n = WiFi.scanComplete();
  //Serial.println(n);
  switch (n) {
    case -2: // Не запущено
      WiFi.scanNetworks(true, true);
      return _net;
      break;
    case -1: // Выполняется
      return _net;
      break;
    default:
      _net = "{\"networks\":["; // Начало строки для списка сетей
      for (uint8_t i = 0; i < n; i++) { // Получаем все сети по порядку
        _net += "{\"ssid\":\""; // Начало раздела ключей отдельной сети
        String ssid = WiFi.SSID(i); // Добавим ключ ssid текущей сети
        if (ssid == _ssid) {_ssidOn = true; // Если основная сеть которую ищем найдена поднимим флаг
		_Channel = WiFi.channel(i);
		}
        if (ssid.indexOf(_ssidStart) == 0 && _ssidStart != _emptyS) { // нужно соханить шаблон		
          _ssidStart = ssid;
          _ssidStartOn = true; // Если стартовая сеть которую ищем найдена поднимим флаг
        }
        _net += ssid + "\","; // Добавим ssid текущей сети в список
        _net += "\"pass\":\""; // Добавим ключ pass текущей сети
#if defined(ESP8266)
        _net += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "*"; // Если сеть открыта
#else
        _net += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "" : "*"; // Если сеть открыта
#endif
        _net += "\",";
        _net += "\"dbm\":"; // Добавим ключ dmb текущей сети
        _net += WiFi.RSSI(i); // Добавим dmb текущей сети в список
        _net += "}"; // Закроем раздел текущей сети
        if (n != i + 1) _net += ",\r\n"; // Если сеть не последняя добавим ,
      }
      _net += "]}"; // Закончим список сетей
      WiFi.scanDelete();
      return _net;
  }
}

boolean WIFICONNECT::ssidStartOn() {

  return _ssidStartOn;
}

boolean WIFICONNECT::ssidOn() {

  return _ssidOn;
}

boolean WIFICONNECT::modeETH() {
  return false;
}
boolean WIFICONNECT::modeAP() {
  return  _apOn;
}

// ------------- Запрос на удаленный URL -----------------------------------------
String WIFICONNECT::getURL(String urls, boolean norequest) {
  String answer;
  HTTPClient http;
  http.begin(urls); //HTTP

  int httpCode = http.GET();
  if (!norequest) {
    if (httpCode == HTTP_CODE_OK) {
      answer = http.getString();
    }
  }
  http.end();
  return answer;
}

String WIFICONNECT::getURL(String urls) {
  return getURL(urls, false);
}


void WIFICONNECT::restoreCallback(WIFICONNECTCb abc) {
  _abc = abc;
}
void WIFICONNECT::endRestore() {
  saveRestartCon(0);
}

#if defined(ESP8266)
void WIFICONNECT::beginRestore(uint8_t n){
  if (ESP.getResetReason() == "External System") {
    if (ESP.rtcUserMemoryRead(0, (uint32_t*) &rtcData, sizeof(rtcData))) {
      uint32_t crcOfData = calculateCRC32((uint8_t*) &rtcData.data[0], sizeof(rtcData.data));
      if (crcOfData != rtcData.crc32) {
        saveRestartCon(0);
      } else {
        uint8_t nnn = rtcData.data[3];
        Serial.println(nnn);  
        if (nnn >= n) {
          saveRestartCon(0);
        if (_abc) _abc();
        } else saveRestartCon(nnn + 1);

      }
    }
  }
}
#else
void WIFICONNECT::beginRestore(uint8_t n){}
#endif
#if defined(ESP8266)
void WIFICONNECT::saveRestartCon(uint8_t n){
	 for (size_t i = 0; i < sizeof(rtcData.data); i++) {
    rtcData.data[i] = n;
  }
  rtcData.crc32 = calculateCRC32((uint8_t*) &rtcData.data[0], sizeof(rtcData.data));
  ESP.rtcUserMemoryWrite(0, (uint32_t*) &rtcData, sizeof(rtcData));
}
#else
void WIFICONNECT::saveRestartCon(uint8_t n){}
#endif


uint32_t WIFICONNECT::calculateCRC32(const uint8_t *data, size_t length){
	  uint32_t crc = 0xffffffff;
  while (length--) {
    uint8_t c = *data++;
    for (uint32_t i = 0x80; i > 0; i >>= 1) {
      bool bit = crc & 0x80000000;
      if (c & i) {
        bit = !bit;
      }
      crc <<= 1;
      if (bit) {
        crc ^= 0x04c11db7;
      }
    }
  }
  return crc;
}