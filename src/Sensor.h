//
// Created by robos on 24/02/2023.
//

#ifndef ACS233_BURGLAR_ALARM_SENSOR_H
#define ACS233_BURGLAR_ALARM_SENSOR_H

#include "Arduino.h"
#include "constants.h"

//// We cannot use inheritance because the interrupt service routine is static, and we use static variables.
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
    KEYSensor(uint8_t pin, void (*ISRFunc)()) : Sensor(pin, ISRFunc) {}
};




#endif //ACS233_BURGLAR_ALARM_SENSOR_H
