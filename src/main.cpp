// * A burglar alarm system (GROUP 4) 23 Jan 2023
// * Authors: Samuel Scott, Eurico Benedict, Simon Desir, Alex Hicks, Yu-ta Chou
// *
// * This program is the arduino end to a two piece inter-working software that runs
// * the burglar alarm system. It functions as follows, it monitors sensor inputs and reacts
// * accordingly.
// *
// * Input devices are listed as follows;
// * PIR Sensor - used to detect motion
// * Magnetic Reed Sensor - used to detect door state (open/closed)
// * Button - used to detect unlocking of the door
// * Python serial stream - Used for user authorisation confirmation and mode change input
// *
// * Output devices are listed as follows;
// * Piezo buzzer - used as a warning or alarm sound output
// * Solenoid - used to lock and unlock door
// * LEDs - used as a status indicator
// * Python serial stream - Used for authorisation requests
// *
// * There are three modes programmed into the system and depending on the modes it is running on
// * the system behaves a different way.
// * ARMED - in armed mode all sensor input is taken into account, if PIR detects movement when the door is not unlocked
// * it will sound an alarm, if the reed sensor is activated without unlocking it will sound an alarm, if the user
// * fails to be authorised during the entry procedure it will sound an alarm.
// * AT_HOME - in at home mode the pir will not activate the alarm, everything else remains the same unauthorised entry
// * will still set off an alarm
// * DISARMED - in disarmed mode door will still lock but no authorisation will be needed, and no alarm will sound under
// * any conditions.

// 5th May 2023 - the program is split into its component classes with the OOP programming method



#include "Arduino.h"
#include "Controller.h"


/// Definitions
#define MAG_SWITCH_PIN 2 // attached to interrupt
#define KEY_SENSOR_PIN 3 // attached to interrupt
#define SOLENOID_PIN 12 // may be changed

#define BUZZER_PIN 7 //
#define PIR_PIN 19

#define DISARMED_MODE_LED 47 // Red
#define AT_HOME_MODE_LED 46 // Yellow
#define ARMED_MODE_LED 49 // Green
#define PIR_LED 42 // ITS RED

#define ALARM_LED 48 // ITS RED


Controller cont(MAG_SWITCH_PIN, PIR_PIN, KEY_SENSOR_PIN,
                DISARMED_MODE_LED, AT_HOME_MODE_LED, ARMED_MODE_LED, PIR_LED, ALARM_LED,
                SOLENOID_PIN, BUZZER_PIN);


void setup() {
    cont.setup();
    cont.toAtHomeMode() ;
}

void loop() {
    cont.loop();

}