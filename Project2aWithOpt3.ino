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

// optocoupler
const int OPTO_PIN = 38;

// pan and tilt
const int PAN_SERVO_PIN = 36;
const int TILT_SERVO_PIN = 35;

// DAN- add lines 35-40 to finish main document steps 13-15
// Speaker channel
const int SPK_CHANNEL = 1;

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
void IRAM_ATTR PWM_ISR();
const int interruptPinPWM = 37; // Use your optocoupler pin (same as OPTO_PIN)

volatile unsigned long pwmcount = 0;
volatile unsigned long tonecount = 0;

double pwmfrequency = 0.0;

unsigned long lastTime = 0;

const int pulsesPerRevolution = 20;
static String aval = "";
// MOSFET PIN
const int PWM_PIN = 11;
const int pwmFreq = 496; /* 5 KHz */
// PWM channel
const int PWM_CHANNEL = 2;
const int pwmResolution = 8;

bool motorRunning = false;
double desiredDuty = 0.0; // duty cycle entered by user (0–100)

// for three 33 3 3 3

//
//  Setup()
//
void setup()
{
    Serial.begin(115200);
    // Dabble.begin("Group3");
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
    // MOSFET Pin
    pinMode(PWM_PIN, OUTPUT);
    // Setup PWM
    ledcSetup(PWM_CHANNEL, pwmFreq, pwmResolution);
    // Attach the LED PWM Channel to the GPIO Pin
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    ledcWrite(PWM_CHANNEL, 0);

    pinMode(interruptPinPWM, INPUT_PULLUP);

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
        motorRunning = false;
        detachInterrupt(digitalPinToInterrupt(interruptPinPWM));
        ledcWrite(PWM_CHANNEL, 0); // stop motor
        aval = "";                 // reset duty entry
        ShowDisplay(SC_MAIN, ' ', ' ', false);
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
            }
            else if (last_key_processed == KEY_3)
            {
                ShowDisplay(SC_SUB3, 'A', last_key_processed, false);
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
            if (last_key_processed == KEY_MENU)
            {
                motorRunning = false;
                detachInterrupt(digitalPinToInterrupt(interruptPinPWM));
                ledcWrite(PWM_CHANNEL, 0); // stop motor
                aval = "";                 // reset duty entry
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
            else if (last_key_processed == KEY_PLAY) // Play/Pause key
            {
                motorRunning = false;
                detachInterrupt(digitalPinToInterrupt(interruptPinPWM));
                ledcWrite(PWM_CHANNEL, 0);            // stop motor
                aval = "";                            // reset duty entry
                ShowDisplay(SC_SUB3, 'A', ' ', true); // go back to duty entry
            }
            // If the user presses one of the numeric keys, then display the key in the correct place on the screen.
            if (last_key_processed == KEY_0 || last_key_processed == KEY_1 || last_key_processed == KEY_2 || last_key_processed == KEY_3 || last_key_processed == KEY_4 ||
                last_key_processed == KEY_5 || last_key_processed == KEY_6 || last_key_processed == KEY_7 || last_key_processed == KEY_8 || last_key_processed == KEY_9)
            {
                // Serial.print("t=");Serial.println(oLCD.OPTION_STATE);
                if ((oLCD.OPTION_STATE == 'A') || (oLCD.OPTION_STATE == 'B'))
                {
                    ShowDisplay((screen)oLCD.SCREEN_STATE, 'B', oIR.TranslateKey(last_key_processed), false);
                }
                else if ((oLCD.OPTION_STATE == 'C'))
                {
                    // display PWM frequency and value
                    oLCD.printNumF(pwmfrequency, 4, CENTER, 80);
                }
            }
            else if (last_key_processed == KEY_RETURN)
            {
                if (oLCD.OPTION_STATE == 'B')
                {
                    ShowDisplay((screen)oLCD.SCREEN_STATE, 'C', ' ', false);
                }
                else if (oLCD.OPTION_STATE == 'C')
                {
                    // return to A
                    ShowDisplay((screen)oLCD.SCREEN_STATE, 'A', ' ', false);
                }
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
        delay(100);
    }

    if (motorRunning)
    {
        float rpm = measureSpeed();
        oLCD.printNumF(rpm, 1, CENTER, 80);
    }
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

    double dutycalc = 0.0;

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
        Option3(oLCD.OPTION_STATE, ' ');
        if (optionstate == 'A')
        {
            aval = "";
            oLCD.LCDInitialize(LANDSCAPE, initdisplay);
            oLCD.print(F("DESIRED DUTY"), CENTER, 10);
            oLCD.print(F("CYCLE (%)"), CENTER, 25);
            oLCD.setBackColor(RCB_BLACK);
            oLCD.print(F("   "), CENTER, 40);
            oLCD.setBackColor(RCB_PURPLE);

            oLCD.print(F("CALC'D SPEED"), CENTER, 65);
            oLCD.print(F("0"), CENTER, 80);
        }
        else if (optionstate == 'B')
        {
            // Limit value of a to 2 digits
            if (aval.length() <= 2)
            {
                aval = aval + keypressed;
            }
            oLCD.setBackColor(RCB_BLACK);
            oLCD.printNumI(aval.toInt(), CENTER, 40, 3, ' ');
            oLCD.setBackColor(RCB_PURPLE);
        }
        else if (optionstate == 'C')
        {

            oLCD.print(F("C"), CENTER, 100);
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
    ledcWrite(PWM_CHANNEL, 0);
    if (optionstate == 'C') // user pressed return after entering duty cycle
    {
        if (aval.length() > 0)
        {
            desiredDuty = aval.toInt(); // convert entered string to number
            if (desiredDuty > 100)
                desiredDuty = 100; // limit duty cycle
            if (desiredDuty < 0)
                desiredDuty = 0;

            // Start motor at desired duty
            int pwmVal = map(desiredDuty, 0, 100, 0, 255); // 8-bit PWM
            ledcWrite(PWM_CHANNEL, pwmVal);

            pwmcount = 0;

            attachInterrupt(digitalPinToInterrupt(interruptPinPWM), PWM_ISR, FALLING);
            motorRunning = true;
        }
    }
}

void IRAM_ATTR PWM_ISR()
{
    pwmcount++;
}

void Tone_ISR()
{
    // increase tone count
    tonecount++;
}

float measureSpeed()
{
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 1000.0;

    if (elapsedTime <= 0)
        return 0.0;

    float rpm = (pwmcount / pulsesPerRevolution) * 60.0 / elapsedTime;

    pwmcount = 0;
    lastTime = currentTime;

    return rpm;
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
