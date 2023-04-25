//
// Created by robos on 21/04/2023.
//

#ifndef ACS233_BURGLAR_ALARM_CONTROLLER_H
#define ACS233_BURGLAR_ALARM_CONTROLLER_H
#include "Arduino.h"
#include "Sensor.h"
#include "constants.h"
#include "Solenoid.h"
#include "Buzzer.h"


// Definitions
#define ALARM_FREQ 500
#define INTERMITTENT_TIMEOUT 20000 // 30 seconds


#define ALARM_TIMEOUT 30000 // 15 minutes
#define MODE_CHANGE_TIMEOUT 30000

// Macros
#define timeSince_ms(event) millis() - event

class Controller {
private:
    int64_t security_timer = -100000;
    int64_t last_door_open_time = -100000; // in ms
    int64_t last_door_close_time = -100000; // in ms
    int64_t last_alarm_on_time = 0;
    int64_t last_mode_change_time = -100000;

    SYSTEM_MODES system_mode = DISARMED;

    AUTHORISATION_STATES authorization_state = AUTHORISATION_STATES::UNAUTHORISED; //id password authorization_state variable
    VERIFICATION_WINDOW_STATES verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_CLOSED;

    MAGSensor* mag;
    PIRSensor* pir;
    KEYSensor* key;

    LED* disarmedModeLed;
    LED* atHomeModeLed;
    LED* armedModeLed;

    LED* pirLed;
    LED* alarmLed;

    Solenoid* lock;
    Buzzer* buzzer;



    void mag_isr();
    void pir_isr();
    void key_isr();

public:
    static Controller* instance; // Needed to make the interrupt service routines work in a class

    // Constructor
    explicit Controller(int mag_sensor_pin, int pir_sensor_pin, int key_sensor_pin,
                        int disarmed_mode_led, int at_home_mode_led, int armed_mode_led,
                        int pir_led, int alarm_led,int solenoid_pin,int buzzer_pin);

    // Setup
    void setup();

    // Interrupt Service Routines
    static void MAGISRFunc() { // forwards to the non-static function
        instance->mag_isr();
    }

    static void PIRISRFunc() { // forwards to the non-static function
        instance->pir_isr();
    }

    static void KEYISRFunc() { // forwards to the non-static function
        instance->key_isr();
    }

    // Mode change functions
    void toArmedMode();
    void toDisarmedMode();
    void toAtHomeMode();

    void verifyUser();
    void verificationWindowOpen();
    void verificationWindowClosed();
    void changingMode();

    void alarmOn();  // TODO Buzzer class
    void alarmOff();

    void unauthorizedEntry(UNAUTHORISED_ENTRY_METHODS method);
    void authorizedEntry();


    // Destructor
    ~Controller() {
        delete mag;
    }

};


#endif //ACS233_BURGLAR_ALARM_CONTROLLER_H
