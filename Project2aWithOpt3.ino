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

// FOR OPTION 3
const int PWM_FREQ = 496;
const int PWM_RESOLUTION = 8;
const int MOTOR_PIN = 25;

volatile int pulseCount = 0;
unsigned long lastTime = 0;
const int PULSES_PER_REV = 1;

// FOR OPTION 3

void ShowDisplay(screen val, char optionstate, char keypressed, bool intdisplay);
Servo pservo;
Servo tservo;
pservo.setPeriodHertz(50);
tservo.setPeriodHertz(50);

int pangle = 90; // initializing angle to 90 degrees
int tangle = 90; // initializing angle to 90 degrees
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

    // tilt and pan servo pins
    pinMode(TILT_SERVO_PIN, OUTPUT);
    pinMode(PAN_SERVO_OUTPUT, OUTPUT);
    tservo.attach(TILT_SERVO_PIN);
    pservo.attach(PAN_SERVO);
    tservo.write(90);
    pservo.write(90);
    tservo.detach(TILT_SERVO_PIN);
    pservo.detach(PAN_SERVO);
    // Initialize IR Communications
    oIR.IRInitialize();
    // Show main menu when Arduino starts
    // for the 1st time or on reset.
    ShowDisplay(SC_MAIN, ' ', ' ', true);

    // FOR OPTION 3
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
    // attach speaker and channel
    ledcAttachPin(SPEAKER_PIN, SKP_CHANNEL);
    // set background color
    oLCD.fillscreen(RCB_PURPLE);
    // print music playing
    oLCD.print("MUSC PLAYING", 10, 10);

    // arrays for csv files
    int frequency[] = {
        659,
        0,
        349,
        261,
        0,
        659,
        622,
        740,
        0,
        587,
        0,
        349,
        0,
        622,
        740,
        587,
        0,
        466,
        587,
        0,
        622,
        740,
        523,
        0,
        349,
        0,
        261,
        622,
        740,
        0,
        523,
        0,
        466,
        0,
        622,
        740,
        311,
        0,
        466,
        0,
        391,
        622,
        740,
        0,
        466,
        0,
        659,
        0,
        622,
        740,
        349,
        0,
        261,
        0,
        659,
        622,
        740,
        0,
        587,
        0,
        349,
        0,
        622,
        740,
        587,
        0,
        466,
        0,
        587,
        622,
        740,
        0,
        523,
        0,
        349,
        0,
        622,
        740,
        261,
        0,
        293,
        0,
        311,
        622,
        740,
        0,
        329,
        0,
        293,
        0,
        622,
        740,
        293,
        0,
        329,
        0,
    };

    int duration[] = {113, 114, 113, 115, 113, 248, 496, 114, 113, 114, 113, 115, 248, 496, 113, 114, 113, 113, 115, 248, 496,
                      113, 114, 113, 1, 113, 248, 496, 114, 113, 114,
                      113, 114, 248, 496, 113, 115, 113, 114, 113, 248, 496, 1, 113, 114, 113, 114, 248, 496, 113, 1, 113, 114,
                      113, 248, 496, 114, 113, 115, 113, 114, 248, 496, 113,
                      114, 113, 1, 113, 248, 496, 114, 113, 114, 113, 1, 248, 496,
                      113, 1364, 151, 77, 151, 248, 496, 76, 226, 115, 169, 58, 248, 496,
                      151, 76, 113, 1, 151, 248, 496, 76, 151, 76, 151, 77, 248, 496,
                      151, 76, 151, 76, 151, 248, 496, 77, 113, 113, 1, 113, 248, 496,
                      151, 190, 151, 77, 151, 248, 496, 76, 151, 76, 226, 115, 248, 496,
                      169, 58, 151, 77, 113, 248, 496, 151, 77, 151, 76, 151, 248, 496,
                      76, 151, 76, 113, 1, 248, 496, 113, 1, 113, 226, 570, 248, 496,
                      113, 113, 1, 151, 76, 248, 496, 151, 77, 226, 114, 169, 248, 496,
                      59, 151, 76, 113, 1, 248, 496, 151, 76, 151, 76, 151, 248, 496,
                      76, 151, 77,
                      151, 76, 248, 496, 151, 76, 113, 1, 113, 248, 496, 1, 113, 151, 190, 151, 248, 496,
                      76, 151, 77, 151, 76, 248, 496, 151, 76, 151, 76, 113, 248, 496, 1,
                      113, 1, 113, 340, 248, 496, 1, 151, 77, 151, 76, 248, 496, 151, 76,
                      113, 1, 151, 248, 496, 76, 188, 835, 151, 76, 248, 496, 151, 76, 226, 115,
                      169, 248, 496, 58, 151, 77, 113, 151, 248, 496, 77, 151, 76, 151, 76, 248, 496,
                      151, 76, 151, 77, 151, 248, 496, 76, 113, 1, 113, 113, 248, 496, 1, 151,
                      190, 151, 76, 248, 496, 151, 76, 151, 77, 226, 248, 496, 115, 169, 58, 151, 76, 248, 496,
                      113, 1, 151, 76, 151, 248, 496, 76, 151, 77, 151, 76, 248, 496,
                      113, 113, 1, 113, 1, 248, 496, 226, 569, 75, 1, 75, 248, 496,
                      1, 75, 151, 77, 113, 248, 496, 151, 77, 151, 76, 188, 248, 496, 39, 151, 76,
                      151, 190, 248, 496, 151, 77, 151, 76, 113, 248, 496, 1, 151, 76, 151, 76, 248, 496,
                      188, 39, 151, 190, 151, 248, 496, 77, 151, 76, 151, 76, 248, 496, 151,
                      76, 151, 77, 151, 248, 496, 76, 113, 1, 113, 113, 248, 496, 1, 151, 303, 340, 1, 248, 496,
                      151, 77, 151, 76, 113, 248, 496, 188, 40, 151, 76};
}

void Option2(char optionstate)
{
    oLCD.LCDInitialize(LANDSCAPE, false);

    char text[20]; // create character array
    sprintf(text, "PAN ANGLE");
    oLCD.print(text, CENTER, 30);          // output text(PAN ANGLE) to screen
    oLCD.printNumF(pangle, 4, CENTER, 45); // output pan angle number to screen

    sprintf(text, "TILT ANGLE");
    oLCD.print(text, CENTER, 30);          // output text(tilt ANGLE) to screen
    oLCD.printNumF(tangle, 4, CENTER, 45); // output tilt angle number to screen

    Servo objservo;            // Declare servo as object
    servo.setPeriodHertz(50);  // Set servo frequency to 50hz
    objservo.attach(servoPin); // attaching the servopin to the object

    objservo.write(90); // move servo to 90 degree position
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
            while (displayDuty.length() < 3) displayDuty = "0" + displayDuty;

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
        if (key == KEY_MENU || key == KEY_PLAY_PAUSE)
        {
            // Ramp Down Sequence
            for (int i = targetPwm; i >= 0; i--)
            {
                ledcWrite(PWM_CHANNEL, i);
                delay(10);
            }
            
            ledcWrite(PWM_CHANNEL, 0); // Ensure motor is stopped
            return key; // Tell Option3() which key caused the exit
        }
    }
}

void IRAM_ATTR countPulse()
{
    pulseCount++; // increment on each rising edge
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
   if (duty < 0) duty = 0;
    if (duty > 100) duty = 100;
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
        // Run motor
        runMotor(duty);

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