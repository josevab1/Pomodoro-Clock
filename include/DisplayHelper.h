#ifndef DISPLAY_HELPERS_H
#define DISPLAY_HELPERS_H
#include <LiquidCrystal_I2C.h>

void work_minute_display(LiquidCrystal_I2C& lcd, int8_t mins){
    lcd.setCursor(5,0);
    if(mins < 1){
        lcd.print("  0");
    }
    else if(mins < 10){
        lcd.print("  ");
        lcd.print(mins);
    }
    else if(mins < 100){
        lcd.print(" ");
        lcd.print(mins);        
    } 
    else{
        lcd.print(mins);
    }
}

void work_second_display(LiquidCrystal_I2C& lcd, int8_t sec){
    lcd.setCursor(9,0);
    if(sec < 1){
        lcd.print("00");
    }
    else if(sec < 10){
        lcd.print("0");
        lcd.print(sec);
        lcd.print(" ");        
    }
    else{
        lcd.print(sec);
        lcd.print(" ");
    }
}

void rest_minute_display(LiquidCrystal_I2C& lcd, int8_t mins){
    lcd.setCursor(5,1);
    if(mins < 1){
        lcd.print("  0");
    }
    else if(mins < 10){
        lcd.print("  ");
        lcd.print(mins);
    }
    else if(mins < 100){
        lcd.print(" ");
        lcd.print(mins);        
    } 
    else{
        lcd.print(mins);
    }
}

void rest_second_display(LiquidCrystal_I2C& lcd, int8_t sec){
    lcd.setCursor(9,1);
    if(sec < 1){
        lcd.print("00 ");
    }
    else if(sec < 10){
        lcd.print("0");
        lcd.print(sec);
        lcd.print(" ");        
    }
    else{
        lcd.print(sec);
        lcd.print(" ");
    }
}

void buttonswitch(bool& Last_Button_State, bool& Current_Button_State, uint8_t Button_Pin, bool& Switch_State){
    Last_Button_State = Current_Button_State;
    Current_Button_State = digitalRead(Button_Pin);
    if(Last_Button_State == HIGH && Current_Button_State == LOW){
        Switch_State = !Switch_State;
        delay(50);
    }
}

#endif
