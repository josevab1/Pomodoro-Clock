#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>

#include "DisplayHelper.h"
#include "ButtonSwitch.h"

/*
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

// For Pomodoro Mode
const uint8_t Pomodoro_Button = 2;

ButtonSwitch pomoSwitch(Pomodoro_Button);

int8_t INITIAL_WORK_TIME = 25; // default it to 25 mins
int8_t INITIAL_REST_TIME = 5; // default it to 5 mins
int8_t INITIAL_WORK_TIME_SEC = 0; 
int8_t INITIAL_REST_TIME_SEC = 0;
int8_t Work_Time = INITIAL_WORK_TIME; // minutes
int8_t Rest_Time = INITIAL_REST_TIME; // minutes
static uint64_t lastUpdate = 0;  // Stores the last time we updated
int8_t Work_Time_Sec = INITIAL_WORK_TIME_SEC; 
int8_t Rest_Time_Sec = INITIAL_REST_TIME_SEC;
uint8_t cycle_count = 0;
bool cycle_counted = false;

bool pomo_done = false;
// indicate by angle 
// 0 = idle
// 45 = finished cycle
// 90 = rest
// 135 = work
// 180 = edit
uint8_t servo_pin = 13;

bool Current_Pomo_Button_State;
bool Last_Pomo_Button_State;
bool pomoState = false;

uint8_t servo_pos = 0;

// For Edit Mode
const uint8_t Edit_Button = 3;

// Make button to a switch
ButtonSwitch editSwitch(Edit_Button);

const uint8_t Cursor_Button = 4;
ButtonSwitch cursorSwitch(Cursor_Button);

const uint8_t Inc_Button = 5;
const uint8_t Dec_Button = 6;

bool Last_Cursor_Button_State;
bool Current_Cursor_Button_State;
bool cursorState= false;

bool Current_Edit_Button_State;
bool Last_Edit_Button_State;
bool editState = false;

void pomo_setup(){
  pinMode(Pomodoro_Button, INPUT_PULLUP);

  lcd.init();
  lcd.clear();
  delay(100); // realized that distance affect i2c, just make connection close as possible
  lcd.backlight();
  lcd.clear();

  lcd.begin(16,2);
  // Work Time
  lcd.setCursor(0,0);
  lcd.print("WORK:");
  // work_minute_display
  work_minute_display(lcd, Work_Time);

  lcd.setCursor(8,0);
  lcd.print(":");

  // work_second_display
  work_second_display(lcd, Work_Time_Sec);

  // Rest Time
  lcd.setCursor(0,1);
  lcd.print("REST:");

  // rest_minute_dispaly
  rest_minute_display(lcd, Rest_Time);

  lcd.setCursor(8,1);
  lcd.print(":");

  rest_second_display(lcd, Rest_Time_Sec);
  servo.attach(servo_pin);

}

void edit_setup(){
  pinMode(Edit_Button, INPUT_PULLUP);
  pinMode(Cursor_Button, INPUT_PULLUP);
  pinMode(Inc_Button, INPUT_PULLUP);
  pinMode(Dec_Button, INPUT_PULLUP);

}

void work_time_tick(){
  // Have seconds decrease in work time
  uint64_t currentTime = millis();
  
  // For every 1 second, everytime 60 seconds happens, a minute happens
  // Display it on a screen...
  if(currentTime - lastUpdate >= 1000){
    lastUpdate = currentTime;  // Update the timestamp 
        //Serial.print(lastUpdate);
        //Serial.println(currentTime);
    if(Work_Time >= 0 && Work_Time_Sec >= 0){
      Work_Time_Sec = Work_Time_Sec - 1;

      if(Work_Time_Sec < 0 && Work_Time >= 1){
          Work_Time = Work_Time - 1;
          Work_Time_Sec = 59; // make this 59 sec for full minute;
      }
      else if(Work_Time_Sec < 0 && Work_Time < 1){
        Work_Time_Sec = 59;
      }
      else if(Work_Time_Sec == 0 && Work_Time == 0){
        Work_Time = -1;
        Work_Time_Sec = -1;
      }
    }


work_minute_display(lcd, Work_Time);
work_second_display(lcd, Work_Time_Sec);
    //Serial.println(Work_Time_Sec);
    //Serial.print(Work_Time);
  }
}


void rest_time_tick(){
  
  uint64_t currentTime = millis();
  
  if(currentTime - lastUpdate >= 1000){
    lastUpdate = currentTime;  // Update the timestamp 
        //Serial.print(lastUpdate);
        //Serial.println(currentTime);
    if(Rest_Time >= 0 || Rest_Time_Sec >= 0){
      Rest_Time_Sec = Rest_Time_Sec - 1;
      if(Rest_Time_Sec < 0 && Rest_Time >= 1){
        Rest_Time = Rest_Time - 1;
        Rest_Time_Sec = 59;
      }
      else if(Rest_Time_Sec < 0 && Rest_Time < 1){
        Rest_Time_Sec = 59;
      }
    }
    if(Rest_Time_Sec == 0 && Rest_Time == 0){
      Rest_Time = -1;
      Rest_Time_Sec = -1;
    }

    rest_minute_display(lcd, Rest_Time);
    rest_second_display(lcd, Rest_Time_Sec);
    
    }
    //Serial.println(Work_Time);
    //Serial.print(" ");
    //Serial.println(Rest_Time_Sec);
    //Serial.print(":");
    //Serial.print(Rest_Time);
  }



void pomo_mode(bool mode){
  bool pomo = digitalRead(Pomodoro_Button);
  bool pomo_done = false;

  static int8_t Work_Time_Appearance = 0;
  static int8_t Work_Time_Sec_Appearance = 0;
  static int8_t Rest_Time_Appearance = 0;
  static int8_t Rest_Time_Sec_Appearance = 0;
  static bool appearance_saved = false;


  if(mode == true){

    if(!appearance_saved){
      Work_Time_Appearance = INITIAL_WORK_TIME;
      Work_Time_Sec_Appearance = INITIAL_WORK_TIME_SEC;
      Rest_Time_Appearance = INITIAL_REST_TIME;
      Rest_Time_Sec_Appearance = INITIAL_REST_TIME_SEC;
      appearance_saved = true;
    }

    if(Work_Time >= 0){
      lcd.setCursor(12,1);
      lcd.print("POMO");
      lcd.print("    ");
      work_time_tick();
      //Serial.print(Work_Time);
      //Serial.println(Rest_Time);
      servo.write(135);
    
    }
    else if(Work_Time < 0 && Work_Time_Sec < 0 && Rest_Time >= 0){
      lcd.setCursor(12,1);
      lcd.print("POMO");
      lcd.print("    ");
      rest_time_tick();
      // Have button be pressed to move on to next mode.
      //Serial.print(Work_Time);
      //Serial.println(Rest_Time);
      servo.write(90);
    }
    
    else if(Work_Time < 0 && Rest_Time < 0 && Work_Time_Sec < 0 && Rest_Time_Sec < 0){
      //Serial.print(Work_Time);
      //Serial.println(Rest_Time);
      // Have button to determine whether to do another cycle.
      if(!cycle_counted){
      cycle_count = cycle_count + 1;
      cycle_counted = true;
      }

      pomo_done = true;

      if(pomo_done == true){
      servo.write(45);
      lcd.setCursor(11,0);
      lcd.print("cycle");
      lcd.setCursor(11,1);
      lcd.print("# = ");
      lcd.print(cycle_count);

  work_minute_display(lcd, Work_Time_Appearance);
  work_second_display(lcd, Work_Time_Sec_Appearance); 

  rest_minute_display(lcd, Rest_Time_Appearance);
  rest_second_display(lcd, Rest_Time_Sec_Appearance);    

    if(cycle_count > 10){
      lcd.setCursor(11,1);
      lcd.print("# =");        
    }

      lcd.setCursor(11,0);
      lcd.print("     ");
      lcd.setCursor(11,1);
      lcd.print("     ");
  }
      // I can press button million times, would not reset
      // reset button fax.
      if(!pomo){
        Work_Time = INITIAL_WORK_TIME;
        Rest_Time = INITIAL_REST_TIME;
        Work_Time_Sec = INITIAL_WORK_TIME_SEC;
        Rest_Time_Sec = INITIAL_REST_TIME_SEC;
        lastUpdate = millis();
        cycle_counted = false;
        pomo_done = false;
        appearance_saved = false;
        
    work_minute_display(lcd, Work_Time);
    work_second_display(lcd, Work_Time_Sec);
    rest_minute_display(lcd, Rest_Time);
    rest_second_display(lcd, Rest_Time_Sec);
    lcd.setCursor(5,1);
      }
      else{
      }
    }
    
  }
  else{
    lcd.setCursor(12,1);
    lcd.print("    ");
    appearance_saved = false;
  }
}

void edit_mode(bool state){
  // Cursor Button
  // cursor 5,0 is work time pos, cursor5,1 is rest time pos
  // if at those cursor, can inc or dec rest/work time
if(INITIAL_WORK_TIME < 1) INITIAL_WORK_TIME = 120;
if(INITIAL_WORK_TIME > 120) INITIAL_WORK_TIME = 1;

if(INITIAL_REST_TIME < 1) INITIAL_REST_TIME = 120;
if(INITIAL_REST_TIME > 120) INITIAL_REST_TIME = 1;

  if(state == true){
    lcd.setCursor(12,1);
    lcd.print("EDIT");
    servo.write(180);

  cursorState = cursorSwitch.as_switch();  
  // Editing Work Time 
  if(cursorState == false){
      lcd.setCursor(11,0);
      lcd.print("<");
      lcd.setCursor(11,1);
      lcd.print(" ");

    if(digitalRead(Inc_Button) == LOW && INITIAL_WORK_TIME <= 120){
      delay(250);
      lcd.setCursor(11,0);
      
      INITIAL_WORK_TIME += 1;
      Work_Time = INITIAL_WORK_TIME;
      

      lcd.setCursor(5,0);
        if(Work_Time > 120){
          Work_Time = 1;
          lcd.print("  ");
          lcd.print(Work_Time);
        }
        else if(Work_Time < 10){
          lcd.print("  ");
          lcd.print(Work_Time);
        }
        else if(Work_Time < 100){
          lcd.print(" ");
          lcd.print(Work_Time);
        }
        else{
          lcd.print(Work_Time);
        }

      lcd.setCursor(8,0);
      lcd.print(":");

      if(Work_Time_Sec < 0){
        Work_Time_Sec = 0;
        lcd.print("00");
      }
      else if(Work_Time_Sec < 10){
        lcd.print("0");
        lcd.print(Work_Time_Sec);
      }
      else{
      lcd.print(Work_Time_Sec);
      }
    }
    else if(digitalRead(Dec_Button)==LOW && INITIAL_WORK_TIME <= 120){
      delay(250);
  
      INITIAL_WORK_TIME -= 1;
      Work_Time = INITIAL_WORK_TIME;
      
      lcd.setCursor(5,0);
        if(Work_Time <= 0){
          Work_Time = 120;
          lcd.print(Work_Time);
        }
        else if(Work_Time < 10){
          lcd.print("  ");
          lcd.print(Work_Time);
        }
        else if(Work_Time < 100){
          lcd.print(" ");
          lcd.print(Work_Time);
        }
        else{
          lcd.print(Work_Time);
        }

      lcd.setCursor(8,0);
      lcd.print(":");

      if(Work_Time_Sec < 0){
        Work_Time_Sec = 0;
        lcd.print("00");
      }
      else if(Work_Time_Sec < 10){
        lcd.print("0");
        lcd.print(Work_Time_Sec);
      }
      else{
      lcd.print(Work_Time_Sec);
      }
    }
  }
  // Editing Rest
  else{
      lcd.setCursor(11,1);
      lcd.print("<");
      lcd.setCursor(11,0);
      lcd.print(" ");

    if(digitalRead(Inc_Button) == LOW && INITIAL_REST_TIME <= 120){
      delay(250);

      INITIAL_REST_TIME += 1;
      Rest_Time = INITIAL_REST_TIME;

      lcd.setCursor(5,1);
        if(Rest_Time > 120){
          Rest_Time = 1;
          lcd.print("  ");
          lcd.print(Rest_Time);
        }
        else if(Rest_Time < 10){
          lcd.print("  ");
          lcd.print(Rest_Time);
        }
        else if(Rest_Time < 100){
          lcd.print(" ");
          lcd.print(Rest_Time);
        }
        else{
          lcd.print(Rest_Time);
        }

      lcd.setCursor(8,1);
      lcd.print(":");

      if(Rest_Time_Sec < 0){
        Rest_Time_Sec = 0;
        lcd.print("00");
      }
      else if(Rest_Time_Sec < 10){
        lcd.print("0");
        lcd.print(Rest_Time_Sec);
      }
      else{
      lcd.print(Rest_Time_Sec);
      }
    }
    else if(digitalRead(Dec_Button)==LOW && INITIAL_REST_TIME <= 120){
      delay(250);

      INITIAL_REST_TIME -= 1;
      Rest_Time = INITIAL_REST_TIME;

      lcd.setCursor(5,1);
        if(Rest_Time <= 0){
          Rest_Time = 120;
          lcd.print(Rest_Time);
        }
        else if(Rest_Time< 10){
          lcd.print("  ");
          lcd.print(Rest_Time);
        }
        else if(Rest_Time < 100){
          lcd.print(" ");
          lcd.print(Rest_Time);
        }
        else{
          lcd.print(Rest_Time);
        }

      lcd.setCursor(8,1);
      lcd.print(":");

      if(Rest_Time_Sec < 0){
        Rest_Time_Sec = 0;
        lcd.print("00");
      }
      else if(Rest_Time_Sec < 10){
        lcd.print("0");
        lcd.print(Rest_Time_Sec);
      }
      else{
      lcd.print(Rest_Time_Sec);
      }
    }
  }

}
  else{
    lcd.setCursor(11,0);
    lcd.print("    ");
    lcd.setCursor(11,1);
    lcd.print("    ");    
}

}

void setup() {
  pomo_setup();
  edit_setup();
  Serial.begin(9600);

  Current_Pomo_Button_State = digitalRead(Pomodoro_Button);
  Current_Edit_Button_State = digitalRead(Edit_Button);
  Current_Cursor_Button_State = digitalRead(Cursor_Button);

}

void loop() {

// Edit Mode
editState = editSwitch.as_switch();
edit_mode(editState);

// Pomodoro Mode logic. 
pomoState = pomoSwitch.as_switch();
pomo_mode(pomoState);

if(pomo_done == true){
    lcd.setCursor(12,1);
    lcd.print("    ");
    pomo_done = false;
}

if(pomoState == true && editState == true){
  editState = false;
  pomoState = true;
}

else if(pomoState == false && editState == false){
  servo.write(servo_pos);
}
}