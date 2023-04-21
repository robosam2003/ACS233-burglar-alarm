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



//
//class PIRSensor : public Sensor {
//private:
//    PIR_STATES pir_state;
//public:
//    explicit PIRSensor(uint8_t pin) : Sensor(pin) {
//        this->pir_state = NO_PRESENCE;
//    }
//
//    void update_state() { // could be "update state" and then a big "controller update state" is called like *all the time* so it acts like an interrupt?
//        PIR_STATES prev_state = pir_state;
//        pir_state = (PIR_STATES) this->read();
//
//        /* Some ISR like code here */
//    }
//};
//
//// magnetic switch
//class MAGSensor : public Sensor {
//private:
//    DOOR_STATES door_state;
//public:
//    explicit MAGSensor(uint8_t pin) : Sensor(pin) {
//        this->door_state = (DOOR_STATES) this->read();
//    }
//
//    void change_state(DOOR_STATES state) {
//        door_state = state;
//    }
//};
//
//// Key Sensor
//class KEYSensor : public Sensor {
//public:
//    explicit KEYSensor(uint8_t pin) : Sensor(pin) {}
//};
//
//
//
//// ----------------------------------------------------------------------------------------------
//class LED {
//private:
//    uint8_t pin;
//public:
//    uint8_t state;
//    LED(uint8_t pin) {
//        this->pin = pin;
//        this->state = LOW;
//        pinMode(pin, OUTPUT);
//    }
//
//    void on() {
//        digitalWrite(pin, HIGH);
//        state = HIGH;
//    }
//
//    void off() {
//        digitalWrite(pin, LOW);
//        state = LOW;
//    }
//
//    void LEDBlink(int onDel, int offDel) {
//        digitalWrite(pin, HIGH);
//        delay(onDel);
//        digitalWrite(pin, LOW);
//        delay(offDel);
//    }
//};


#endif //ACS233_BURGLAR_ALARM_SENSOR_H
