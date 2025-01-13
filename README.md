Project Description:  
The IoT project consists of several integrated components that work together to measure, process, and display real-time data. These components were created using various technologies such as ESP8266, MQTT, Docker, Python, and AI tools (Whisper).

Main Features of the Project:  
1. Temperature Measurement:  
   - The DS18B20 sensor connected to the ESP8266 measures the temperature.  
   - The result is displayed on the Arduino terminal and sent to the MQTT broker.  
   - The MQTT broker forwards the data to InfluxDB, running on Docker.  
2. Data Processing:  
   - A Python subscriber subscribes to the data from MQTT and saves it in the InfluxDB database.  
   - Temperature data is visualized using Grafana.  
3. Speech Processing (Speech-to-Text):  
   - A program using the Whisper model converts spoken words into written text.  
   - The text is sent to the MQTT broker, and the ESP8266 receives and displays it on the LCD screen (top line).  

Environment Requirements and Configuration  

Technologies:  
- ESP8266 with DS18B20 sensor and LCD display.  
- MQTT broker with authentication (username, password, TLS).  
- Docker (InfluxDB, Grafana).  
- Python with libraries:  
   - paho-mqtt (for MQTT).  
   - influxdb-client (for writing data to InfluxDB).  
   - Whisper (speech-to-text).  

System Requirements:  
- Docker Engine and Docker Compose.  
- Python 3.8 or newer.  
- Arduino IDE for ESP8266 management.  

Authors: @kwiatkowksijan, @Olesz96
