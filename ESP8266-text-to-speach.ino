#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include <BearSSLHelpers.h>

// Ustawienia WiFi
const char *ssid = "your-wifi";  // Nazwa sieci WiFi
const char *password = "your-wifi-psswd";  // Hasło WiFi

// Ustawienia MQTT
const char *mqtt_broker = "broker.emqx.io";  // Adres brokera MQTT
const int mqtt_port = 8883;                  // Port MQTT (TLS)
const char *mqtt_topic = "iot";              // Temat MQTT
const char *mqtt_username = "wojti";         // Nazwa użytkownika MQTT
const char *mqtt_password = "Wojti1234";     // Hasło MQTT

// NTP Server settings
const char *ntp_server = "pool.ntp.org";  // Default NTP server
const long gmt_offset_sec = 0;            // GMT offset in seconds (adjust for your time zone)
const int daylight_offset_sec = 0;        // Daylight saving time offset in seconds

// Certyfikat SSL dla brokera MQTT
static const char ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)EOF";

// Obiekty
BearSSL::WiFiClientSecure espClient;
PubSubClient mqtt_client(espClient);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Funkcja łączenia z WiFi
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void syncTime() {
  configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
  Serial.print("Waiting for NTP time sync: ");
  while (time(nullptr) < 8 * 3600 * 2) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Time synchronized");
}

// Funkcja łączenia z brokerem MQTT
void connectToMQTT() {
  BearSSL::X509List serverTrustedCA(ca_cert);
  espClient.setTrustAnchors(&serverTrustedCA);

  while (!mqtt_client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqtt_client.connect("esp8266-client", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker!");
      mqtt_client.subscribe(mqtt_topic);
      Serial.println("Subscribed to topic: " + String(mqtt_topic));
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.println(mqtt_client.state());
      delay(5000);
    }
  }
}

// Funkcja callback dla odbierania wiadomości
void mqttCallback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  String message = "";
  lcd.clear();
  lcd.setCursor(0, 0);

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
    lcd.print((char)payload[i]);
  }

  Serial.print("Message: ");
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);

  // Inicjalizacja LCD
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Connecting...");

  // Połączenie z WiFi
  connectToWiFi();
  syncTime();
  // Konfiguracja MQTT
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(mqttCallback);

  // Połączenie z brokerem MQTT
  connectToMQTT();

  lcd.clear();
  lcd.print("MQTT Ready!");
}

void loop() {
  if (!mqtt_client.connected()) {
    connectToMQTT();
  }
  mqtt_client.loop();
}
