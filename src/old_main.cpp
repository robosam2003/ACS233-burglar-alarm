///*
// * A burglar alarm system (GROUP 4) 23 Jan 2023
// * Authors: Samuel Scott, Eurico Benedict, Simon Desir, Alex Hicks, Yu-ta Chou
// *
// * This program is the arduino end to a two piece inter-working software that runs
// * the burglar alarm system. It functions as follows, it monitors sensor inputs and reacts
// * accordingly.
// *
// * Input devices are listed as follows;
// * PIR Sensor - used to detect motion
// * Magnetic Reed Sensor - used to detect door state (open/closed)
// * Button - used to detect unlocking of the door
// * Python serial stream - Used for user authorisation confirmation and mode change input
// *
// * Output devices are listed as follows;
// * Piezo buzzer - used as a warning or alarm sound output
// * Solenoid - used to lock and unlock door
// * LEDs - used as a status indicator
// * Python serial stream - Used for authorisation requests
// *
// * There are three modes programmed into the system and depending on the modes it is running on
// * the system behaves a different way.
// * ARMED - in armed mode all sensor input is taken into account, if PIR detects movement when the door is not unlocked
// * it will sound an alarm, if the reed sensor is activated without unlocking it will sound an alarm, if the user
// * fails to be authorised during the entry procedure it will sound an alarm.
// * AT_HOME - in at home mode the pir will not activate the alarm, everything else remains the same unauthorised entry
// * will still set off an alarm
// * DISARMED - in disarmed mode door will still lock but no authorisation will be needed, and no alarm will sound under
// * any conditions.
// *
// */
//
//
//// Have to include "Arduino.h" because we are using PlatformIO build system.
//#include <Arduino.h>
//
///// Definitions
//#define MAG_SWITCH_PIN 2 // attached to interrupt
//#define KEY_SENSOR_PIN 3 // attached to interrupt
//#define SOLENOID_PIN 12 // may be changed
//
//#define BUZZER_PIN 7 //
//#define PIR_PIN 19
//
//#define ALARM_FREQ 500
//#define INTERMITTENT_TIMEOUT 20000 // 30 seconds
//
//int64_t ALARM_TIMEOUT = 30000; // 15 minutes
//#define MODE_CHANGE_TIMEOUT 30000
//
//#define DISARMED_MODE_LED 47 // Red
//#define AT_HOME_MODE_LED 46 // Yellow
//#define ARMED_MODE_LED 49 // Green
//#define PIR_LED 42 // ITS RED
//
//#define ALARM_LED 48 // ITS RED
//
//// Macros
//#define timeSince_ms(event) millis() - event
//
///// Constants and enums
//// Enumerations to make the code more readable
//enum SYSTEM_MODES {
//    ARMED,
//    DISARMED,
//    AT_HOME
//};
//
//enum DOOR_STATES {
//    CLOSED = 0, // pin reads 0 when closed, 1 when open
//    OPEN = 1
//};
//
//enum LOCK_STATES {
//    LOCKED = 0,
//    UNLOCKED = 1
//};
//
//enum PIR_STATES {
//    NO_PRESENCE = 0,
//    PRESENCE = 1
//};
//
//enum AUTHORISATION_STATES {
//    UNAUTHORISED = 0,
//    AUTHORISED = 1
//};
//
//enum UNAUTHORISED_ENTRY_METHODS {
//    DOOR_OPENED = 0,
//    PIR_TRIGGERED = 1
//};
//
//enum VERIFICATION_WINDOW_STATES {
//    WINDOW_CLOSED = 0,
//    WINDOW_OPEN = 1
//};
//
//// States
//SYSTEM_MODES system_mode = SYSTEM_MODES::ARMED; // initialised to DISARMED // TODO
//
//volatile LOCK_STATES lock_state = LOCK_STATES::LOCKED;
//volatile DOOR_STATES door_state = DOOR_STATES::CLOSED;
//volatile PIR_STATES pir_state = PIR_STATES::NO_PRESENCE;
//
//AUTHORISATION_STATES authorization_state = AUTHORISATION_STATES::UNAUTHORISED; //id password authorization_state variable
//VERIFICATION_WINDOW_STATES verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_CLOSED;
//
///// Global Variables
//int64_t last_mag_switch_interrupt_time = 0; // in ms - used for debouncing
//int64_t last_key_sensor_interrupt_time = 0; // in ms - used for debouncing
//
//// Timers
//int64_t security_timer = -100000; // in ms
//int64_t last_door_open_time = -100000; // in ms
//int64_t last_door_close_time = -100000; // in ms
//int64_t last_alarm_on_time = 0;
//int64_t last_mode_change_time = -100000;
//
//
////ISR Prototypes
//void toArmedMode();
//void toDisarmedMode();
//void toAtHomeMode();
//
//// Sensor ISRs
//void keySensorISR();
//void magSwitchISR();
//void PIRSensorISR();
//
//
//void verifyUser();
//void verificationWindowOpen();
//void verificationWindowClosed();
//void changingMode();
//void LEDblink(int onDel,int offDel);
//void beep(int freq,int onDel,int offDel);
//void alarmOn();
//void alarmOff();
//void unauthorizedEntry(UNAUTHORISED_ENTRY_METHODS method);
//void authorizedEntry();
//
//// Setup and check_states
//void setup() {
//    // Set up Serial port (USB) on 9600 baud
//    Serial.begin(9600);
//
//    /* Magnetic switch setup - Resistance of sensor goes very high when door is OPEN, and very low when door is CLOSED.
//     * Internal pull-up resistor means that when door is closed, the pin will read HIGH, and when door is open, the pin will read LOW. */
//    pinMode(MAG_SWITCH_PIN, INPUT_PULLUP); // REED
//    attachInterrupt(digitalPinToInterrupt(MAG_SWITCH_PIN), magSwitchISR, CHANGE);
//
//    /* Key sensor setup - We are using a push-button to simulate a key sensor. */
//    pinMode(KEY_SENSOR_PIN, INPUT_PULLUP); // BUTTON
//    attachInterrupt(digitalPinToInterrupt(KEY_SENSOR_PIN), keySensorISR, FALLING); // Trigger interrupt on falling edge (HIGH to LOW)
//
//    /* Solenoid setup */
//    pinMode(SOLENOID_PIN, OUTPUT);
//
//    /* Buzzer setup */
//    pinMode(BUZZER_PIN, OUTPUT);
//
//    /* PIR setup */
//    pinMode(PIR_PIN, INPUT);
//    attachInterrupt(digitalPinToInterrupt(PIR_PIN), PIRSensorISR, CHANGE); // Trigger interrupt on change (LOW to HIGH or HIGH to LOW)
//
//    /* Alarm LED setup */
//    pinMode(ALARM_LED,OUTPUT);
//    pinMode(ARMED_MODE_LED,OUTPUT);
//    pinMode(AT_HOME_MODE_LED,OUTPUT);
//    pinMode(DISARMED_MODE_LED,OUTPUT);
//    pinMode(PIR_LED,OUTPUT);
//
//    toAtHomeMode(); // todo: change to disarmed mode
//    last_mode_change_time = -100000; // reset last mode change time so that system works instantly
//}
//
//void check_states() {
//    // Check for door state change
//    if (lock_state == LOCK_STATES::UNLOCKED) {
//        authorizedEntry();
//        delay(10);
//    }
//
//    // unauthorised entry
//    if (door_state == DOOR_STATES::OPEN) { // triggered by door opening without unlocking
//        unauthorizedEntry(UNAUTHORISED_ENTRY_METHODS::DOOR_OPENED);
//    }
//    if (pir_state == PIR_STATES::PRESENCE) { // triggered by PIR sensor
//        unauthorizedEntry(UNAUTHORISED_ENTRY_METHODS::PIR_TRIGGERED);
//    }
//
//    // for when the Alarm times out, we need to switch verification window states on python end.
//    if ((verification_window_state == VERIFICATION_WINDOW_STATES::WINDOW_OPEN) && (timeSince_ms(security_timer) > ALARM_TIMEOUT)) {
//        verificationWindowClosed();
//        toDisarmedMode();
//    }
//
//
//    if (verification_window_state == VERIFICATION_WINDOW_STATES::WINDOW_CLOSED) {
//        // Check for mode change, if verification window is closed (i.e. User is inside wanting to change mode before going out)
//        changingMode();
//    }
//
//    // Check for mode change key from python
//} // TODO: Add mode change time checks
//
//void toArmedMode() {
//    system_mode = SYSTEM_MODES::ARMED;
//    digitalWrite(DISARMED_MODE_LED, LOW);
//    digitalWrite(AT_HOME_MODE_LED, LOW);
//    digitalWrite(ARMED_MODE_LED, HIGH);
//    security_timer = -100000; // reset security timer in case mode changed inside intermittent timeout window
//    last_mode_change_time = millis();
//}
//
//void toAtHomeMode() {
//    system_mode = SYSTEM_MODES::AT_HOME;
//    digitalWrite(DISARMED_MODE_LED, LOW);
//    digitalWrite(ARMED_MODE_LED, LOW);
//    digitalWrite(AT_HOME_MODE_LED, HIGH);
//    security_timer = -100000; // reset security timer in case mode changed inside intermittent timeout window
//    last_mode_change_time = millis();
//}
//
//void toDisarmedMode() {
//    system_mode = SYSTEM_MODES::DISARMED;
//    digitalWrite(AT_HOME_MODE_LED, LOW);
//    digitalWrite(ARMED_MODE_LED, LOW);
//    digitalWrite(DISARMED_MODE_LED, HIGH);
//    security_timer = -100000; // reset security timer in case mode changed inside intermittent timeout window
//    last_mode_change_time = millis();
//}
//
//
///// ISRs
//void magSwitchISR() {
//    if (timeSince_ms(last_mag_switch_interrupt_time) > 100) {     // debounce the interrupt using millis()
//        int state = digitalRead(MAG_SWITCH_PIN); // reading the door state after a change
//        door_state = (DOOR_STATES) state;
//
//        if (state == DOOR_STATES::CLOSED) {
//            Serial.println("Door was closed");
//            last_door_close_time = millis();
//            if (lock_state == LOCK_STATES::UNLOCKED) {
//                // If the door gets closed and the lock is unlocked, lock the door
//                Serial.println("LOCKING DOOR1");
//                lock_state = LOCK_STATES::LOCKED;
//                digitalWrite(SOLENOID_PIN, LOW);
//            }
//        }
//        else { // door_state == DOOR_STATES::OPEN
//            Serial.println("Door was opened");
//            switch (system_mode) {
//                case (SYSTEM_MODES::ARMED):
//                case (SYSTEM_MODES::AT_HOME):
//                    verificationWindowOpen();
//                    break;
//                case (SYSTEM_MODES::DISARMED):
//                default:
//                    break;
//            }
//            last_door_open_time = millis();
//        }
//    }
//    // update the last interrupt time
//    last_mag_switch_interrupt_time = millis();
//}
//
//void keySensorISR() {
//    if (timeSince_ms(last_key_sensor_interrupt_time) > 100) { // debounce the interrupt using millis()
//        security_timer = millis(); // start the security timer
//        Serial.println("UNLOCKING DOOR");
//
//        // Unlock the door
//        digitalWrite(SOLENOID_PIN, HIGH); // Unlock door
//        lock_state = LOCK_STATES::UNLOCKED;
//
//        Serial.println("Lock state changed to: UNLOCKED");
//    }
//    last_key_sensor_interrupt_time = millis();
//}
//
//void PIRSensorISR() {
//    // The PIR does not need debouncing, it goes High for 8 seconds when motion is detected
//    pir_state = (PIR_STATES) digitalRead(PIR_PIN);
//    if (pir_state == PIR_STATES::PRESENCE) {
//        pir_state = PIR_STATES::PRESENCE;
//        Serial.println("PIR state changed to: PRESENCE");
//        digitalWrite(PIR_LED,HIGH);
//        switch (system_mode) {
//            case (SYSTEM_MODES::ARMED):
//                verificationWindowOpen();
//                break;
//            case (SYSTEM_MODES::AT_HOME):
//            case (SYSTEM_MODES::DISARMED):
//            default:
//                break;
//        }
//    }
//    else {
//        digitalWrite(PIR_LED,LOW);
//    }
//}
//
///// Functions
//void changingMode() {
//    if (Serial.available()) { // wait for user input
//        String message;
//        // Read the serial port until a newline
//        message = Serial.readStringUntil('\n');
//        Serial.println("(changingMode) Message received: " + message);
//
//        if (message == "MODE: ARMED") {
//            toArmedMode();
//        } else if (message == "MODE: AT-HOME") {
//            toAtHomeMode();
//        } else if (message == "MODE: DISARMED") {
//            toDisarmedMode();
//        }
//        verificationWindowClosed();
//        while(door_state == DOOR_STATES::OPEN); // wait until door is closed again
//    }
//}
//
//
//void verifyUser() {
//    if (verification_window_state == VERIFICATION_WINDOW_STATES::WINDOW_OPEN) {
//        Serial.println("VERIFICATION REQUEST");
//        String message;
//        while (!Serial.available()); // wait for user input
//        // Read the serial port until a newline
//        message = Serial.readStringUntil('\n');
//        Serial.println("(verifyUser) Message received: " + message);
//
//        if (message == "VERIFIED") {
//            authorization_state = AUTHORISATION_STATES::AUTHORISED;
//            Serial.println("I RECEIVED YOUR VERIFIED MESSAGE");
//            alarmOff();
//
//            while (!Serial.available()); // wait but only on this one
//            changingMode();
//            Serial.println("MODE CHANGED TO: " + (String) system_mode);
//            verificationWindowClosed();
//        }
//        else if (message == "VERIFICATION WINDOW CLOSED") { // This is for when the user enters the pin too many times.
//            verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_CLOSED;
//        }
//        else if (message == "VERIFICATION WINDOW OPEN") {
//            verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_OPEN;
//        }
//        else { // message is "NOT VERIFIED"
//            authorization_state = AUTHORISATION_STATES::UNAUTHORISED;
//            return;
//        }
//    }
//}
//
//void verificationWindowOpen() {
//    Serial.println("VERIFICATION WINDOW OPEN");
//    verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_OPEN;
//}
//
//void verificationWindowClosed() {
//    Serial.println("VERIFICATION WINDOW CLOSED");
//    verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_CLOSED;
//}
//
//void LEDblink(int onDel,int offDel) {
//    digitalWrite(ALARM_LED,HIGH);
//    delay(onDel);
//    digitalWrite(ALARM_LED,LOW);
//    delay(offDel);
//}
//
//void beep(int freq,int onDel,int offDel) {
//    tone(BUZZER_PIN, freq);
//    delay(onDel);
//    noTone(BUZZER_PIN);
//    delay(offDel);
//}
//
//void alarmOn() {
//    tone(BUZZER_PIN, ALARM_FREQ);
//    last_alarm_on_time = millis();
//}
//
//void alarmOff() {
//    noTone(BUZZER_PIN);
//    digitalWrite(BUZZER_PIN, LOW);
//}
//
//void unauthorizedEntry(UNAUTHORISED_ENTRY_METHODS method) {
//    switch (method) {
//        case DOOR_OPENED:
//            switch (system_mode) {
//                case (SYSTEM_MODES::ARMED):   // when cases don't have a break, they fall through to the next case (i.e. this means OR)
//                case (SYSTEM_MODES::AT_HOME):
//                    alarmOn(); // alarm switching on
//                    break;
//                case (SYSTEM_MODES::DISARMED):
//                    return;
//                default:
//                    break;
//            }
//
//            // Waiting for verified pin or 15 minutes elapsed
//            while ((authorization_state == AUTHORISATION_STATES::UNAUTHORISED) &&
//                    (timeSince_ms(last_alarm_on_time) < ALARM_TIMEOUT)) {
//                LEDblink(100,100);
//                verifyUser();
//                if (authorization_state == AUTHORISATION_STATES::AUTHORISED) {
//                    authorization_state = AUTHORISATION_STATES::UNAUTHORISED;
//                    last_door_open_time = -100000;
//                    break;
//                }
//            }
//            alarmOff(); // This is needed when the alarm times out
//            break;
//        case PIR_TRIGGERED:
//            if ((timeSince_ms(security_timer) < INTERMITTENT_TIMEOUT)) {
//                break;
//            } // if the security timer is less than 30 seconds, the alarm will not go off
//            else {
//                switch (system_mode) {
//                    case (SYSTEM_MODES::ARMED):   // when cases don't have a break, they fall through to the next case (i.e. this means OR)
//                        alarmOn(); // alarm switching on
//                        break;
//                    case (SYSTEM_MODES::AT_HOME):
//                    case (SYSTEM_MODES::DISARMED):
//                        return;
//                    default:
//                        break;
//                }
//                pir_state = PIR_STATES::NO_PRESENCE;
//
//
//                // Waiting for verified pin or 15 minutes elapsed
//                while (authorization_state == AUTHORISATION_STATES::UNAUTHORISED && timeSince_ms(last_alarm_on_time) < ALARM_TIMEOUT) {
//                    LEDblink(100,100);
//                    verifyUser();
//                    if (authorization_state == AUTHORISATION_STATES::AUTHORISED) {
//                        authorization_state = AUTHORISATION_STATES::UNAUTHORISED;
//                        last_door_open_time = -100000;
//                        break;
//                    }
//                }
//                alarmOff(); // This is needed when the alarm times out
//            }
//            break;
//    }
//}
//
//void authorizedEntry() {
//    // authorized entry
////        Serial.println("UNLOCKING DOOR"); - Now happening in the ISR
////        digitalWrite(SOLENOID_PIN, HIGH); // Unlock door
//    while ((timeSince_ms(security_timer) < INTERMITTENT_TIMEOUT) && (authorization_state == AUTHORISATION_STATES::UNAUTHORISED)) {
//        if (door_state == DOOR_STATES::CLOSED) {
//            if ((timeSince_ms(security_timer) > 10000) && (lock_state == LOCK_STATES::UNLOCKED)) { // only re-lock if the lock is unlocked
//                // Door will re-lock after 10 seconds if not opened
//                // (i.e. unlocked but not entered)
//                Serial.println("LOCKING DOOR2");
//                lock_state = LOCK_STATES::LOCKED;
//                digitalWrite(SOLENOID_PIN, LOW);
//                return;
//            }
//        }
//
//        // starts warning buzzer if door is open
//        switch (system_mode) {
//            case (SYSTEM_MODES::ARMED):
//            case (SYSTEM_MODES::AT_HOME):
//                if (timeSince_ms(last_door_open_time) < INTERMITTENT_TIMEOUT) {
//                    beep(1000, 50, 500);  // starts warning buzzer if door is open
//                    verifyUser();
//                    if (authorization_state == AUTHORISATION_STATES::AUTHORISED) {
//                        authorization_state = AUTHORISATION_STATES::UNAUTHORISED;
//                        last_door_open_time = -100000; // reset
//                        return;
//                    }
//                }
//                break;
//            case (SYSTEM_MODES::DISARMED):
//            default:
//                return;
//        }
//    }
//
//    while ((authorization_state == AUTHORISATION_STATES::UNAUTHORISED) && (timeSince_ms(last_door_open_time) < ALARM_TIMEOUT)) {
//        Serial.println("Alarming from authorised entry");
//        alarmOn(); // alarm switching on
//        LEDblink(100,100);
//        verifyUser();
//    }
//    alarmOff(); // alarm switching off
//    last_door_open_time = -100000;
//    authorization_state = AUTHORISATION_STATES::UNAUTHORISED;
//}
//
//
