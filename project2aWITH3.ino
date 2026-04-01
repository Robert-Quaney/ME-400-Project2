#include <WiFi.h>
#include "lcdhelperv2.h"
#include "irhelper.h"
#include <ESP32Servo.h>
#include "httphelper.h"
#include <DabbleESP32.h>
//
//  Create an instance of the lcdhelper class named oLCD
//
lcdhelper oLCD(ILI9163_4L, 3, 2, 9, 10, 7);
//
//  Create an instance of irhelper class named oIR.
//
irhelper oIR;
//
//  Define function prototypes.
//
const int SW1_PIN = 18;
const int SW2_PIN = 17;
const int SW3_PIN = 16;
const int SW4_PIN = 15;

const int SPEAKER_PIN = 8;

const int MAX_CS_PIN = 10;
const int MAX_CLK_PIN = 12;
const int MAX_SO_PIN = 13;
// MOSFET PIN
const int PWM_PIN = 11;
// optocoupler
const int OPTO_PIN = 38;
// pan and tilt
const int PAN_SERVO_PIN = 36;
const int TILT_SERVO_PIN = 35;

// DAN- add lines 35-40 to finish main document steps 13-15
// Speaker channel
const int SPK_CHANNEL = 1;
// PWM channel
const int PWM_CHANNEL = 2;
// Audio channel
const int AUDIO_CHANNEL = 4;

void ShowDisplay(screen val, char optionstate, char keypressed, bool intdisplay);

void Option1();
void Option2(char optionstate);
void Option3(char optionstate, char keypressed);
void Option4(char keypressed);
void Option5();

Servo pservo;
Servo tservo;

