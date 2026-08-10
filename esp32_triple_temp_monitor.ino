/*
  ESP32 Quad Temperature + Humidity Monitor
  - 1x DHT22   (temperature + humidity/moisture)
  - 3x DS18B20 (temperature, each sensor on its own dedicated OneWire pin)
  4 temperature readings total (1 DHT22 + 3 DS18B20) plus 1 humidity reading.
  Output: streamed over Bluetooth Serial (SPP), viewable in any
  Bluetooth serial terminal app on phone/PC.

  Wiring:
    DHT22 DATA     -> GPIO4  (with 10k pull-up to 3.3V if module has no pull-up)
    DS18B20_1 DATA -> GPIO5  (with 4.7k pull-up to 3.3V)
    DS18B20_2 DATA -> GPIO18 (with 4.7k pull-up to 3.3V)
    DS18B20_3 DATA -> GPIO19 (with 4.7k pull-up to 3.3V)
    All VCC -> 3.3V, all GND -> GND

  Libraries required (Arduino Library Manager):
    - DHT sensor library (Adafruit)
    - Adafruit Unified Sensor
    - OneWire
    - DallasTemperature
*/

#include <BluetoothSerial.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ---------- Pin config ----------
#define DHTPIN 4
#define DHTTYPE DHT22
#define ONEWIRE_PIN_1 5
#define ONEWIRE_PIN_2 18
#define ONEWIRE_PIN_3 19

// ---------- Bluetooth device name ----------
#define BT_DEVICE_NAME "ESP32_TempMonitor"

// ---------- Sample interval ----------
#define SAMPLE_INTERVAL_MS 2000

BluetoothSerial SerialBT;
DHT dht(DHTPIN, DHTTYPE);

OneWire oneWire1(ONEWIRE_PIN_1);
OneWire oneWire2(ONEWIRE_PIN_2);
OneWire oneWire3(ONEWIRE_PIN_3);
DallasTemperature ds18b20_1(&oneWire1);
DallasTemperature ds18b20_2(&oneWire2);
DallasTemperature ds18b20_3(&oneWire3);

bool ds1_present = false;
bool ds2_present = false;
bool ds3_present = false;
unsigned long lastSample = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(BT_DEVICE_NAME);

  dht.begin();
  ds18b20_1.begin();
  ds18b20_2.begin();
  ds18b20_3.begin();

  ds1_present = ds18b20_1.getDeviceCount() > 0;
  ds2_present = ds18b20_2.getDeviceCount() > 0;
  ds3_present = ds18b20_3.getDeviceCount() > 0;

  Serial.println("ESP32 Quad Temperature + Humidity Monitor started.");
  Serial.print("Bluetooth device name: ");
  Serial.println(BT_DEVICE_NAME);
  Serial.print("DS18B20_1 detected: ");
  Serial.println(ds1_present ? "yes" : "no");
  Serial.print("DS18B20_2 detected: ");
  Serial.println(ds2_present ? "yes" : "no");
  Serial.print("DS18B20_3 detected: ");
  Serial.println(ds3_present ? "yes" : "no");

  SerialBT.println("ESP32 Quad Temperature + Humidity Monitor");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSample < SAMPLE_INTERVAL_MS) return;
  lastSample = now;

  // ---- DHT22 read (temperature + humidity/moisture) ----
  float dhtTemp = dht.readTemperature();
  float dhtHum = dht.readHumidity();

  // ---- DS18B20 reads (independent buses) ----
  ds18b20_1.requestTemperatures();
  ds18b20_2.requestTemperatures();
  ds18b20_3.requestTemperatures();
  float ds1 = ds1_present ? ds18b20_1.getTempCByIndex(0) : NAN;
  float ds2 = ds2_present ? ds18b20_2.getTempCByIndex(0) : NAN;
  float ds3 = ds3_present ? ds18b20_3.getTempCByIndex(0) : NAN;

  // ---- Build output line ----
  String line = "";

  if (isnan(dhtTemp) || isnan(dhtHum)) {
    line += "DHT22: read error | ";
  } else {
    line += "DHT22: " + String(dhtTemp, 1) + " C, " + String(dhtHum, 1) + " %RH | ";
  }

  if (!ds1_present || ds1 == DEVICE_DISCONNECTED_C || isnan(ds1)) {
    line += "DS18B20_1: read error | ";
  } else {
    line += "DS18B20_1: " + String(ds1, 2) + " C | ";
  }

  if (!ds2_present || ds2 == DEVICE_DISCONNECTED_C || isnan(ds2)) {
    line += "DS18B20_2: read error | ";
  } else {
    line += "DS18B20_2: " + String(ds2, 2) + " C | ";
  }

  if (!ds3_present || ds3 == DEVICE_DISCONNECTED_C || isnan(ds3)) {
    line += "DS18B20_3: read error";
  } else {
    line += "DS18B20_3: " + String(ds3, 2) + " C";
  }

  Serial.println(line);
  SerialBT.println(line);
}
