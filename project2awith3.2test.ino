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

bool servosAttached = false; // setting servos as unattached

// for three 33 3 3 3
String dutyInput = ""; // stores typed numbers
int dutyValue = 0;

// for three 33 3 3 3

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

    // for three 33 3 3 3
    pinMode(PWM_PIN, OUTPUT);
    ledcSetup(PWM_CHANNEL, 496, 8);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);
    // for three 33 3 3 3
}
//
//  Loops(): Perform looping operations
//
void loop()
{
    unsigned long last_key_processed = oIR.GetKeyPressed();

    if (last_key_processed == KEY_MENU)
    {
        ShowDisplay(SC_MAIN, ' ', ' ', false);
    }

    // Update gamepad only if in Pan & Tilt screen
    if (oLCD.SCREEN_STATE == SC_SUB2)
    {
        HandleGamePad();
    }

    // Returning to main menu
    if (last_key_processed == KEY_RETURN && oLCD.SCREEN_STATE == SC_SUB2)
    {
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

        oLCD.LCDInitialize(PORTRAIT, initdisplay);

        // Reset input on entry
        static bool resetEntry = true;
        if (resetEntry)
        {
            dutyInput = "";
            dutyValue = 0;
            resetEntry = false;
        }

        // Labels
        oLCD.print("DESIRED DUTY", CENTER, 20);
        oLCD.print("CYCLE (%)", CENTER, 35);

        // Duty cycle display
        oLCD.setBackColor(RCB_BLACK);
        oLCD.print("0", CENTER, 55);
        oLCD.setBackColor(RCB_PURPLE);

        // Speed display
        oLCD.print("CALC'D SPEED", CENTER, 80);
        oLCD.print("0", CENTER, 100);

        // Handle key input inside ShowDisplay
        int key = oIR.GetKeyPressed();

        if (key >= KEY_0 && key <= KEY_9)
        {
            if (dutyInput.length() < 3)
                dutyInput += oIR.TranslateKey(key);

            dutyValue = dutyInput.toInt();

            // Update duty display
            oLCD.setBackColor(RCB_BLACK);
            oLCD.print("   ", CENTER, 55); // clear old value
            oLCD.print(dutyInput.c_str(), CENTER, 55);
            oLCD.setBackColor(RCB_PURPLE);
        }
        else if (key == KEY_RETURN) // User confirmed the duty cycle
        {
            resetEntry = true;

            // Run motor with entered duty cycle
            ledcWrite(PWM_CHANNEL, map(dutyValue, 0, 100, 0, 255));

            // Continuously update measured speed
            bool running = true;
            while (running)
            {
                float rpm = measureMotorSpeed(); // You need to implement this function
                char speedStr[10];
                sprintf(speedStr, "%.1f", rpm);
                oLCD.setBackColor(RCB_BLACK);
                oLCD.print("       ", CENTER, 100); // Clear old speed
                oLCD.print(speedStr, CENTER, 100);
                oLCD.setBackColor(RCB_PURPLE);

                int loopKey = oIR.GetKeyPressed();
                if (loopKey == KEY_MODE) // Mode key stops motor and returns to main menu
                {
                    ledcWrite(PWM_CHANNEL, 0); // Stop motor
                    ShowDisplay(SC_MAIN, ' ', ' ', false);
                    running = false;
                }
                else if (loopKey == KEY_PLAY_PAUSE) // Play/Pause stops motor and clears duty input
                {
                    ledcWrite(PWM_CHANNEL, 0); // Stop motor
                    dutyInput = "";
                    dutyValue = 0;
                    ShowDisplay(SC_SUB3, 'A', ' ', true); // Reset duty entry
                    running = false;
                }
                delay(1000); // 1-second delay between updates
            }
        }
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

void Option2(char optionstate)
{
    oLCD.LCDInitialize(LANDSCAPE, false);
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
