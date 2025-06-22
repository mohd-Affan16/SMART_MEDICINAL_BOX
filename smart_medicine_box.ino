// IoT Medicine Reminder Final Code
// Developed using ESP8266, RTC module, IR sensor, and ThingSpeak integration
// Purpose: Set a medicine time from ThingSpeak and buzz at that time if medicine is not taken.

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include "RTClib.h"

// ================= USER CONFIGURATIONS ======================
const char* ssid = "ENTER_YOUR_WIFI_NAME";      // Your Wi-Fi/Hotspot Name
const char* password = "ENTER_YOUR_WIFI_PASSWORD";  // Your Wi-Fi Password

const String readAPIKey = "ETL01GN5PQC8J82E";     // ThingSpeak Read API Key
const String writeAPIKey = "A2M5V70I6M6K2ZJ8";    // ThingSpeak Write API Key
const int channelID = 2974161;                   // ThingSpeak Channel ID

const int irSensorPin = D6;   // Pin for IR sensor
const int buzzerPin = D5;     // Pin to control transistor connected to buzzer

RTC_DS1307 rtc;
WiFiClient client;

// ==================== SETUP ================================
void setup() {
  Serial.begin(115200);

  pinMode(irSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("❌ Couldn't find RTC module");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("✅ RTC is NOT running, setting time now...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected!");
  } else {
    Serial.println("\n❌ Failed to connect to WiFi!");
  }
}

// ==================== LOOP ================================
void loop() {
  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinute = now.minute();
  Serial.print("\n🕒 Current Time: ");
  Serial.print(currentHour);
  Serial.print(":");
  Serial.println(currentMinute);

  int scheduledHour = fetchFromThingSpeak(1);
  int scheduledMinute = fetchFromThingSpeak(2);

  Serial.print("📥 Scheduled Hour from ThingSpeak: ");
  Serial.println(scheduledHour);
  Serial.print("📥 Scheduled Minute from ThingSpeak: ");
  Serial.println(scheduledMinute);

  if (scheduledHour == currentHour && scheduledMinute == currentMinute) {
    int irValue = digitalRead(irSensorPin);
    Serial.print("📊 IR Sensor State: ");
    Serial.println(irValue);

    if (irValue == LOW) {
      Serial.println("✅ Medicine taken. Sending to ThingSpeak.");
      sendMedicineStatusToThingSpeak(1); // 1 = taken
    } else {
      Serial.println("⏰ Buzzing! Medicine not taken.");
      ringBuzzer(30); // 30 seconds
      sendMedicineStatusToThingSpeak(0); // 0 = not taken
    }
    delay(60000); // Wait a minute to prevent repeated triggering
  }

  delay(5000); // Check every 5 seconds
}

// ========== FUNCTION TO FETCH VALUES FROM THINGSPEAK ======
int fetchFromThingSpeak(int fieldNumber) {
  HTTPClient http;
  String url = "http://api.thingspeak.com/channels/" + String(channelID) + "/fields/" + String(fieldNumber) + "/last.txt?api_key=" + readAPIKey;
  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode == 200) {
    String payload = http.getString();
    http.end();
    return payload.toInt();
  } else {
    Serial.print("❌ Failed to fetch field ");
    Serial.print(fieldNumber);
    Serial.print(" from ThingSpeak. HTTP Code: ");
    Serial.println(httpCode);
    http.end();
    return -1;
  }
}

// ========== FUNCTION TO SEND STATUS TO THINGSPEAK =========
void sendMedicineStatusToThingSpeak(int status) {
  HTTPClient http;
  String writeURL = "http://api.thingspeak.com/update?api_key=" + writeAPIKey + "&field3=" + String(status);
  http.begin(client, writeURL);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.print("📤 ThingSpeak Response: ");
    Serial.println(httpCode);
  } else {
    Serial.println("❌ Failed to update ThingSpeak");
  }
  http.end();
}

// ========== FUNCTION TO RING BUZZER ======================
void ringBuzzer(int durationSeconds) {
  for (int i = 0; i < durationSeconds * 5; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}