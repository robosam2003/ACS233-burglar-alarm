//
// Created by bened on 4/24/2023.
//

#ifndef ACS233_BURGLAR_ALARM_BUZZER_H
#define ACS233_BURGLAR_ALARM_BUZZER_H

#include "Arduino.h"

class Buzzer {
private:
    uint8_t BUZZER_PIN;

public:

    Buzzer(uint8_t pin){
        uint8_t BUZZER_PIN = pin;
        pinMode(this->BUZZER_PIN, OUTPUT);
    }

    void on() {
        tone(BUZZER_PIN, 500);
    }
    void off() {
        noTone(BUZZER_PIN);
    }
};


#endif //ACS233_BURGLAR_ALARM_BUZZER_H
