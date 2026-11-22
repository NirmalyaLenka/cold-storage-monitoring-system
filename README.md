# ESP32 Cold-Storage Temperature and Humidity Monitor

An ESP32 prototype that reads one DHT22 temperature/humidity sensor and three DS18B20 temperature probes, then streams the readings over Bluetooth Classic Serial Port Profile (SPP). The same readings are printed to the USB serial console for setup and troubleshooting.

> **Prototype status:** This repository provides sensor monitoring only. It is not a validated cold-chain logger or safety alarm, and it does not control refrigeration, store readings to a file, or send threshold alerts.

## Features

- Four temperature values: one from the DHT22 and one from each of three DS18B20 probes.
- One relative-humidity value from the DHT22.
- Separate OneWire data pins for the three DS18B20 sensors.
- Bluetooth SPP output under the device name `ESP32_TempMonitor`.
- USB serial diagnostics at 115200 baud.
- A configurable 2-second sampling interval and sensor pin definitions near the top of the sketch.

## Hardware and wiring

Use an **original ESP32-WROOM-32 / ESP32 DevKit board with Bluetooth Classic support**. Not every ESP32-family board supports Bluetooth SPP.

| Signal | ESP32 connection | Notes |
|---|---|---|
| DHT22 DATA | GPIO4 | Add a 10 kΩ pull-up to 3.3 V if the sensor module does not already include one |
| DS18B20 #1 DATA | GPIO5 | 4.7 kΩ pull-up to 3.3 V |
| DS18B20 #2 DATA | GPIO18 | 4.7 kΩ pull-up to 3.3 V |
| DS18B20 #3 DATA | GPIO19 | 4.7 kΩ pull-up to 3.3 V |
| Sensor VCC | 3.3 V | Confirm the breakout/probe wiring before powering |
| Sensor GND | ESP32 GND | Common ground |

Each DS18B20 has its own OneWire bus in this sketch. Do not combine their data lines unless you also change the firmware.

## Prerequisites

- Arduino IDE with the Espressif ESP32 board package installed.
- An ESP32 board selected that supports Bluetooth Classic SPP.
- Arduino libraries installed through Library Manager:
  - **DHT sensor library** by Adafruit
  - **Adafruit Unified Sensor**
  - **OneWire**
  - **DallasTemperature**

`BluetoothSerial` is provided by the ESP32 Arduino core; it is not a separate Library Manager dependency.

## Build and upload

1. Connect the ESP32 over USB.
2. Open the root-level sketch **[`esp32_triple_temp_monitor.ino`](esp32_triple_temp_monitor.ino)**. There is no `src/` directory in this repository.
3. In Arduino IDE, select the appropriate ESP32 board and its serial port.
4. Install the libraries listed above, if not already installed.
5. Compile and upload the sketch.
6. Open **Tools → Serial Monitor** at **115200 baud**. Startup output reports the Bluetooth name and whether each DS18B20 bus was detected.

Before connecting sensors, verify the pinout of each module and avoid shorting 3.3 V to ground.

## Read the measurements

1. Pair a phone or computer with **`ESP32_TempMonitor`** using a Bluetooth Classic serial terminal.
2. Connect to the paired serial service. The Bluetooth SPP link does not use the terminal's baud-rate setting in the same way as a physical UART.
3. Readings are emitted about every two seconds. Example format:

```text
DHT22: 26.4 C, 58.2 %RH | DS18B20_1: 26.10 C | DS18B20_2: 25.95 C | DS18B20_3: 26.02 C
```

The numbers above are an illustration of the output format, not a measured result from this repository. USB serial prints the same line.

## Configuration

Edit the definitions near the top of `esp32_triple_temp_monitor.ino`:

- `DHTPIN` and `DHTTYPE` — DHT sensor connection/type.
- `ONEWIRE_PIN_1`, `ONEWIRE_PIN_2`, `ONEWIRE_PIN_3` — individual DS18B20 data pins.
- `BT_DEVICE_NAME` — advertised Bluetooth device name.
- `SAMPLE_INTERVAL_MS` — interval between readings.

Changing pin definitions requires updating the physical wiring to match.

## Troubleshooting

| Symptom | Checks |
|---|---|
| Bluetooth device does not appear | Confirm the board supports Bluetooth Classic, the upload succeeded, and the sketch started without errors |
| A DS18B20 reports a read error | Check its dedicated DATA pin, ground, power, and 4.7 kΩ pull-up; restart after correcting the wiring |
| DHT22 reports a read error | Check GPIO4, sensor power/ground, and the module's pull-up |
| Serial output is unreadable | Set the USB Serial Monitor to 115200 baud |
| Readings appear plausible but inaccurate | Compare sensors with a suitable reference; this sketch does not implement a calibration routine |

## Known limitations and safe use

- No data logging, timestamps, dashboard, refrigeration control, or threshold alarm is implemented.
- Sensor detection and readings are not a substitute for periodic calibration or independent monitoring.
- This project has not been validated for regulated food, medicine, or other cold-chain use. Validate the complete sensor placement, measurement accuracy, failure response, and alarm system before relying on it.
- Portable battery operation is not implemented. Do not assume a Li-ion/LiPo cell is safe to charge or operate at a claimed sub-zero temperature; follow the exact cell manufacturer's operating and charging limits and provide appropriate protection and regulation.
- The root-level file named `download` contains ignore-pattern text, not a download script. Because it is not named `.gitignore`, Git will not treat it as the repository's standard ignore file.

## Repository files

| Path | Purpose |
|---|---|
| [`esp32_triple_temp_monitor.ino`](esp32_triple_temp_monitor.ino) | Main ESP32 sketch |
| [`README.md`](README.md) | Setup, wiring, and limitations |
| [`LICENSE`](LICENSE) | MIT license text |
| `download` | Text file containing ignore-pattern lines |

## License

MIT; see [`LICENSE`](LICENSE).

---

## Maintenance & Audit

- **Audit date:** 11 November 2026
- **Documentation status:** README reviewed and audit record added on this date.
- **Scope and evidence:** This is a documentation-level maintenance pass. It does not establish that source code compiles, dependencies are current, security has been tested, or hardware/runtime behavior has been verified.
- **Code and tests:** No new functional code changes or test results are claimed by this README update. Run project-specific build, test, and hardware/runtime checks before relying on the project.
- **Maintenance baseline:** Documentation status recorded through 11 November 2026; revisit setup instructions, dependencies, safety limits, known limitations, and tests when the project changes.
