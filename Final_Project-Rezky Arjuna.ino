#define BLYNK_TEMPLATE_ID "TMPL67MIChwYh"
#define BLYNK_TEMPLATE_NAME "Air Immortal"
#define BLYNK_PRINT Serial
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Token Auth Blynk, kredensial WiFi
char auth[] = "YW7gzaI99VYemuUczUGWIAAjhDQhzDJ6"; // Masukkan token Auth Anda
char ssid[] = "Gugu"; // Masukkan SSID WiFi Anda
char pass[] = "12345678"; // Masukkan kata sandi WiFi Anda

BlynkTimer timer;
int pinValue = 0;

#define Buzzer 27
#define Green 25
#define Red  26
#define Blue  13
#define Sensor 34

void setup() {
  Serial.begin(115200);
  
  // Atur mode pin
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Sensor, INPUT);
  
  // Hubungkan ke Blynk
  Blynk.begin(auth, ssid, pass);
  
  // Atur timer untuk menjalankan fungsi notifikasi setiap 100 ms
  timer.setInterval(100L, notifikasi);
}

// Fungsi untuk membaca pin virtual V0 dari Blynk
BLYNK_WRITE(V0) {
  pinValue = param.asInt();
}

// Fungsi untuk menangani deteksi gas dan notifikasi
void notifikasi() {
  int sensorValue = analogRead(Sensor);
  Serial.println(sensorValue);
  
  if (pinValue == 1) {
    if (sensorValue <= 850) { // Ambang batas bisa disesuaikan sesuai kebutuhan
      digitalWrite(Green, HIGH);
      digitalWrite(Blue, LOW);
      digitalWrite(Red, LOW);
      digitalWrite(Buzzer, LOW);
    } else if (sensorValue > 850 && sensorValue <= 2000) {
      Serial.println("Peringatan! Kebocoran gas terdeteksi");
      digitalWrite(Green, LOW);
      digitalWrite(Blue, HIGH);
      digitalWrite(Red, LOW);
      digitalWrite(Buzzer, HIGH);
      delay(500);
    } else if (sensorValue > 2000) {
      Serial.println("Peringatan! Kebocoran gas berbahaya terdeteksi");
      digitalWrite(Green, LOW);
      digitalWrite(Blue, LOW);
      digitalWrite(Red, HIGH);
      digitalWrite(Buzzer, HIGH);
      delay(500);
    }
    Blynk.virtualWrite(V5, sensorValue);
     Blynk.virtualWrite(V2, sensorValue);
  } else {
    digitalWrite(Red, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green, LOW);
    digitalWrite(Blue, LOW);
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
