# cold storage monitoring system

Reads temperature and humidity from four sensors on an ESP32 and streams
the readings over Bluetooth Serial (SPP) so they can be viewed in any
Bluetooth serial terminal app.

Sensors:
- 1x DHT22 (temperature + humidity/moisture)
- 3x DS18B20 (temperature only, each on its own dedicated OneWire pin)

Total: 4 temperature readings (1 from DHT22 + 3 from DS18B20) plus
1 humidity/moisture reading (from DHT22).

## Hardware

| Signal          | ESP32 Pin | Notes                                  |
|------------------|-----------|-----------------------------------------|
| DHT22 DATA       | GPIO4     | 10k pull-up to 3.3V if module lacks one |
| DS18B20_1 DATA   | GPIO5     | 4.7k pull-up to 3.3V                    |
| DS18B20_2 DATA   | GPIO18    | 4.7k pull-up to 3.3V                    |
| DS18B20_3 DATA   | GPIO19    | 4.7k pull-up to 3.3V                    |
| All VCC          | 3.3V      |                                          |
| All GND          | GND       |                                          |

Each DS18B20 is wired on its own dedicated OneWire pin, so no address
lookup or bus-sharing logic is needed — each sensor is read directly from
its own bus.

## Libraries

Install via Arduino IDE Library Manager:
- DHT sensor library (Adafruit)
- Adafruit Unified Sensor
- OneWire
- DallasTemperature

BluetoothSerial is bundled with the ESP32 Arduino core, no separate install
needed.

## Build and flash

1. Open `src/esp32_triple_temp_monitor.ino` in Arduino IDE.
2. Select an ESP32 board under Tools > Board.
3. Select the correct COM port.
4. Upload.

## Reading the data

1. On your phone or PC, pair with Bluetooth device `ESP32_TempMonitor`.
2. Open a Bluetooth serial terminal app (e.g. Serial Bluetooth Terminal on
   Android, or a PC terminal over the paired virtual COM port).
3. Connect at any baud rate — SPP ignores baud, framing is handled by the
   Bluetooth stack.
4. Readings print every 2 seconds in the form:

```
DHT22: 26.4 C, 58.2 %RH | DS18B20_1: 26.10 C | DS18B20_2: 25.95 C | DS18B20_3: 26.02 C
```

Readings are also mirrored to the USB serial console at 115200 baud for
debugging without Bluetooth.

## Configuration

Edit the `#define` block at the top of the sketch to change:
- `DHTPIN` / `ONEWIRE_PIN_1` / `ONEWIRE_PIN_2` / `ONEWIRE_PIN_3` — GPIO assignments
- `BT_DEVICE_NAME` — Bluetooth device name shown when pairing
- `SAMPLE_INTERVAL_MS` — time between readings

## License

MIT, see LICENSE.
## IT is helpfull in various sectors like the vesitable truck food storage we can build a portable version by adding a lipo battery since lipo battery have a headroom of working in -30 degree selsious.
it can be used in food delhivery 
and in various sectors 
