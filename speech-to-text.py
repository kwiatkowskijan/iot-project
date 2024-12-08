import warnings
warnings.filterwarnings("ignore", category=FutureWarning)

import ssl
import sounddevice as sd
import whisper
import numpy as np
from paho.mqtt import client as mqtt_client
import time

# Zapisz certyfikat jako plik .crt w tym samym katalogu co skrypt
CA_CERT_FILE = "ca_cert.crt"

# Ustawienia MQTT
broker = 'broker.emqx.io'
port = 8883
username = 'wojti'
password = 'Wojti1234'
topic = 'iot'

# Załaduj model Whisper
model = whisper.load_model("base")

# Połączenie z brokerem MQTT
def connect_mqtt():
    client = mqtt_client.Client()
    client.username_pw_set(username, password)

    # Ustawienie certyfikatu z pliku
    client.tls_set(ca_certs=CA_CERT_FILE, tls_version=ssl.PROTOCOL_TLSv1_2)
    client.tls_insecure_set(False)

    client.connect(broker, port)
    return client

# Załaduj model Whisper
model = whisper.load_model("small")  # Możesz użyć innego modelu np. "small" lub "medium"

# Funkcja nagrywania audio
def record_audio(duration=5, samplerate=16000):
    print("Recording...")
    audio = sd.rec(int(duration * samplerate), samplerate=samplerate, channels=1, dtype='float32')
    sd.wait()
    print("Recording finished.")
    return np.squeeze(audio)

# Funkcja konwersji mowy na tekst
def speech_to_text(audio, samplerate=16000):
    print("Transcribing...")
    result = model.transcribe(audio, fp16=False, language="pl")
    print("Transcription:", result["text"])
    return result["text"]

# Funkcja publikowania tekstu przez MQTT
def publish_message(client, message):
    client.publish(topic, message)
    print(f"Published to MQTT: {message}")

# Główna funkcja
def main():
    client = connect_mqtt()
    client.loop_start()
    try:
        while True:
            audio = record_audio()  # Nagrywanie 5 sekund audio
            transcription = speech_to_text(audio)  # Zamiana mowy na tekst
            publish_message(client, transcription)  # Publikowanie do brokera
            time.sleep(1)  # Krótka przerwa przed następną iteracją
    except KeyboardInterrupt:
        print("Exiting...")
        client.loop_stop()

if __name__ == "__main__":
    main()
