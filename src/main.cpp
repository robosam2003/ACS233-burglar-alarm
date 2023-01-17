/*
 * A burglar alarm system
 *
 * // TODO: Add a description of the project
 *
 * Authors: Samuel Scott, Eurico Benedict, Simon Desir, Alex Hicks, Yu-ta Chou
 */


// Have to include "Arduino.h" because we are using PlatformIO build system.
#include <Arduino.h>



/// Definitions
#define MAG_SWITCH_PIN 2 // attached to interrupt
#define KEY_SENSOR_PIN 3 // attached to interrupt -
#define SOLENOID_PIN 12 // may be changed -

#define BUZZER_PIN 7 //
#define PIR_PIN 6


/// Global Variables
uint64_t last_mag_switch_interrupt_time = 0; // in ms
uint64_t last_key_sensor_interrupt_time = 0; // in ms

/// Constants and enums
enum SYSTEM_MODES {
    ARMED,
    DISARMED,
    AT_HOME
};

enum DOOR_STATES {
    CLOSED = 0,
    OPEN = 1
};






/// ISRs
void magSwitchISR() {
    // debounce the interrupt using millis()
    // if the interrupt is triggered within 100ms of the last interrupt, ignore it
    // otherwise, set the alarm off
    if (millis() - last_mag_switch_interrupt_time > 100) {
        int state = digitalRead(MAG_SWITCH_PIN);
        delay(1000);
        Serial.print("Magnetic switch triggered, state:  ");
        Serial.println(state);
    }
    // update the last interrupt time
    last_mag_switch_interrupt_time = millis();
}

void keySensorISR() {
    // debounce the interrupt using millis()
    // if the interrupt is triggered within 100ms of the last interrupt, ignore it
    // otherwise, set the alarm off
    if (millis() - last_key_sensor_interrupt_time > 100) {
        int state = digitalRead(KEY_SENSOR_PIN);
        delay(1000);
        Serial.print("Key sensor triggered, state:  ");
        Serial.println(state);
    }
    last_key_sensor_interrupt_time = millis();
}

void PIRSensorISR() {
    Serial.println("PIR sensor triggered! ");
}

/// Functions

/// Setup and loop
void setup() {
    // Set up Serial port (USB) on 9600 baud
    Serial.begin(9600);

    /* Magnetic switch setup - Resistance of sensor goes very high when door is OPEN, and very low when door is CLOSED.
     * Internal pull-up resistor means that when door is closed, the pin will read HIGH, and when door is open, the pin will read LOW.
     */
    pinMode(MAG_SWITCH_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MAG_SWITCH_PIN), magSwitchISR, CHANGE); // Trigger interrupt on change (LOW to HIGH or HIGH to LOW)

    /* Key sensor setup - We are using a push-button to simulate a key sensor.
     */
    pinMode(KEY_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(KEY_SENSOR_PIN), keySensorISR, FALLING); // Trigger interrupt on falling edge (HIGH to LOW)

    /* Solenoid setup */
    pinMode(SOLENOID_PIN, OUTPUT);

    /* Buzzer setup */
    pinMode(BUZZER_PIN, OUTPUT);

    /* PIR setup */
    pinMode(PIR_PIN, INPUT);
//    attachInterrupt(digitalPinToInterrupt(PIR_PIN), PIRSensorISR, RISING); // Trigger interrupt on change (LOW to HIGH or HIGH to LOW)
}

void loop() {
    int PIR_state = digitalRead(PIR_PIN);
    Serial.print("PIR state: ");
    Serial.println(PIR_state);

    delay(100);
}