int pangle = 90; // angle to 90 degrees
int tangle = 90; // angle to 90 degrees
///////////////////////////////////////////////////////////////////////////////////////
bool servosAttached = false; // setting servos as unattached
//
//  Setup()
//
void setup()
{
    Serial.begin(115200);
    Dabble.begin("Group3");
    while (!Serial)
    {
        // wait for serial to start
    }

    // switch pin inputs
    pinMode(SW1_PIN, INPUT_PULLUP);
    pinMode(SW2_PIN, INPUT_PULLUP);
    pinMode(SW3_PIN, INPUT_PULLUP);
    pinMode(SW4_PIN, INPUT_PULLUP);

    // speaker pin
    pinMode(SPEAKER_PIN, OUTPUT);

    // MAX6675 pins
    pinMode(MAX_CS_PIN, OUTPUT);
    pinMode(MAX_CLK_PIN, OUTPUT);
    pinMode(MAX_SO_PIN, INPUT);
    digitalWrite(MAX_CS_PIN, HIGH);

    ESP32PWM::allocateTimer(3); // set servo objects to 20 milliseconds
    // MOSFET Pin
    pinMode(PWM_PIN, OUTPUT);

    // optocouple pin
    pinMode(OPTO_PIN, INPUT);

    // tilt and pan servo pins
    pservo.setPeriodHertz(50);
    tservo.setPeriodHertz(50);
    pinMode(TILT_SERVO_PIN, OUTPUT);
    pinMode(PAN_SERVO_PIN, OUTPUT);
    tservo.attach(TILT_SERVO_PIN);
    pservo.attach(PAN_SERVO_PIN);
    tservo.write(90);
    pservo.write(90);
    delay(500);
    tservo.detach();
    pservo.detach();
    // Initialize IR Communications
    oIR.IRInitialize();
    // Show main menu when Arduino starts
    // for the 1st time or on reset.
    ShowDisplay(SC_MAIN, ' ', ' ', true);
}
//
//  Loops(): Perform looping operations
//
void loop()
{
    unsigned long last_key_processed = oIR.GetKeyPressed();

    if(last_key_processed == KEY_MENU) {
        ShowDisplay(SC_MAIN, ' ', ' ', false);
    }

    // Update gamepad only if in Pan & Tilt screen
    if(oLCD.SCREEN_STATE == SC_SUB2) {
        HandleGamePad();
    }

    // Returning to main menu
    if(last_key_processed == KEY_RETURN && oLCD.SCREEN_STATE == SC_SUB2) {
        tservo.detach();
        pservo.detach();
        servosAttached = false;
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
                HandleGamePad();
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
        else if (oLCD.SCREEN_STATE == SC_SUB1) // WARNINGS
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
                tservo.detach(); // detaching servos when leaving option2
                pservo.detach();
                servosAttached = false; // setting servos to detached
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        }
        else if (oLCD.SCREEN_STATE == SC_SUB3) // WARNINGS
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
    // Store screen state.
    oLCD.SCREEN_STATE = val;
    oLCD.OPTION_STATE = optionstate;
    int selectedpid = 0;
    char text[20];

    if (val == SC_MAIN)
    {
        oLCD.LCDInitialize(LANDSCAPE, initdisplay);
        sprintf(text, "SELECT OPTION");
        oLCD.print(text, 10, 10);
        sprintf(text, "1. Mystery Music");
        oLCD.print(text, 10, 25);
        sprintf(text, "2. Pan and Tilt");
        oLCD.print(text, 10, 40);
        sprintf(text, "3. Run the Motor");
        oLCD.print(text, 10, 55);
        sprintf(text, "4. Stream Audio");
        oLCD.print(text, 10, 70);
        sprintf(text, "5. Query Sensors");
        oLCD.print(text, 10, 85);
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
void Option1()
{
    oLCD.LCDInitialize(LANDSCAPE, false);
}
///  //////////////////////////////////////////////////////////////////////////////////
void AttachServos() // initialize a function to attach servos
{
    if (servosAttached == false) // seeing if servos are set to attached
    {
        tservo.attach(TILT_SERVO_PIN); // attaching tilt servo
        pservo.attach(PAN_SERVO_PIN);  // attaching pan servo

        tservo.write(tangle); // moving servo to specified angle
        pservo.write(pangle); // moving servo to specified angle

        servosAttached = true; // setting servos to attached
    }
}
void ServoScreenUpdate()
{
    char text[20]; // create character array
    sprintf(text, "PAN ANGLE");
    oLCD.print(text, CENTER, 30);          // output text(PAN ANGLE) to screen
    oLCD.printNumF(pangle, 4, CENTER, 45); // output pan angle number to screen

    sprintf(text, "TILT ANGLE");
    oLCD.print(text, CENTER, 60);          // output text(tilt ANGLE) to screen
    oLCD.printNumF(tangle, 4, CENTER, 75); // output tilt angle number to screen

    sprintf(text, "PRESS <RETURN>");
    oLCD.print(text, CENTER, 90); // output "PRESS <RETURN>" to the screen

    sprintf(text, "TO GO BACK");
    oLCD.print(text, CENTER, 105); // output "TO GO BACK" to the screen
}
void HandleGamePad()
{
    Dabble.processInput(); // starts collecting data from dabble

    if (GamePad.isUpPressed()) // Up button pressed
    {
        tangle += 5;      // increase tilt angle by 5 degrees
        if (tangle > 165) // see if tilt angle is going above maximum
        {
            tangle = 165; // set tilt angle at maximum
        }
        tservo.write(tangle); // write the tilt angle to the specified amount
    }
    if (GamePad.isDownPressed()) // Down button is pressed
    {
        tangle -= 5;     // decrease tilt angle by 5 degrees
        if (tangle < 15) // see if tilt angle is going below minimum
        {
            tangle = 15; // set tilt angle at minimum
        }
        tservo.write(tangle);
    }
    if (GamePad.isLeftPressed()) // Left button is pressed
    {
        pangle += 5;      // pan 5 degrees to the left
        if (pangle > 165) // see if pan angle is at the max
        {
            pangle = 165; // set pan angle to max if going above
        }
        pservo.write(pangle); // move servo to specified angle
    }
    if (GamePad.isRightPressed()) // Right button is pressed
    {
        pangle -= 5;     // pan 5 degrees to the right
        if (pangle < 15) // see if the pan angle is at the min
        {
            pangle = 15; // set pan angle at min if going below
        }
        pservo.write(pangle); // move servo to specified angle
    }
    // update the display of new angles
    ServoScreenUpdate(); // calls function to update ther servo screen

    ////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Option2(char optionstate)
{
    ////////////////////////////////////////////////////////////////////////////////////
    AttachServos(); // call function to attach servos
    oLCD.LCDInitialize(LANDSCAPE, false);
    //////////////////////////////////////////////////////////
    char text[20]; // create character array
    sprintf(text, "PAN ANGLE");
    oLCD.print(text, CENTER, 30);          // output text(PAN ANGLE) to screen
    oLCD.printNumF(pangle, 4, CENTER, 45); // output pan angle number to screen

    sprintf(text, "TILT ANGLE");
    oLCD.print(text, CENTER, 60);          // output text(tilt ANGLE) to screen
    oLCD.printNumF(tangle, 4, CENTER, 75); // output tilt angle number to screen

    sprintf(text, "PRESS <RETURN>");
    oLCD.print(text, CENTER, 90); // output "PRESS <RETURN>" to the screen

    sprintf(text, "TO GO BACK");
    oLCD.print(text, CENTER, 105); // output "TO GO BACK" to the screen
    ////////////////////////////////////////////////////////////
}
//
//  Code for Option3
//
void Option3(char optionstate, char keypressed)
{
    oLCD.LCDInitialize(LANDSCAPE, false);
}
//
//  Option to Move between pictures and songs.
//
void Option4(char keypressed)
{
    oLCD.LCDInitialize(LANDSCAPE, false);
}
//
//  Thermocouple implementation Entry Point
//
void Option5()
{
    oLCD.LCDInitialize(LANDSCAPE, false);
}
//********************************************************************
//  END OPTION CODE
//********************************************************************
