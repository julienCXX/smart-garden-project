/*
 * Arduino sketch for user story #1
 * Reads earth humidity from a sensor and sends it to the serial bus
 * Reads commands from the serial bus, to control a LED and a motor
 * Motor is mounted with a transistor and an external power source, like:
 * https://learn.adafruit.com/adafruit-arduino-lesson-13-dc-motors/transistors
 */

#define HUMIDITYPIN A0 // The analog pin the earth humidity sensor is connected to
#define LEDPIN 13 // The digital pin the LED is connected to
#define MOTORPIN 5 // The digital pin the motor command is connected to
#define DELAY 1000 // The main loop delay (ms)
#define PULSE_DURATION 500 // The motor pulse duration (ms)
/*
 * The raw humidity sensor value under which humidity is 100%
 * May need to be calibrated
 */
#define HSENSOR_MIN 350
#define HSENSOR_MAX 1023 // The raw humidity sensor value over which humidity is 0%

String inputString = "";
int currentDelay = DELAY;

const String LED_OFF = ">LED=OFF\n";
const String LED_ON = ">LED=ON\n";

const String MOTOR_OFF = ">MOTOR=OFF\n";
const String MOTOR_ON = ">MOTOR=ON\n";
const String MOTOR_PULSE = ">MOTOR=PULSE\n";


void setup() {
    Serial.begin(9600);
    pinMode(HUMIDITYPIN, INPUT);
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);
    pinMode(MOTORPIN, OUTPUT);
    digitalWrite(MOTORPIN, LOW);
    inputString.reserve(64);
}

void loop() {
    delay(currentDelay);

    // Reading humidity and sending to the serial bus
    int humidity = analogRead(HUMIDITYPIN);
    /*
     * Converting raw value from the sensor to percentage
     * (HSENSOR_MIN or lower -> 100%, HSENSOR_MAX -> 0%)
     * See: http://www.instructables.com/id/Arduino-LCD-Soil-Moisture-Sensor/?ALLSTEPS
     */
    humidity = constrain(humidity, HSENSOR_MIN, HSENSOR_MAX);
    humidity = map(humidity, HSENSOR_MIN, HSENSOR_MAX, 100, 0);
    Serial.print("HUMIDITY");
    Serial.println(humidity);

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
