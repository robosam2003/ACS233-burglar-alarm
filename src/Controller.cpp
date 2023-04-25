/** Controller.cpp - Created by robosam2003 on 21/04/2023.
 *  This file contains the implementation of the Controller class methods - See Controller.h.
 *
 */

#include "Controller.h"

// This has to be done because the ISR functions are static and cannot access the instance of the class
// This works for singleton (only one instance of the class) classes.
Controller* Controller::instance = nullptr;


// Constructor
Controller::Controller(int mag_sensor_pin, int pir_sensor_pin, int key_sensor_pin,
                       int disarmed_mode_led, int at_home_mode_led, int armed_mode_led,
                       int pir_led, int alarm_led, int solenoid_pin, int buzzer_pin) {
    instance = this;
//    mag = new MAGSensor(mag_sensor_pin, MAGISRFunc);
//    pir = new PIRSensor(pir_sensor_pin, PIRISRFunc);
    key = new KEYSensor(key_sensor_pin, KEYISRFunc);

//    disarmedModeLed = new LED(disarmed_mode_led);
//    atHomeModeLed = new LED(at_home_mode_led);
//    armedModeLed = new LED(armed_mode_led);
//
//    pirLed = new LED(pir_led);
//    alarmLed = new LED(alarm_led);
//
    lock = new Solenoid(solenoid_pin);
//    buzzer = new Buzzer(buzzer_pin);

    last_mode_change_time = -100000; // reset last mode change time so that system works instantly

}

void Controller::setup() {
    Serial.begin(115200);
//    mag->setup();
//    pir->setup();
    key->setup();
}

void Controller::mag_isr() { // private because it is only called by the interrupt service routine

    if (timeSince_ms(mag->last_interrupt_time) > 100) {     // debounce the interrupt using millis()
        // update the last interrupt time
        mag->last_interrupt_time = millis();
        Serial.print("mag_isr() was triggered  a=");
        mag->door_state = (DOOR_STATES) mag->read();

        if (mag->door_state == DOOR_STATES::CLOSED) {
            Serial.println("Door was closed");
            last_door_close_time = millis();
            if (lock->state == LOCK_STATES::UNLOCKED) {
                // If the door gets closed and the lock is unlocked, lock the door
                Serial.println("LOCKING DOOR1");
                lock->lock();
            }
        }
        else { // door_state == DOOR_STATES::OPEN
            Serial.println("Door was opened");
            switch (system_mode) {
                case (SYSTEM_MODES::ARMED):
                case (SYSTEM_MODES::AT_HOME):
                    verificationWindowOpen();
                    break;
                case (SYSTEM_MODES::DISARMED):
                default:
                    break;
            }
            last_door_open_time = millis();
        }
    }

}

void Controller::pir_isr() {
    // The PIR does not need debouncing, it goes High for 8 seconds when motion is detected
    pir->pir_state = (PIR_STATES) pir->read();
    if (pir->pir_state == PIR_STATES::PRESENCE) {
        pir->pir_state = PIR_STATES::PRESENCE;
        Serial.println("PIR state changed to: PRESENCE");
        pirLed->high();
        switch (system_mode) {
            case (SYSTEM_MODES::ARMED):
                verificationWindowOpen();
                break;
            case (SYSTEM_MODES::AT_HOME):
            case (SYSTEM_MODES::DISARMED):
            default:
                break;
        }
    }
    else {
        pirLed->low();
    }
}

void Controller::key_isr() {
//    Serial.println("The key was pressed");
    if (timeSince_ms(key->last_interrupt_time) > 100) { // debounce the interrupt using millis()
        security_timer = millis(); // start the security timer
        Serial.println("UNLOCKING DOOR");

        // Unlock the door
        if (lock->state == LOCK_STATES::LOCKED)
            lock->unlock();
        else
            lock->lock();
//        lock->unlock();
//        Serial.println("Lock state changed to: UNLOCKED");
//        delay(1000);
//        lock->lock();
//        Serial.println("Lock state changed to: LOCKED");
    }
    key->last_interrupt_time = millis();


}

void Controller::toArmedMode() {
    system_mode = SYSTEM_MODES::ARMED;
        disarmedModeLed->low(); //
        atHomeModeLed->low();   //
        armedModeLed->high();   //
    security_timer = -100000; // reset security timer in case mode changed inside intermittent timeout window
    last_mode_change_time = millis();
}

void Controller::toDisarmedMode() {
    system_mode = SYSTEM_MODES::DISARMED;
    disarmedModeLed->high(); //
    atHomeModeLed->low();   //
    armedModeLed->low();   //
    security_timer = -100000; // reset security timer in case mode changed inside intermittent timeout window
    last_mode_change_time = millis();
}

