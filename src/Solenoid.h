//
// Created by bened on 4/16/2023.
//
// Solenoid class for the alarm system, allows for solenoid to lock and unlock.

#ifndef ACS233_BURGLAR_ALARM_SOLENOID_H
#define ACS233_BURGLAR_ALARM_SOLENOID_H

#include "Arduino.h"

class Solenoid {
private:
    uint8_t SOLENOID_PIN;

public:
    LOCK_STATES state;


    explicit Solenoid (uint8_t pin) {
        SOLENOID_PIN = pin;
        state = LOCK_STATES::LOCKED;
        pinMode(this->SOLENOID_PIN, OUTPUT);
        lock();
    }

    void unlock () {
        digitalWrite(SOLENOID_PIN, HIGH);
        state = LOCK_STATES::UNLOCKED;
    }

    void lock () {
        digitalWrite(SOLENOID_PIN, LOW);
        state = LOCK_STATES::LOCKED;
    }
};


#endif //ACS233_BURGLAR_ALARM_SOLENOID_H
