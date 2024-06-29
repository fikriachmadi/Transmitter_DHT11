#include <LoRa_E32.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT.h>

#define LED 2
#define RXp2 16
#define TXp2 17
#define mosfet 4

#define DHTPIN 5
#define DHTTYPE DHT11

#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  3600

DHT dht(DHTPIN, DHTTYPE);

RTC_DATA_ATTR int bootCount = 0;

void kirimData(){
  String data;

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature : ");
  Serial.println(t);
  Serial.print("Humidity : ");
  Serial.println(h);

  data = String(t) + "#" + String(h);
  data.trim();
  Serial.println(data);
  Serial2.print(data);
}

void setup() {
  // put your setup code here, to run once:
  pinMode (LED, OUTPUT);
  pinMode (mosfet, OUTPUT);
  Serial.begin(115200);
  dht.begin();
  digitalWrite (mosfet, HIGH);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  if (bootCount == 0)
  {
   delay(3000);
   kirimData();
  }
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
}

void loop() { 
  digitalWrite(LED, HIGH);
  String minta = "";
  
  while(Serial2.available()>0)
  {
    minta = Serial2.readString();
  }
  minta.trim();
  if(minta == "Ya")
  {
    delay(3000);
    kirimData();
    delay(1000);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start();  
  }
  minta = "";
  delay(1000);
  kirimData();
}