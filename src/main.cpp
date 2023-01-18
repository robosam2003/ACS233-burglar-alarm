/*
 * A burglar alarm system
 *
 * This program will run the burglar alarm system.
 * The program will monitor door states via reed switch
 * The program will monitor button presses
 * the program will monitor human presence via pir
 *
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

uint64_t security_timer = 0; // in ms


/// Constants and enums
// Enums to make the code more readable
enum SYSTEM_MODES {
    ARMED,
    DISARMED,
    AT_HOME
};

enum DOOR_STATES {
    CLOSED = 0, // pin reads 0 when closed, 1 when open
    OPEN = 1
};

enum LOCK_STATES {
    LOCKED = 0,
    UNLOCKED = 1
};

// States
volatile LOCK_STATES lock_state = LOCK_STATES::LOCKED;
volatile DOOR_STATES door_state = DOOR_STATES::CLOSED;


/// ISRs
void magSwitchISR() {
    // debounce the interrupt using millis()
    // if the interrupt is triggered within 100ms of the last interrupt, ignore it
    // otherwise, set the alarm off
    if (millis() - last_mag_switch_interrupt_time > 100) {
        int state = digitalRead(MAG_SWITCH_PIN);
        door_state = (DOOR_STATES) state;
    }
    // update the last interrupt time
    last_mag_switch_interrupt_time = millis();
}

void keySensorISR() {
    if (millis() - last_key_sensor_interrupt_time > 100) { // debounce the interrupt using millis()
        security_timer = millis();
        lock_state = LOCK_STATES::UNLOCKED;
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
    pinMode(MAG_SWITCH_PIN, INPUT_PULLUP); // REED
    attachInterrupt(digitalPinToInterrupt(MAG_SWITCH_PIN), magSwitchISR, RISING); // TODO consider change to RISING (door opening??? TODO )

    /* Key sensor setup - We are using a push-button to simulate a key sensor.
     */
    pinMode(KEY_SENSOR_PIN, INPUT_PULLUP); // BUTTON
    attachInterrupt(digitalPinToInterrupt(KEY_SENSOR_PIN), keySensorISR, FALLING); // Trigger interrupt on falling edge (HIGH to LOW)

    /* Solenoid setup */
    pinMode(SOLENOID_PIN, OUTPUT);

    /* Buzzer setup */
    pinMode(BUZZER_PIN, OUTPUT);

    /* PIR setup */
    pinMode(PIR_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), PIRSensorISR, RISING); // Trigger interrupt on change (LOW to HIGH or HIGH to LOW)
}

void loop() {

    int enter = 0; // temp variable that confirms user entry

    if (lock_state == LOCK_STATES::UNLOCKED) {
        // Unlock door
        digitalWrite(SOLENOID_PIN, HIGH);

        // starts  warning buzzer
        while (millis() - security_timer < 30000) {
            if ((door_state = DOOR_STATES::CLOSED)) {
                if (millis() - security_timer > 10000) {
                    lock_state = LOCK_STATES::LOCKED;
                    digitalWrite(SOLENOID_PIN, LOW);
                    break;
                }
            }
            if ((door_state = DOOR_STATES::OPEN)){
                tone(BUZZER_PIN, 1000);
                delay(100);
                noTone(BUZZER_PIN);
            }




            delay(1000);
            if (millis() - security_timer > 10000) {
                lock_state = LOCK_STATES::LOCKED;
                digitalWrite(SOLENOID_PIN, LOW);
            }

        }
    }



}

