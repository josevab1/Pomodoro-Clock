#include <Arduino.h>
#ifndef BUTTONSWITCH_H
#define BUTTONSWITCH_H

class ButtonSwitch{
    private:

        uint8_t pin;
        bool Last_Button_State;
        bool Current_Button_State;
        bool Switch_State;

    public:
    // parametric constructor
        ButtonSwitch(uint8_t Button_Pin) : pin(Button_Pin),
                                            Last_Button_State(HIGH),
                                            Current_Button_State(HIGH),
                                            Switch_State(false) {
            pinMode(pin, INPUT_PULLUP);
        }

    // getter
    bool getSwitch_State() const {return Switch_State;}    
    
    // button as switch function
        bool as_switch(){
            Last_Button_State = Current_Button_State;
            Current_Button_State = digitalRead(pin);
            if(Last_Button_State == HIGH && Current_Button_State == LOW){
                Switch_State = !Switch_State;
                delay(50);
            }
            return Switch_State;
        }

};


#endif