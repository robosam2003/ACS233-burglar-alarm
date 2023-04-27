//
// Created by robos on 24/02/2023.
//

#ifndef ACS233_BURGLAR_ALARM_SENSOR_H
#define ACS233_BURGLAR_ALARM_SENSOR_H

#include "Arduino.h"
#include "constants.h"

class Sensor {
protected:
    uint8_t sensor_pin;
    void (*_isr)(); // pointer to the interrupt service routine
public:
    int64_t last_interrupt_time;
    Sensor(uint8_t pin, void (*isr)()) {
        this->sensor_pin = pin;
        last_interrupt_time = -100000;
        _isr = isr;
    }
    uint8_t setup() {
        pinMode(this->sensor_pin, INPUT);
        if (_isr != nullptr) { // Not all sensors have an ISR
            attachInterrupt(digitalPinToInterrupt(this->sensor_pin), _isr, CHANGE);
        }
        return 0;
    }

    int read() const {
        return digitalRead(sensor_pin);
    }
};


class MAGSensor : public Sensor {
public:
    DOOR_STATES door_state;
    uint8_t setup() {
        pinMode(this->sensor_pin, INPUT_PULLUP);
        if (_isr != nullptr) { // Not all sensors have an ISR
            attachInterrupt(digitalPinToInterrupt(this->sensor_pin), _isr, CHANGE);
        }
        return 0;
    }

    MAGSensor(uint8_t pin, void (*ISRFunc)()) : Sensor(pin, ISRFunc) {
        this->door_state = (DOOR_STATES) this->read();
    }
};

class PIRSensor : public Sensor {
public:
    PIR_STATES pir_state;

    PIRSensor(uint8_t pin, void (*ISRFunc)()) : Sensor(pin, ISRFunc) {
        this->pir_state = (PIR_STATES) this->read();
    }
};

class KEYSensor : public Sensor {
public:
    uint8_t setup() {
        pinMode(this->sensor_pin, INPUT_PULLUP);
        if (_isr != nullptr) { // Not all sensors have an ISR
            attachInterrupt(digitalPinToInterrupt(this->sensor_pin), _isr, FALLING);
        }
        return 0;
    }
    KEYSensor(uint8_t pin, void (*ISRFunc)()) : Sensor(pin, ISRFunc) {}
};


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



#endif //ACS233_BURGLAR_ALARM_SENSOR_H
