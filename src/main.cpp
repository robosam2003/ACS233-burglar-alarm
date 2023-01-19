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

#define ALARM_FREQ 1000
#define ALARM_TIMEOUT 10000 // 15*60*1000 // Should be 15 minutes in the end // TODO



/// Global Variables
int64_t last_mag_switch_interrupt_time = 0; // in ms - used for debouncing
int64_t last_key_sensor_interrupt_time = 0; // in ms - used for debouncing


int64_t last_door_open_time = -100000; // in ms
int64_t last_door_close_time = -100000; // in ms
int64_t last_alarm_on_time = 0;


// Macros
#define timeSince_ms(event) millis() - event




int64_t security_timer = 0; // in ms

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

enum PIR_STATES {
    NO_PRESENCE = 0,
    PRESENCE = 1
};

enum AUTHORISATION_STATES {
    UNAUTHORISED = 0,
    AUTHORISED = 1
};

enum UNAUTHORISED_ENTRY_METHODS {
    DOOR_OPENED = 0,
    PIR_TRIGGERED = 1
};

// States
SYSTEM_MODES system_mode = SYSTEM_MODES::AT_HOME; // initialised to DISARMED // TODO

volatile LOCK_STATES lock_state = LOCK_STATES::LOCKED;
volatile DOOR_STATES door_state = DOOR_STATES::CLOSED;
volatile PIR_STATES pir_state = PIR_STATES::NO_PRESENCE;


/// ISRs
void magSwitchISR() {
    if (timeSince_ms(last_mag_switch_interrupt_time) > 100) {     // debounce the interrupt using millis()
        int state = digitalRead(MAG_SWITCH_PIN); // reading the door state after a change
        door_state = (DOOR_STATES) state;

        if (state == DOOR_STATES::CLOSED) {
            Serial.println("Door was closed");
            last_door_close_time = millis();
            if (lock_state == LOCK_STATES::UNLOCKED) {
                // If the door gets closed and the lock is unlocked, lock the door
                Serial.println("LOCKING DOOR1");
                lock_state = LOCK_STATES::LOCKED;
                digitalWrite(SOLENOID_PIN, LOW);
            }
        }
        else { // door_state == DOOR_STATES::OPEN
            Serial.println("Door was opened");
            last_door_open_time = millis();
        }
    }
    // update the last interrupt time
    last_mag_switch_interrupt_time = millis();
}

void keySensorISR() {
    if (timeSince_ms(last_key_sensor_interrupt_time) > 100) { // debounce the interrupt using millis()
        security_timer = millis(); // start the security timer
        Serial.println("UNLOCKING DOOR");
        digitalWrite(SOLENOID_PIN, HIGH); // Unlock door
        lock_state = LOCK_STATES::UNLOCKED;
        Serial.println("Lock state changed to: UNLOCKED");
    }
    last_key_sensor_interrupt_time = millis();
}

void PIRSensorISR() {
    // The PIR does not need debouncing, it goes High for 8 seconds when motion is detected
    pir_state = PIR_STATES::PRESENCE;
    Serial.println("PIR state changed to: PRESENCE");
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
    attachInterrupt(digitalPinToInterrupt(MAG_SWITCH_PIN), magSwitchISR, CHANGE);

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

    int entered = 0; // checks for user entry
    // authorized entry
    if (lock_state == LOCK_STATES::UNLOCKED) {
//        Serial.println("UNLOCKING DOOR"); - Now happening in the ISR
//        digitalWrite(SOLENOID_PIN, HIGH); // Unlock door
        while (timeSince_ms(security_timer) < 30000) {
            if (door_state == DOOR_STATES::CLOSED) {
                if ((timeSince_ms(security_timer) > 10000) && (lock_state == LOCK_STATES::UNLOCKED) ) { // only re-lock if the lock is unlocked
                    // Door will re-lock after 10 seconds if not opened
                    // (i.e. unlocked but not entered)
                    Serial.println("LOCKING DOOR2");
                    lock_state = LOCK_STATES::LOCKED;
                    digitalWrite(SOLENOID_PIN, LOW);
                    break;
                }
//                if ((timeSince_ms(last_door_open_time) < 10000) && (lock_state == LOCK_STATES::UNLOCKED)) {
//                    // Door will re-lock after it has been opened in the past 10 seconds
//                    // (i.e. opened then closed)             // Now happening in ISR
//                    Serial.println("LOCKING DOOR3");
//                    lock_state = LOCK_STATES::LOCKED;
//                    digitalWrite(SOLENOID_PIN, LOW);
//                }
            }

            // starts warning buzzer if door is open
            switch (system_mode) {
                case (SYSTEM_MODES::ARMED):
                case (SYSTEM_MODES::AT_HOME):
                    if (timeSince_ms(last_door_open_time) < 30000) {
                        beep(1000, 50, 500);  // starts warning buzzer if door is open
                    }
                    break;
                case (SYSTEM_MODES::DISARMED):
                default:
                    break;
            }
        }
//        if ((authorization == AUTHORISATION_STATES::UNAUTHORISED) && (timeSince_ms(last_door_open_time) < 15*60*1000)) {
//            alarmOn(); // alarm switching on
//        }
        delay(10);
    }


}

