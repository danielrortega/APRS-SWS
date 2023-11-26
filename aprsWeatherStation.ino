#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiManager.h>
#include <Adafruit_BMP085.h> 

#define DHTPIN D5
#define DHTTYPE DHT22

#define USE_SERIAL Serial
#define SLEEP_DELAY 3000

ESP8266WiFiMulti WiFiMulti;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "a.ntp.br");

const int resetHour = 2;  // Horário para o reset diário (0-23)

String serialData = "";

// Informações do Weather Underground
const char *wundergroundServer = "weatherstation.wunderground.com";
const char *wundergroundPath = "/weatherstation/updateweatherstation.php";
const char *wundergroundID = "YOUR_WUNDERGROUND_ID";
const char *wundergroundPassword = "YOUR_WUNDERGROUND_PASSWORD";

// Informações da estação APRS
const char *aprsCallsignPrefix = "YOUR_APRS_CALLSIGN_PREFIX";
const char *aprsCallsignSSID = "YOUR_APRS_CALLSIGN_SSID";
const char *aprsPassword = "YOUR_APRS_PASSWORD";
const char *aprsVersion = "YOUR_APRS_VERSION";
const char *aprsLatitude = "YOUR_APRS_LATITUDE";
const char *aprsLongitude = "YOUR_APRS_LONGITUDE";
const char *aprsComment = "YOUR_APRS_COMMENT";

// Function declarations
float calculateDewPoint(float temperature, float humidity);
void sendToWeatherUnderground(float temperature, float humidity, float dewPoint, float pressure);

// Configuração inicial para atualizações OTA
void setupOTA() {
  ArduinoOTA.setHostname("WeatherStation");
  ArduinoOTA.setPassword("admin");
  ArduinoOTA.begin();
}

// Atualiza o horário via NTP
void updateTimeFromNTP() {
  timeClient.begin();
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
}

// Reinicia a placa diariamente no horário especificado
void dailyReset() {
  if (hour() == resetHour && minute() == 0 && second() == 0) {
    ESP.restart();
  }
}

void closeConnection(HTTPClient *pClientToClose) {
  pClientToClose->end();
  delay(SLEEP_DELAY);
}

void setup() {
  USE_SERIAL.begin(115200);
  USE_SERIAL.flush();
  delay(1000);

  // Configuração OTA
  setupOTA();

  WiFiManager wifiManager;

  wifiManager.setConnectTimeout(30);  // 30 segundos, ajuste conforme necessário
  wifiManager.setDebugOutput(true);
  wifiManager.autoConnect("APRS WeatherStation");
  pinMode(DHTPIN, INPUT_PULLUP);      // Ativa o resistor pull up da porta do ESP8266
  dht.begin();                        // Inicia o DHT22
  bmp.begin();                        // Inicia o BMP180
}

void loop() {
  // Atualização OTA
  ArduinoOTA.handle();

  // Atualização do horário via NTP
  updateTimeFromNTP();

  // Reinicia a placa diariamente
  dailyReset();

  if (WiFiMulti.run() == WL_CONNECTED) {
    HTTPClient http;

    const uint16_t port = 14580;
    const char *host = "aprs.pu3dro.com.br";
    WiFiClient client;

    if (!client.connect(host, port)) {
      return;
    }

    client.print("user ");
    client.print(aprsCallsignPrefix);
    client.print("-");
    client.print(aprsCallsignSSID);
    client.print(" pass ");
    client.print(aprsPassword);
    client.print(" vers ");
    client.print(aprsVersion);
    client.println();
    delay(250);

    float h22 = dht.readHumidity();
    float t22 = dht.readTemperature();

    // Leitura dos dados do BMP180
    float pressure = bmp.readPressure() / 100.0F;  // Pressão em hPa
    float pressureTenths = pressure * 10;          // Pressão em tenths of hPascal

    if (isnan(t22) || isnan(h22)) {
      Serial.println("Failed to read from DHT");
    } else {
      USE_SERIAL.print("Temperature dht: ");
      USE_SERIAL.println(t22);
      USE_SERIAL.print("Humidity dht: ");
      USE_SERIAL.println(h22);
      USE_SERIAL.print("Pressure: ");
      USE_SERIAL.println(pressure);

      // Calcula o ponto de orvalho
      float dewPoint = calculateDewPoint(t22, h22);
      USE_SERIAL.print("Dew Point: ");
      USE_SERIAL.println(dewPoint);

      // Envia dados para o Weather Underground
      sendToWeatherUnderground(t22, h22, dewPoint, pressure);
      
    }

    const int f = static_cast<int>(1.8 * t22 + 32);
    const int h = static_cast<int>(h22);

    USE_SERIAL.println(f);
    USE_SERIAL.println(h);

    client.print(aprsCallsignPrefix);
    client.print("-");
    client.print(aprsCallsignSSID);
    client.print(">");
    client.print("APRSWX,TCPIP*,qAC,WIDE1-1:=");
    client.print(aprsLatitude);
    client.print("/");
    client.print(aprsLongitude);
    client.print("_");
    client.print(".../...g...");
    if (f >= 0) {
      client.print("t0");
      client.print(f);
    } else {
      client.print("t");
      client.print(f);
    }
    client.print("r...p...P...h");
    client.print(h);
    client.print("b");
    client.print(pressureTenths);                 // Adiciona a medida de pressão em tenths of hPascal
    client.println("");
    client.println(aprsCallsignPrefix);
    client.print(">");
    client.print("APRSWX,TCPIP*,qAC,WIDE1-1:> ");
    client.print(aprsComment);
    delay(300000);
  }
}

float calculateDewPoint(float temperature, float humidity) {
  // Calcula o ponto de orvalho em Celsius
  return temperature - ((100 - humidity) / 5.0);
}

void sendToWeatherUnderground(float temperature, float humidity, float dewPoint, float pressure) {
  // Monta a URL para enviar dados para o Weather Underground
  String url = String("/weatherstation/updateweatherstation.php?");
  url += "ID=" + String(wundergroundID);
  url += "&PASSWORD=" + String(wundergroundPassword);
  url += "&dateutc=now";
  url += "&tempf=" + String(temperature * 1.8 + 32); // Temperatura em Fahrenheit
  url += "&humidity=" + String(humidity);            // Umidade em %
  url += "&dewptf=" + String(dewPoint * 1.8 + 32);   // Ponto de orvalho em Fahrenheit
  url += "&baromin=" + String(pressure * 0.02953);   // Adiciona a medida de pressão em inHg
  url += "&action=updateraw";

  // Conecta-se ao servidor Weather Underground
  WiFiClient wuClient;
  if (wuClient.connect(wundergroundServer, 80)) {
    wuClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + wundergroundServer + "\r\n" +
                   "Connection: close\r\n\r\n");
    while (wuClient.connected() && !wuClient.available()) delay(10);
    while (wuClient.available()) {
      char c = wuClient.read();
      Serial.write(c);
    }
    wuClient.stop();
  } else {
    Serial.println("Failed to connect to Weather Underground");
  }
}

