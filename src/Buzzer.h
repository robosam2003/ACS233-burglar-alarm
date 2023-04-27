//
// Created by bened on 4/24/2023.
//

#ifndef ACS233_BURGLAR_ALARM_BUZZER_H
#define ACS233_BURGLAR_ALARM_BUZZER_H

#include "Arduino.h"

#define ALARM_FREQ 500


class Buzzer {
private:
    uint8_t BUZZER_PIN;

public:

    Buzzer(uint8_t pin){
        BUZZER_PIN = pin;
        pinMode(this->BUZZER_PIN, OUTPUT);
    }

    void on() const {
        tone(BUZZER_PIN, ALARM_FREQ);
    }

    void off() const {
        noTone(BUZZER_PIN);
    }

    void beep(int freq, int onDel,int offDel) const {
        tone(BUZZER_PIN, freq);
        delay(onDel);
        noTone(BUZZER_PIN);
        delay(offDel);
    }
};


#endif //ACS233_BURGLAR_ALARM_BUZZER_H
