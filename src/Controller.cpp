//
// Created by robos on 21/04/2023.
//

#include "Controller.h"

Controller* Controller::instance = nullptr;

Controller::Controller(int mag_sensor_pin) {
    instance = this;
    a = rand();
    mag = new MAGSensor(mag_sensor_pin, MAGISRFunc);
}

