/*

Used Bresser Protocol from https://github.com/merbanan/rtl_433_tests/tree/master/tests/bresser_3ch
Replaces this sensor: https://www.bresser.de/Wetter-Zeit/BRESSER-Thermo-Hygro-Sensor-3CH-passend-fuer-BRESSER-Thermo-Hygrometer.html
Tested with DHT22 Sensor and generic 433Mhz chip on an Arduino Nano clone
Library for DHT22 sensor: https://github.com/adafruit/DHT-sensor-library

*/

#include <DHT.h>
#include <DHT_U.h>

byte SEND_PIN = 3;

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int periodusec = 250;
byte repeats = 15;
byte randomId = 252;
int sendPeriod = 30000;

#define DEBUG false

void setup()
{
  pinMode(SEND_PIN, OUTPUT);

  if (DEBUG)
  {
    Serial.begin(9600);
    Serial.println(F("DHT22 Test!"));
  }

  dht.begin();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (DEBUG)
  {
    if (isnan(h) || isnan(t) || isnan(f))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.println();
  }

  int Temperature = (int)((f + 90) * 10);
  int Humidity = (int)h;

  sendData(Temperature, Humidity);

  delay(sendPeriod);
}

void sendData(int Temperature, int Humidity)
{
  for (int i = repeats; i >= 0; i--)
  {
    // Send Preambel Bits
    sendPraBits();

    // Calculate Checksum
    int checkSum = 0;
    checkSum += randomId;

    int byte2 = B0001;
    byte2 = byte2 << 4;
    byte2 = byte2 | (Temperature >> 8);
    checkSum += byte2;

    int byte3 = Temperature & 0xFF;
    checkSum += byte3;

    checkSum += Humidity;

    // Send Address
    send8Bit(randomId);

    // Battery 0 = good
    // Sync/Test Bit 0 = OK
    // Channel 1=01, 2=10, 3=11
    send4Bit(B0001);

    // Send Temperature
    sendTemp(Temperature);

    // Send Humidity
    send8Bit(Humidity);

    // Send Checksum
    send8Bit(checkSum % 256);
  }
}

void send8Bit(int address)
{
  for (int i = 7; i >= 0; i--)
  {
    boolean curBit = address & 1 << i;
    sendBit(curBit);
  }
}

void send4Bit(int address)
{
  for (int i = 3; i >= 0; i--)
  {
    boolean curBit = address & 1 << i;
    sendBit(curBit);
  }
}

void sendTemp(int address)
{
  for (int i = 11; i >= 0; i--)
  {
    boolean curBit = address & 1 << i;
    sendBit(curBit);
  }
}

void sendBit(boolean isBitOne)
{
  if (isBitOne)
  {
    // Send '1'
    digitalWrite(SEND_PIN, HIGH);
    delayMicroseconds(periodusec * 2);
    digitalWrite(SEND_PIN, LOW);
    delayMicroseconds(periodusec);
  }
  else
  {
    // Send '0'
    digitalWrite(SEND_PIN, HIGH);
    delayMicroseconds(periodusec);
    digitalWrite(SEND_PIN, LOW);
    delayMicroseconds(periodusec * 2);
  }
}

void sendPraBits()
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(SEND_PIN, HIGH);
    delayMicroseconds(750);
    digitalWrite(SEND_PIN, LOW);
    delayMicroseconds(750);
  }
}