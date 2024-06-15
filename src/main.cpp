#include <Arduino.h>
#include <DHT.h>
#include <NanitLib.h>

DHT dht(P4_2, DHT11);

#define RED P4_3
#define GRN P4_4

int grn_val = 100;
int red_val = 0;

float getIntervalDeviation (float min_v, float max_v, float value) {
  float width = max_v - min_v;
  if (value >= min_v && value <= max_v) {
    return 0;
  }
  if (value < min_v) {
    return (value - min_v) / width;
  }
  if (value > max_v) {
    return (value - max_v) / width;
  }
  return 0;
}

void setup() {
  Nanit_Base_Start();
  pinMode(RED, OUTPUT);
  pinMode(GRN, OUTPUT);
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  float t = dht.readTemperature(false);
  float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) {
    Serial.println("Could not read the values");
    return;
  }

  float overall = (abs(getIntervalDeviation(20, 24, t)) + abs(getIntervalDeviation(45, 55, h)))/2;

  float deviation = min(overall * 100.0, 100);
  grn_val = 100 - deviation;
  red_val = 0 + deviation;

  // Serial.println(String(abs(overall)) + " + " + String(deviation) + " + " + String(map(grn_val, 0, 100, 0, 255)) + " + " + String(map(red_val, 0, 100, 0, 255)));

  analogWrite(GRN, map(grn_val, 0, 100, 0, 255));
  analogWrite(RED, map(red_val, 0, 100, 0, 255));
  
  Serial.println(String(getIntervalDeviation(20, 24, t)) + "t + "  + String(getIntervalDeviation(45, 55, h)) + "h");
  Serial.println();
  
}