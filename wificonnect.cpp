#include "wificonnect.h"

void WIFICONNECT::init(String ssid, String ssidPass, String ssidAP, String ssidApPass) {
	WiFi.mode(WIFI_OFF);
	_ssid = ssid;
    _ssidPass = ssidPass;
	_ssidAP = ssidAP;
	_ssidApPass = ssidApPass;
	_ssidStart = "";
}
void WIFICONNECT::init(String ssid, String ssidPass, String ssidAP, String ssidApPass, String ssidStart) {
	WiFi.mode(WIFI_OFF);
	_ssid = ssid;
    _ssidPass = ssidPass;
	_ssidAP = ssidAP;
	_ssidApPass = ssidApPass;
	_ssidStart = ssidStart;
}
void WIFICONNECT::start() {
	scan(false);
	if (ssidOn()){ // если в эфире найдена сеть _ssid
  startSTA();      // подключится к сети
  //isConnect();

	} else {
		startAP(); // Иначе включить точку доступа
	}
	if (!_StaAp) startAP();
	//WiFiTimer.attach_ms(20000, std::bind(&WIFICONNECT::stop, this));
}

void WIFICONNECT::startSTA() {
  WiFi.mode(WIFI_OFF);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.hostname ("test");
  String volume;
  if (_staticIP=="1") {
	IPAddress staticIP;
    if (_ip != _emptyS) staticIP.fromString(_ip);
	IPAddress staticGateway;
    if (_getway != _emptyS) staticGateway.fromString(_getway);
	IPAddress staticSubnet;
    if (_subnet != _emptyS) staticSubnet.fromString(_subnet);
	WiFi.config (staticIP, staticGateway, staticSubnet);
  }
  //Serial.println("Begin");
  if (_ssid == _emptyS && _ssidPass == _emptyS) {
	  //Serial.println("empty");
    WiFi.begin();
  }
  else {
	  //Serial.println("pass");
    WiFi.begin(_ssid.c_str(), _ssidPass.c_str());
  }
  isConnect();
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
	if (WiFi.status()== 3) {
		_ip = localIP().toString();
		_StaAp=true;
		}

}
// Запустить точку доступа
void WIFICONNECT::startAP() {
 IPAddress apIP(192, 168, 4, 1);
  IPAddress staticGateway(192, 168, 4, 1);
  IPAddress staticSubnet(255, 255, 255, 0);
  WiFi.softAPConfig(apIP, staticGateway, staticSubnet);
  WiFi.softAP(_ssidAP.c_str(), _ssidApPass.c_str());
  WiFi.mode(WIFI_AP);
  dnsServer.start(53, "*", apIP);
   _StaAp=false;
}
// Обработка DNS сервера в режиме AP
void WIFICONNECT::DNSRequest() {
	//if (!_StaAp)
		dnsServer.processNextRequest();
}

void WIFICONNECT::stop() {
	//if (_ssidStartOn){
	//	WiFi.mode(WIFI_OFF);
	//anotherDev();
	//}
	//else {scan(true);}
	scan(true);
}


String WIFICONNECT::StringIP(){
	return _ip;
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
 _ssidStartOn = false;
  String ssid = _ssid;
  String pass = _ssidPass;
  _ssid = _ssidStart;
  _ssidPass ="";
  startSTA();
  //isConnect();
  //delay(15000);

  String gatewayIP = WiFi.gatewayIP().toString();
  //Serial.println("gatewayIP=");
  //Serial.println(gatewayIP);
  String URL = "http://192.168.4.1/ssid?ssid="+ssid+"&ssidPass="+pass;
  //Serial.println(URL);
  //Serial.println(getURL(URL));
  URL = "http://192.168.4.1/restart?device=ok";
  //Serial.println(URL);
  //Serial.println(getURL(URL));

  _ssid = ssid;
  _ssidPass = pass;
   startSTA();
   //isConnect();



}

void WIFICONNECT::isConnect() {
   int tries = _cAttempts;
   while (--tries && WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print(".");
	//Serial.println(WiFi.status());
  }
  //Serial.println(WiFi.status());

}

boolean WIFICONNECT::modeSTA(){
	return _StaAp;
}

String WIFICONNECT::network(){
	return _net;
}

String WIFICONNECT::scan(boolean Async) {
	int8_t n;
	if (!Async) {n = WiFi.scanNetworks();
	} else n = WiFi.scanComplete();
	//Serial.println(n);
  switch (n) {
    case -2: // Не запущено
    //Serial.println("SkanStart");
      WiFi.scanNetworks(true, true);
      return _net;
      break;
    case -1: // Выполняется
    //Serial.println("SkanWork");
      return _net;
      break;
    default:
	//Serial.println("SkanUpdate");
     _net="{\"networks\":["; // Начало строки для списка сетей
 for (uint8_t i = 0; i < n; i++) { // Получаем все сети по порядку
	 _net +="{\"ssid\":\""; // Начало раздела ключей отдельной сети
	 String ssid = WiFi.SSID(i); // Добавим ключ ssid текущей сети
	 if (ssid == _ssid) _ssidOn=true; // Если сеть которую ищем найдена поднимим флаг
	 if (ssid == _ssidStart) _ssidStartOn=true; // Если сеть которую ищем найдена поднимим флаг
	 _net += ssid + "\","; // Добавим ssid текущей сети в список
	 _net +="\"pass\":\""; // Добавим ключ pass текущей сети
	 _net += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "*"; // Если сеть открыта
	 _net += "\",";
	 _net +="\"dmb\":";  // Добавим ключ dmb текущей сети
	 _net += WiFi.RSSI(i); // Добавим dmb текущей сети в список
	 _net += "}"; // Закроем раздел текущей сети
	 if (n != i+1) _net += ",\r\n"; // Если сеть не последняя добавим ,
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

// ------------- Запрос на удаленный URL -----------------------------------------
String WIFICONNECT::getURL(String urls) {
  String answer;
  HTTPClient http;
  http.begin(urls); //HTTP
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    answer = http.getString();
  }
  http.end();
  return answer;
}
