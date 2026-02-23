#ifndef DISPLAY_HELPERS_H
#define DISPLAY_HELPERS_H
#include <LiquidCrystal_I2C.h>

void work_minute_display(LiquidCrystal_I2C& lcd, int8_t mins){
    lcd.setCursor(5,0);
    if(mins < 1){
        lcd.print("00");
    }
    else if(mins < 10){
        lcd.print("0");
        lcd.print(mins);
    }
    else{
        lcd.print(mins);
    }
}

void work_second_display(LiquidCrystal_I2C& lcd, int8_t sec){
    lcd.setCursor(7,0);
    lcd.print(":");
    lcd.setCursor(8,0);
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
        lcd.print("00");
    }
    else if(mins < 10){
        lcd.print("0");
        lcd.print(mins);
    }
    else{
        lcd.print(mins);
    }
}

void rest_second_display(LiquidCrystal_I2C& lcd, int8_t sec){
    lcd.setCursor(7,1);
    lcd.print(":");
    lcd.setCursor(8,1);
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

void long_rest_minute_display(LiquidCrystal_I2C& lcd, int8_t mins){
    lcd.setCursor(0,1);
    lcd.print("LNGREST:");
    lcd.setCursor(7,1);
    if(mins < 1){
        lcd.print("00");
    }
    else if(mins < 10){
        lcd.print("0");
        lcd.print(mins);
    }
    else{
        lcd.print(mins);
    }
}

void long_rest_second_display(LiquidCrystal_I2C& lcd, int8_t sec){
    lcd.setCursor(10,1);
    lcd.print(":");
    lcd.setCursor(11,1);
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

#endif
