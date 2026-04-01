#include <WiFi.h>  // ESP32 WiFi library
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
// tilt and pan
const int TILT_SERVO_PIN = 35;
const int PAN_SERVO_PIN = 36;

// DAN- add lines 35-40 to finish main document steps 13-15
// Speaker channel
const int SPK_CHANNEL = 1;
// PWM channel
const int PWM_CHANNEL = 2;
// Audio channel
const int AUDIO_CHANNEL = 4;

void ShowDisplay(screen val, char optionstate, char keypressed, bool intdisplay);
Servo pservo;
Servo tservo;

int pangle = 90; // initializing angle to 90 degrees
int tangle = 90; // initializing angle to 90 degrees

// FOR OPTION 3
const int PWM_FREQ = 496;
const int PWM_RESOLUTION = 8;
const int MOTOR_PIN = 25;

volatile int pulseCount = 0;
unsigned long lastTime = 0;
const int PULSES_PER_REV = 1;

void IRAM_ATTR countPulse()
{
    pulseCount++; // increment on each rising edge
}

// FOR OPTION 3


//
//  Setup(): Perform initialization
//
void setup()
{
    //  Setup Serial Port
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

    ESP32PWM::allocateTimer(3); // set servo objects to 20 milliseconds
    // MOSFET Pin
    pinMode(PWM_PIN, OUTPUT);

    // optocouple pin
    pinMode(OPTO_PIN, INPUT);

    
    
    // Initialize IR Communications
    oIR.IRInitialize();
    // Show main menu when Arduino starts
    // for the 1st time or on reset.
    ShowDisplay(SC_MAIN, ' ', ' ', true);

    // FOR OPTION 3

    pservo.setPeriodHertz(50);
    tservo.setPeriodHertz(50);

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(MOTOR_PIN, PWM_CHANNEL);

    pinMode(OPTO_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(OPTO_PIN), countPulse, RISING);
    lastTime = millis(); // initialize timer
    // FOR OPTION 3
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

void Option2(char optionstate)
{
    oLCD.LCDInitialize(LANDSCAPE, false);
}
//
//  Code for Option3
//
void displayOption3(String duty, float speed)
{
    oLCD.LCDInitialize(LANDSCAPE, false);
    oLCD.print("DESIRED DUTY", 10, 10);
    oLCD.print("CYCLE (%)", 10, 25);
    oLCD.print(duty.c_str(), 10, 40);

    oLCD.print("CALC'D SPEED", 10, 70);
    char buffer[10];
    sprintf(buffer, "%.1f", speed);
    oLCD.print(buffer, 10, 85);
}

String getDutyFromIR()
{
    String input = "";
    while (true)
    {
        unsigned long key = oIR.GetKeyPressed();

        if (key >= KEY_0 && key <= KEY_9)
        {
            int num = key - KEY_0;
            input += String(num);

            // pad with leading zeros to 3 digits
            String displayDuty = input;
            while (displayDuty.length() < 3)
                displayDuty = "0" + displayDuty;

            displayOption3(displayDuty, 0);
        }
        else if (key == KEY_RETURN)
        {
            return input;
        }
    }
}

void rampMotor(int duty)
{
    int target = dutyToPWM(duty);

    // ramp up
    for (int i = 0; i <= target; i++)
    {
        ledcWrite(PWM_CHANNEL, i);
        delay(10);
    }

    // ramp down
    for (int i = target; i >= 0; i--)
    {
        ledcWrite(PWM_CHANNEL, i);
        delay(10);
    }
}

unsigned long runMotor(int duty)
{
    int targetPwm = dutyToPWM(duty);

    // Ramp Up Sequence
    for (int i = 0; i <= targetPwm; i++)
    {
        ledcWrite(PWM_CHANNEL, i);
        delay(10);
    }

    // Continuous Measurement Loop
    while (true)
    {
        ledcWrite(PWM_CHANNEL, targetPwm);
        float speed = measureSpeed();
        displayOption3(String(duty), speed);

        delay(1000); // Wait 1 second

        unsigned long key = oIR.GetKeyPressed();

        // Check for exit conditions
        if (key == KEY_MENU)
        {
            // Ramp Down Sequence
            for (int i = targetPwm; i >= 0; i--)
            {
                ledcWrite(PWM_CHANNEL, i);
                delay(10);
            }

            ledcWrite(PWM_CHANNEL, 0); // Ensure motor is stopped
            return key;                // Tell Option3() which key caused the exit
        }
    }
}



float measureSpeed()
{
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 1000.0; // seconds

    if (elapsedTime <= 0)
        return 0;

    // RPM calculation
    float rpm = (pulseCount / (float)PULSES_PER_REV) / elapsedTime * 60.0;

    // Reset counters for next measurement
    pulseCount = 0;
    lastTime = currentTime;

    return rpm;
}

int dutyToPWM(int duty)
{
    if (duty < 0)
        duty = 0;
    if (duty > 100)
        duty = 100;
    return map(duty, 0, 100, 0, 255);
}

void Option3(char optionstate, char keypressed)
{
    while (true)
    {
        // Initial display
        displayOption3("   ", 0);

        // Get user input
        String input = getDutyFromIR();
        int duty = input.toInt();

        // Clamp values
        if (duty < 0)
            duty = 0;
        if (duty > 100)
            duty = 100;

        // Ramp motor
unsigned long exitKey = runMotor(duty);



        if (exitKey == KEY_MENU)
        {
            ShowDisplay(SC_MAIN, ' ', ' ', false);
            break;
        }
    }
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
