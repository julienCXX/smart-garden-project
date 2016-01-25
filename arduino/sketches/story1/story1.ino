/*
 * Arduino sketch for user story #1
 * Reads earth moisture from a sensor and sends it to the serial bus
 * Reads water tank level from a sonar and sends it to the serial bus
 * Reads commands from the serial bus, to control a LED and a motor
 * Motor is mounted with a transistor and an external power source, like:
 * https://learn.adafruit.com/adafruit-arduino-lesson-13-dc-motors/transistors
 */

#include <DHT.h>

#define MOISTUREPIN A0 // The analog pin the earth moisture sensor is connected to
#define LEDPIN 13 // The digital pin the LED is connected to
#define MOTORPIN 5 // The digital pin the motor command is connected to
#define DHTPIN 6 // The digital pin the temperature and humidity sensor is connected to
#define SONAR_ECHOPIN 11 // The digital pin the sonar echo pin is connected to (PWM only)
#define SONAR_TRIGPIN 12 // The digital pin the sonar trig pin is connected to
#define DELAY 1000 // The main loop delay (ms)
#define PULSE_DURATION 500 // The motor pulse duration (ms)

// Choosing the kind of DHT sensor
#define DHTTYPE DHT11 // DHT 11

/*
 * The raw moisture sensor value under which moisture is 100%
 * May need to be calibrated
 */
#define MSENSOR_MIN 350
#define MSENSOR_MAX 1023 // The raw moisture sensor value over which moisture is 0%
/*
 * The raw sonar value under which water tank is full (100%)
 * May need to be calibrated
 */
#define SONAR_MIN 170
/*
 * The raw sonar value over which water tank is empty (0%)
 * May need to be calibrated
 */
#define SONAR_MAX 620

// Initializing DHT sensor
DHT dht(DHTPIN, DHTTYPE);

String inputString = "";
int currentDelay = DELAY;

const String LED_OFF = ">LED=OFF\n";
const String LED_ON = ">LED=ON\n";

const String MOTOR_OFF = ">MOTOR=OFF\n";
const String MOTOR_ON = ">MOTOR=ON\n";
const String MOTOR_PULSE = ">MOTOR=PULSE\n";


void setup() {
    Serial.begin(9600);
    dht.begin();
    pinMode(MOISTUREPIN, INPUT);
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);
    pinMode(MOTORPIN, OUTPUT);
    digitalWrite(MOTORPIN, LOW);
    pinMode(SONAR_TRIGPIN, OUTPUT);
    digitalWrite(SONAR_TRIGPIN, LOW);
    pinMode(SONAR_ECHOPIN, INPUT);
    inputString.reserve(64);
}

void loop() {
    delay(currentDelay);

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
    Serial.print((int) t);

    // Reading moisture and sending to the serial bus
    int moisture = analogRead(MOISTUREPIN);
    /*
     * Converting raw value from the sensor to percentage
     * (MSENSOR_MIN or lower -> 100%, MSENSOR_MAX -> 0%)
     * See: http://www.instructables.com/id/Arduino-LCD-Soil-Moisture-Sensor/?ALLSTEPS
     */
    moisture = constrain(moisture, MSENSOR_MIN, MSENSOR_MAX);
    moisture = map(moisture, MSENSOR_MIN, MSENSOR_MAX, 100, 0);
    Serial.print(" MOISTURE");
    Serial.print(moisture);

    // Reading water tank level and sending to the serial bus
    digitalWrite(SONAR_TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(SONAR_TRIGPIN, LOW);
    int distance = pulseIn(SONAR_ECHOPIN, HIGH);
    // Converting raw value from the sonar to water level percentage
    int waterLevel = constrain(distance, SONAR_MIN, SONAR_MAX);
    waterLevel = map(waterLevel, SONAR_MIN, SONAR_MAX, 100, 0);
    Serial.print(" WATER_LEVEL");
    Serial.println(waterLevel);

    // Reading commands from serial bus, to control the LED and the motor
    while (Serial.available()) {
        inputString += (char) Serial.read();
    }
    currentDelay = DELAY;
    if (LED_OFF == inputString) {
        digitalWrite(LEDPIN, LOW);
    } else if (LED_ON == inputString) {
        digitalWrite(LEDPIN, HIGH);
    } else if (MOTOR_OFF == inputString) {
        digitalWrite(MOTORPIN, LOW);
    } else if (MOTOR_ON == inputString) {
        digitalWrite(MOTORPIN, HIGH);
    } else if (MOTOR_PULSE == inputString) {
        digitalWrite(MOTORPIN, HIGH);
        delay(PULSE_DURATION);
        digitalWrite(MOTORPIN, LOW);
        currentDelay = DELAY - PULSE_DURATION;
    }
    inputString = "";
}
