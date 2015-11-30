/*
 * Arduino sketch for user story #1
 * Reads earth humidity from a sensor and sends it to the serial bus
 * Reads commands from the serial bus, to control a LED and a motor
 */

#define HUMIDITYPIN A0 // The analog pin the earth humidity sensor is connected to
#define LEDPIN 13 // The digital pin the LED is connected to
#define MOTORPIN 5 // The digital pin the motor command is connected to
#define DELAY 1000 // The main loop delay

String inputString = "";

const String LED_OFF = ">LED=OFF\n";
const String LED_ON = ">LED=ON\n";

const String MOTOR_OFF = ">MOTOR=OFF\n";
const String MOTOR_ON = ">MOTOR=ON\n";

void setup() {
    Serial.begin(9600);
    pinMode(HUMIDITYPIN ,INPUT);
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);
    pinMode(MOTORPIN, OUTPUT);
    digitalWrite(MOTORPIN, LOW);
    inputString.reserve(64);
}

void loop() {
    delay(DELAY);

    // Reading humidity and sending to the serial bus
    int humidity = analogRead(HUMIDITYPIN);
    Serial.print("HUMIDITY");
    Serial.println(humidity);

    // Reading commands from serial bus, to control the LED and the motor
    while (Serial.available()) {
      inputString += (char) Serial.read();
    }
    if (LED_OFF == inputString)
      digitalWrite(LEDPIN, LOW);
    if (LED_ON == inputString)
      digitalWrite(LEDPIN, HIGH);
     if (MOTOR_OFF == inputString)
      digitalWrite(MOTORPIN, LOW);
     if (MOTOR_ON == inputString)
      digitalWrite(MOTORPIN, HIGH);
    inputString = "";
}
