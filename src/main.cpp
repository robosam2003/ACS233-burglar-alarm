//
// Created by robos on 24/02/2023.
//


#include "Arduino.h"
#include "Controller.h"


/// Definitions
#define MAG_SWITCH_PIN 2 // attached to interrupt
#define KEY_SENSOR_PIN 3 // attached to interrupt
#define SOLENOID_PIN 12 // may be changed

#define BUZZER_PIN 7 //
#define PIR_PIN 19

#define ALARM_FREQ 500
#define INTERMITTENT_TIMEOUT 20000 // 30 seconds

int64_t ALARM_TIMEOUT = 30000; // 15 minutes
#define MODE_CHANGE_TIMEOUT 30000

#define DISARMED_MODE_LED 47 // Red
#define AT_HOME_MODE_LED 46 // Yellow
#define ARMED_MODE_LED 49 // Green
#define PIR_LED 42 // ITS RED

#define ALARM_LED 48 // ITS RED




Controller cont(3);


void setup() {
    cont.setup();
}


void loop() {
    // put your main code here, to run repeatedly:

}