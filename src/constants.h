//
// Created by robos on 21/04/2023.
//

#ifndef ACS233_BURGLAR_ALARM_CONSTANTS_H
#define ACS233_BURGLAR_ALARM_CONSTANTS_H

/// Constants and enums
// Enumerations to make the code more readable
enum SYSTEM_MODES {
    ARMED,
    DISARMED,
    AT_HOME
};

enum DOOR_STATES {
    CLOSED = 0, // pin reads 0 when closed, 1 when open
    OPEN = 1
};

enum LOCK_STATES {
    LOCKED = 0,
    UNLOCKED = 1
};

enum PIR_STATES {
    NO_PRESENCE = 0,
    PRESENCE = 1
};

enum AUTHORISATION_STATES {
    UNAUTHORISED = 0,
    AUTHORISED = 1
};

enum UNAUTHORISED_ENTRY_METHODS {
    DOOR_OPENED = 0,
    PIR_TRIGGERED = 1
};

enum VERIFICATION_WINDOW_STATES {
    WINDOW_CLOSED = 0,
    WINDOW_OPEN = 1
};

#endif //ACS233_BURGLAR_ALARM_CONSTANTS_H