void Controller::toAtHomeMode() {
    system_mode = SYSTEM_MODES::AT_HOME;
    disarmedModeLed->low(); //
    atHomeModeLed->high();   //
    armedModeLed->low();   //
    security_timer = -100000; // reset security timer in case mode changed inside intermittent timeout window
    last_mode_change_time = millis();
}

void Controller::verifyUser() {
    if (verification_window_state == VERIFICATION_WINDOW_STATES::WINDOW_OPEN) {
        Serial.println("VERIFICATION REQUEST");
        String message;
        while (!Serial.available()); // wait for user input
        // Read the serial port until a newline
        message = Serial.readStringUntil('\n');
        Serial.println("(verifyUser) Message received: " + message);

        if (message == "VERIFIED") {
            authorization_state = AUTHORISATION_STATES::AUTHORISED;
            Serial.println("I RECEIVED YOUR VERIFIED MESSAGE");
            buzzer->on(); // turns on Buzzer

            while (!Serial.available()); // wait but only on this one
            changingMode();
            Serial.println("MODE CHANGED TO: " + (String) system_mode);
            verificationWindowClosed();
        }
        else if (message == "VERIFICATION WINDOW CLOSED") { // This is for when the user enters the pin too many times.
            verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_CLOSED;
        }
        else if (message == "VERIFICATION WINDOW OPEN") {
            verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_OPEN;
        }
        else { // message is "NOT VERIFIED"
            authorization_state = AUTHORISATION_STATES::UNAUTHORISED;
            return;
        }
    }
}

void Controller::verificationWindowOpen() {
    Serial.println("VERIFICATION WINDOW OPEN");
    verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_OPEN;
}

void Controller::verificationWindowClosed() {
    Serial.println("VERIFICATION WINDOW CLOSED");
    verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_CLOSED;
}

void Controller::changingMode() {
    if (Serial.available()) { // wait for user input
        String message;
        // Read the serial port until a newline
        message = Serial.readStringUntil('\n');
        Serial.println("(changingMode) Message received: " + message);

        if (message == "MODE: ARMED") {
            toArmedMode();
        } else if (message == "MODE: AT-HOME") {
            toAtHomeMode();
        } else if (message == "MODE: DISARMED") {
            toDisarmedMode();
        }
        verificationWindowClosed();
        while(mag->door_state == DOOR_STATES::OPEN); // wait until door is closed again
    }
}

void Controller::unauthorizedEntry(UNAUTHORISED_ENTRY_METHODS method) {
    switch (method) {
        case DOOR_OPENED:
            switch (system_mode) {
                case (SYSTEM_MODES::ARMED):   // when cases don't have a break, they fall through to the next case (i.e. this means OR)
                case (SYSTEM_MODES::AT_HOME):
                     buzzer->on(); // turns on Buzzer
                    break;
                case (SYSTEM_MODES::DISARMED):
                    return;
                default:
                    break;
            }

            // Waiting for verified pin or 15 minutes elapsed
            while ((authorization_state == AUTHORISATION_STATES::UNAUTHORISED) &&
                   (timeSince_ms(last_alarm_on_time) < ALARM_TIMEOUT)) {
                alarmLed->blink(100,100);
                verifyUser();
                if (authorization_state == AUTHORISATION_STATES::AUTHORISED) {
                    authorization_state = AUTHORISATION_STATES::UNAUTHORISED;
                    last_door_open_time = -100000;
                    break;
                }
            }
            buzzer->off(); // turns on Buzzer
            break;
        case PIR_TRIGGERED:
            if ((timeSince_ms(security_timer) < INTERMITTENT_TIMEOUT)) {
                break;
            } // if the security timer is less than 30 seconds, the alarm will not go off
            else {
                switch (system_mode) {
                    case (SYSTEM_MODES::ARMED):   // when cases don't have a break, they fall through to the next case (i.e. this means OR)
                        buzzer->on(); // turns on Buzzer
                        break;
                    case (SYSTEM_MODES::AT_HOME):
                    case (SYSTEM_MODES::DISARMED):
                        return;
                    default:
                        break;
                }
//                pir_state = PIR_STATES::NO_PRESENCE;
                pir->pir_state = PIR_STATES::NO_PRESENCE;


                // Waiting for verified pin or 15 minutes elapsed
                while (authorization_state == AUTHORISATION_STATES::UNAUTHORISED && timeSince_ms(last_alarm_on_time) < ALARM_TIMEOUT) {
                    alarmLed->blink(100,100);
                    verifyUser();
                    if (authorization_state == AUTHORISATION_STATES::AUTHORISED) {
                        authorization_state = AUTHORISATION_STATES::UNAUTHORISED;
                        last_door_open_time = -100000;
                        break;
                    }
                }
                buzzer->off(); // turns on Buzzer
            }
            break;
    }
}

void Controller::alarmOn() {
    buzzer->on(); // turns on Buzzer
    last_alarm_on_time = millis();
}

void Controller::alarmOff() {
    buzzer->off(); // turns on Buzzer
}



