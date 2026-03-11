#include "lcdhelperv2.h"
#include "irhelper.h"
#include <ESP32Servo.h>
#include "httphelper.h"
//
//  Create an instance of the lcdhelper class named oLCD
//
lcdhelper oLCD(ILI9163_4L,3,2,9,10,7);
//
//  Create an instance of irhelper class named oIR.
//
irhelper oIR;
//
//  Define function prototypes.
//
void ShowDisplay(screen val, char optionstate, char keypressed, bool intdisplay);
//
//  Setup(): Perform initialization
//
void setup(){
    //  Setup Serial Port
    Serial.begin(115200);  
    while(!Serial){  
    // wait for serial to start  
    } 
    // Initialize IR Communications
    oIR.IRInitialize();
    // Show main menu when Arduino starts
    // for the 1st time or on reset.
    ShowDisplay(SC_MAIN, ' ', ' ',true);
}
//
//  Loops(): Perform looping operations
//
void loop()
{
    unsigned long last_key_processed;
    // Determine which key was pressed.
    last_key_processed = oIR.GetKeyPressed();
    // Return to the main menu any time the
    // menu key is pressed.
    if (last_key_processed == KEY_MENU)
    {
        ShowDisplay(SC_MAIN, ' ', ' ', false);
    }
    else
    {
        if (oLCD.SCREEN_STATE == SC_MAIN)
        {
            if (last_key_processed == KEY_1)  
            {
                ShowDisplay(SC_SUB1, ' ', ' ', false); 
            }
            else if (last_key_processed == KEY_2) 
            {
                ShowDisplay(SC_SUB2, 'A', ' ', false); 
            }
            else if (last_key_processed == KEY_3) 
            {
                ShowDisplay(SC_SUB3, 'A', ' ', false); 
            }
            else if (last_key_processed == KEY_4) 
            {
                ShowDisplay(SC_SUB4, ' ', ' ', false); 
            }
             else if (last_key_processed == KEY_5) 
            {
                ShowDisplay(SC_SUB5, ' ', ' ', false);
            }
            else if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        } 
        else if (oLCD.SCREEN_STATE == SC_SUB1)  //WARNINGS
        {
            if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false); 
            }
        } 
        else if (oLCD.SCREEN_STATE == SC_SUB2)
        {
            if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        }
        else if (oLCD.SCREEN_STATE == SC_SUB3)  //WARNINGS
        {
            if (last_key_processed == KEY_RETURN) 
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false);  
            }
        } 
        else if (oLCD.SCREEN_STATE == SC_SUB4)
        {
            if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false); 
            }
        }
        else if (oLCD.SCREEN_STATE == SC_SUB5)
        {
            if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        }
    }
    delay(100);
}
// *************************************************************************************
// BEGIN: FUNCTION DEFINITIONS
// *************************************************************************************
//
//  Method to display menus on TFT display
//
void ShowDisplay(screen val, char optionstate, char keypressed, bool initdisplay)
{
    //Store screen state.
    oLCD.SCREEN_STATE = val;
    oLCD.OPTION_STATE = optionstate;
    int selectedpid = 0;
    char text[20];

    if (val == SC_MAIN)
    {
        oLCD.LCDInitialize(LANDSCAPE, initdisplay);
        sprintf(text,"SELECT OPTION");oLCD.print(text,10,10);
        sprintf(text,"1. Mystery Music");oLCD.print(text,10,25); 
        sprintf(text,"2. Pan and Tilt");oLCD.print(text,10,40);
        sprintf(text,"3. Run the Motor");oLCD.print(text,10,55);
        sprintf(text,"4. Stream Audio");oLCD.print(text,10,70);
        sprintf(text,"5. Query Sensors");oLCD.print(text,10,85);
    }
    else if (val == SC_SUB1)
    {
        Option1();
    }
    else if (val == SC_SUB2)
    {
        Option2(optionstate);
    }
    else if (val == SC_SUB3)
    {
        Option3(optionstate, keypressed);
    }
    else if (val == SC_SUB4)
    {
        Option4(keypressed);
    }
    else if (val == SC_SUB5)
    {
        Option5();
    }
}
//********************************************************************
//  BEGIN OPTION CODE
//********************************************************************
void Option1(){
    oLCD.LCDInitialize(LANDSCAPE, false);
}   

void Option2(char optionstate){
    oLCD.LCDInitialize(LANDSCAPE, false);
}
//
//  Code for Option3
//
void Option3(char optionstate, char keypressed){
    oLCD.LCDInitialize(LANDSCAPE, false);
}
//
//  Option to Move between pictures and songs.
//
void Option4(char keypressed){
    oLCD.LCDInitialize(LANDSCAPE, false);
}
//
//  Thermocouple implementation Entry Point
//
void Option5(){
    oLCD.LCDInitialize(LANDSCAPE, false);
}
//********************************************************************
//  END OPTION CODE
//********************************************************************
