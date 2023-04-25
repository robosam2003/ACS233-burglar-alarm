//
// Created by bened on 4/16/2023.
//

#ifndef ACS233_BURGLAR_ALARM_SOLENOID_H
#define ACS233_BURGLAR_ALARM_SOLENOID_H

class Solenoid {
    protected:
    uint8_t SOLENOID_PIN;

    public:
    void state_high (uint8_t SOLENOID_PIN){
        digitalWrite(SOLENOID_PIN, HIGH);
        state = LOCK_STATES::UNLOCKED;
    }
    void state_low (uint8_t SOLENOID_PIN){
        digitalWrite(SOLENOID_PIN, LOW);
        state = LOCK_STATES::LOCKED;
    }
};


#endif //ACS233_BURGLAR_ALARM_SOLENOID_H
