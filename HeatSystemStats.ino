// Heat System Stats
// Louis-Martin Caron 2021-03-06

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 6     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

long now = 180000;
long time0 = 0;
long time1 = 0;
long time2 = 0;
long time3 = 0;
long deltaTime1 = 0;
long deltaTime2 = 0;
long deltaTime3 = 0;
long heatTime = 0;
bool countState = LOW;

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  //Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  now = millis();

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //  // Compute heat index in Fahrenheit (the default)
  //  float hif = dht.computeHeatIndex(f, h);
  //  // Compute heat index in Celsius (isFahreheit = false)
  //  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity:");
  Serial.println(h, 1);
  Serial.print("Temperature:");
  Serial.println(t, 1);

  deltaTime3 = now - time3;
  if (deltaTime3 > 15000) {
    Serial.print("TemperatureGraph:");
    Serial.println(t, 1);
    time3 = now;
  }

  //temps d'attente avant prochain cycle de comptage pour laisser la temperature descendre sous 24
  deltaTime1 = now - time1;
  if (t > 24 && countState == LOW && deltaTime1 > 300000) {
    countState = HIGH;
    //reset timers 0 et 2
    time0 = now;
    time2 = now;
  }
  //temps d'attente avant de stopper la lecture et donner le resultat pour laisser monter a plus de 32
  deltaTime2 = now - time2;
  if (t < 32 && countState == HIGH && deltaTime2 > 135000) {
    heatTime = (now - time0) / 1000; //en secondes
    countState = LOW;
    //reset timer 1
    time1 = now;
    if (heatTime != 135) {
      Serial.print("time:");
      Serial.println(heatTime);
    }
  }
}
