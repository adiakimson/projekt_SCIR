#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

//API key EH0BF82NJUDGZVA0
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

// Parametry sieci WiFi
const char* ssid = "motog";
const char* password = "karo101#";
WiFiClient client;

// Parametry ThingSpeak
unsigned long channelID1 = 2342975; // Zastąp swoim numerem kanału ThingSpeak dla pierwszego akcelerometru
const char *writeAPIKey1 = "EH0BF82NJUDGZVA0"; // Zastąp kluczem API dla pierwszego akcelerometru

unsigned long channelID2 =  2369656; // Zastąp swoim numerem kanału ThingSpeak dla drugiego akcelerometru
const char *writeAPIKey2 = "QJO4Y62XBRVOPFPC"; // Zastąp kluczem API dla drugiego akcelerometru

// Inicjalizacja obiektów ADXL345
Adafruit_ADXL345_Unified accel1 = Adafruit_ADXL345_Unified(0); // ADXL345 1
Adafruit_ADXL345_Unified accel2 = Adafruit_ADXL345_Unified(1); // ADXL345 2

void setup() {
  Serial.begin(115200);

  // Inicjalizacja połączenia WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Inicjalizacja ThingSpeak
  ThingSpeak.begin(client);

  // Inicjalizacja czujników ADXL345
  if(!accel1.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor 1, check wiring!");
    while (1);
  }
  if(!accel2.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor 2, check wiring!");
    while (1);
  }
}

void loop() {
  sensors_event_t event1, event2;
  accel1.getEvent(&event1);
  accel2.getEvent(&event2);

  // Odczyt współrzędnych z czujników ADXL345
  float x1 = event1.acceleration.x;
  float y1 = event1.acceleration.y;
  float z1 = event1.acceleration.z;

  float x2 = event2.acceleration.x;
  float y2 = event2.acceleration.y;
  float z2 = event2.acceleration.z;

  // Wyświetlanie odczytów w konsoli (opcjonalne)
  Serial.print("ADXL345 1 - X: "); Serial.print(x1); Serial.print(", Y: "); Serial.print(y1); Serial.print(", Z: "); Serial.println(z1);
  Serial.print("ADXL345 2 - X: "); Serial.print(x2); Serial.print(", Y: "); Serial.print(y2); Serial.print(", Z: "); Serial.println(z2);

  // Przesyłanie danych do ThingSpeak dla pierwszego akcelerometru
  ThingSpeak.setField(1, x1);
  ThingSpeak.setField(2, y1);
  ThingSpeak.setField(3, z1);

  int response1 = ThingSpeak.writeFields(channelID1, writeAPIKey1);
  if (response1 == 200) {
    Serial.println("Data from ADXL345 1 sent to ThingSpeak successfully!");
  } else {
    Serial.println("Error sending data from ADXL345 1 to ThingSpeak. HTTP error code " + String(response1));
  }

  // Przesyłanie danych do ThingSpeak dla drugiego akcelerometru
  ThingSpeak.setField(1, x2);
  ThingSpeak.setField(2, y2);
  ThingSpeak.setField(3, z2);

  int response2 = ThingSpeak.writeFields(channelID2, writeAPIKey2);
  if (response2 == 200) {
    Serial.println("Data from ADXL345 2 sent to ThingSpeak successfully!");
  } else {
    Serial.println("Error sending data from ADXL345 2 to ThingSpeak. HTTP error code " + String(response2));
  }

  delay(5000); // Odczyt i przesyłanie danych co 15 sekund (można dostosować)
}