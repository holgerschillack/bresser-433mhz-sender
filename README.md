## bresser-433mhz-sender

Arduino sketch for sending Temperature/Humidity data from self-made sensor to Bresser weather station

### Self-made sensor

##### Parts:
- Arduino Nano clone
- Generic 433 MHz Chip (with Vin, Gnd, Data, Antenna)
- DHT22 sensor

### Sketch

The sketch collects Temp/Hum data from the sensor, builds the 5 bytes for sending incl. checksum and broadcasts it every 30 seconds.
More explanations directly in the code.
