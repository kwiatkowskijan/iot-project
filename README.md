Dokumentacja Projektu IoT

Opis Projektu:
Projekt IoT składa się z kilku zintegrowanych komponentów, które współpracują w celu pomiaru, przetwarzania i wyświetlania danych w czasie rzeczywistym. 
Zostały one stworzone przy pomocy różnych technologii, takich jak ESP8266, MQTT, Docker, Python oraz narzędzia AI (Whisper).

Główne funkcje projektu:
1. Pomiar temperatury:
- Czujnik DS18B20 podłączony do ESP8266 dokonuje pomiaru temperatury.
- Wynik wyświetlany jest w terminalu Arduino oraz wysyłany do brokera MQTT z autoryzacją i szyfrowaniem.
2. Przetwarzanie danych:
- Python subscriber subskrybuje dane z MQTT i zapisuje je w bazie InfluxDB.
- Dane temperaturowe są wizualizowane przy użyciu Grafany.
3. Przetwarzanie mowy (speech-to-text):
- Program wykorzystujący model Whisper konwertuje polski tekst mówiony na tekst pisany.
- Tekst jest wysyłany do brokera MQTT, a ESP8266 odbiera go i wyświetla na wyświetlaczu LCD (górna linia).
Wymagania i Konfiguracja Środowiska

Technologie:
- ESP8266 z czujnikiem DS18B20 oraz wyświetlaczem.
- MQTT broker z autoryzacją (nazwa użytkownika, hasło, TLS).
- Docker (InfluxDB, Grafana).
- Python z bibliotekami:
   - paho-mqtt (dla MQTT).
   - influxdb-client (dla zapisu danych do InfluxDB).
   - Whisper (speech-to-text).
   
Wymagania systemowe:
 - Docker Engine i Docker Compose.
 - Python 3.8 lub nowszy.
 - Arduino IDE do obsługi ESP8266.
