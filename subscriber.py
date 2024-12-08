import ssl
import json
from datetime import datetime
from paho.mqtt import client as mqtt_client
import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

# Zapisz certyfikat jako plik .crt w tym samym katalogu co skrypt
CA_CERT_FILE = "ca_cert.crt"

# Ustawienia MQTT
broker = 'broker.emqx.io'
port = 8883
username = 'wojti'
password = 'Wojti1234'

# Ustawienia InfluxDB
influx_url = "http://localhost:8086"
bucket = "temperature"
org = "iot-org"
token = "Eb2ibTRjba5rfXm-gSvgQnS1U-m4PrIjjq8MlTA2o3moB0N4xp7WmzbzJaVckakaiMm3L_3qFqSbW0EdP_NSUA=="

# Połączenie z InfluxDB
influx_client = InfluxDBClient(url=influx_url, token=token, org=org)
write_api = influx_client.write_api(write_options=SYNCHRONOUS)

def connect_mqtt():
    client = mqtt_client.Client()
    client.username_pw_set(username, password)

    # Ustawienie certyfikatu z pliku
    client.tls_set(ca_certs=CA_CERT_FILE, tls_version=ssl.PROTOCOL_TLSv1_2)
    client.tls_insecure_set(False)

    client.connect(broker, port)
    return client

# try:
#     test_point = Point("temperature").tag("device", "test_device").field("value", 23.5)
#     print(f"Test write to InfluxDB: {test_point.to_line_protocol()}")
#     write_api.write(bucket=bucket, record=test_point)
#     print("Test write successful")
# except Exception as e:
#     print(f"Error during test write: {e}")


def save_to_influx(temperature):
    try:
        point = Point("temperature") \
            .tag("device", "esp8266") \
            .field("value", temperature)
            # .time(datetime.utcnow(), WritePrecision.NS)
        print(f"Writing to InfluxDB: {point.to_line_protocol()}")
        write_api.write(bucket=bucket, record=point)
        print(f"Saved to InfluxDB: temperature={temperature}")
    except Exception as e:
        print(f"Error writing to InfluxDB: {e}")


def on_message(client, userdata, message):
    try:
        payload = message.payload.decode()
        print(f"Received message: {payload} from topic: {message.topic}")
        
        # Debugowanie danych JSON
        try:
            data = json.loads(payload)
            print(f"Parsed JSON: {data}")
        except json.JSONDecodeError as e:
            print(f"JSON Decode Error: {e}")
            return

        temperature = data.get("temperature")
        print(f"Extracted temperature: {temperature}")

        if temperature is not None:
            save_to_influx(temperature)
        else:
            print("Temperature not found in payload")
    except Exception as e:
        print(f"Error processing message: {e}")


def run():
    client = connect_mqtt()
    client.loop_start()
    client.on_message = on_message
    client.subscribe("iot")
    print("MQTT client started and subscribed to topic iot")
    try:
        while True:
            time.sleep(1)  # Zapewnia minimalne zużycie CPU
    except KeyboardInterrupt:
        print("Exiting...")
        client.loop_stop()

if __name__ == '__main__':
    run()
