#include <Arduino.h>
#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

class ButtonManager{
    private:

        uint8_t pin;
        bool Last_Button_State;
        bool Current_Button_State;
        bool Switch_State;

    public:
    // parametric constructor
        ButtonManager(uint8_t Button_Pin); 
    
    // initialize as switch

    // button as switch function
        bool as_switch();

};


#endif