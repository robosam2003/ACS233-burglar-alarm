/*
 * LED.h
 * Authors: Samuel Scott, Eurico Benedict, Simon Desir, Alex Hicks, Yu-ta Chou
 * This header file contains the LED class needed for the burglar alarm system.
 * The class manages the state of the LED and deals with the outputs to the LED.
 */

#ifndef ACS233_GROUP4_BURGLAR_LED_H
#define ACS233_GROUP4_BURGLAR_LED_H

#include "Arduino.h"

class LED {
protected:
    uint8_t LED_pin;
public:
    explicit LED(uint8_t pin) {
        LED_pin = pin;
        pinMode(LED_pin, OUTPUT);
    }

    void high() const {
        digitalWrite(LED_pin,HIGH);
    }

    void low() const {
        digitalWrite(LED_pin,LOW);
    }

    void blink(int onDelay, int offDelay) const {
        digitalWrite(LED_pin,HIGH);
        delay(onDelay);
        digitalWrite(LED_pin,LOW);
        delay(offDelay);
    }
};

#endif //ACS233_GROUP4_BURGLAR_LED_H
