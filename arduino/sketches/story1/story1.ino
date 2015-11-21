/*
 * Arduino sketch for user story #1
 * Reads humidity and temperature from a DHT sensor and sends it to the serial
 * bus
 * Reads commands from the serial bus, to control a LED
 * Adapted from the code bundled with the DHT library, by ladyada, public domain
 */

#include <DHT.h>

#define DHTPIN 5 // The digital pin the DHT sensor is connected to
#define LEDPIN 13 // The digital pin the LED is connected to
#define DELAY 1000 // The main loop delay

// Choosing the kind of DHT sensor
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)

// Initializing DHT sensor
DHT dht(DHTPIN, DHTTYPE);

String inputString = "";

const String LED_OFF = ">LED=OFF\n";
const String LED_ON = ">LED=ON\n";

void setup() {
    Serial.begin(9600);
    dht.begin();
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);
    inputString.reserve(64);
}

void loop() {
    delay(DELAY);

    // Reading humidity and temperature (with error checking)
    float h = dht.readHumidity(true); // Force reading
    float t = dht.readTemperature(false, true); // In °C, force reading
    if (isnan(h) || isnan(t)) {
      Serial.println("E_DHT_READ"); // Sensor reading failed
      return;
    }

    // Sending read values to serial bus
    Serial.print("HUMIDITY");
    Serial.print((int) h);
    Serial.print(" TEMPERATURE");
    Serial.println((int) t);

    // Reading commands from serial bus, to control the LED
    while (Serial.available()) {
      inputString += (char) Serial.read();
    }
    if (LED_OFF == inputString)
      digitalWrite(LEDPIN, LOW);
    if (LED_ON == inputString)
      digitalWrite(LEDPIN, HIGH);
    inputString = "";
}

