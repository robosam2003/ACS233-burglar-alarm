/*
 * Controller.h
 *
 * Authors: Samuel Scott, Eurico Benedict, Simon Desir, Alex Hicks, Yu-ta Chou
 *
 * This header file contains the Controller class needed for the burglar alarm system.
 * The class manages the state of the system and deals with the inputs from the sensors,
 * as well as the outputs to the solenoid and buzzer and to the python code (for facial recognition and GUI).
 */

#ifndef ACS233_BURGLAR_ALARM_CONTROLLER_H
#define ACS233_BURGLAR_ALARM_CONTROLLER_H
#include "Arduino.h"
#include "Sensor.h"
#include "constants.h"
#include "Solenoid.h"
#include "Buzzer.h"
#include "LED.h"




// Macros
#define timeSince_ms(event) (millis() - event)

class Controller {
private:
    int64_t security_timer = -100000;
    int64_t last_door_open_time = -100000; // in ms
    int64_t last_door_close_time = -100000; // in ms
    int64_t last_alarm_on_time = 0;
    int64_t last_mode_change_time = -100000;

    int64_t ENTER_LEAVE_TIMEOUT = 30000;
    int64_t ALARM_TIMEOUT = 40000;

    SYSTEM_MODES system_mode = AT_HOME;

    AUTHORISATION_STATES authorization_state = AUTHORISATION_STATES::UNAUTHORISED; //id password authorization_state variable
    VERIFICATION_WINDOW_STATES verification_window_state = VERIFICATION_WINDOW_STATES::WINDOW_CLOSED;

    MAGSensor* mag;
    PIRSensor* pir;
    KEYSensor* key;

//    LED* disarmedModeLed; // now in an array to fulfill requirements
//    LED* atHomeModeLed;
//    LED* armedModeLed;
    LED* modeLeds[3];

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
                        int disarmed_mode_led, int at_home_mode_led, int armed_mode_led, int pir_led, int alarm_led,
                        int solenoid_pin,int buzzer_pin);

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

    void check_states();

    // Destructor
    ~Controller() {
        delete mag;
    }

};


#endif //ACS233_BURGLAR_ALARM_CONTROLLER_H
