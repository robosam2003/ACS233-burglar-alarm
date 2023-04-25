//
// Created by bened on 4/24/2023.
//

#ifndef ACS233_BURGLAR_ALARM_BUZZER_H
#define ACS233_BURGLAR_ALARM_BUZZER_H


class Buzzer {
    protected:
    uint8_t BUZZER_PIN;

    public:
    void state_high(uint8_t BUZZER_PIN){
        digitalWrite(BUZZER_PIN, HIGH);
    }
    void state_low(uint8_t BUZZER_PIN){
        digitalWrite(BUZZER_PIN, LOW);
    }
};


#endif //ACS233_BURGLAR_ALARM_BUZZER_H
