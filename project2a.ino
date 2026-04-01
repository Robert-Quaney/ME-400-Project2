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
//led pins defined for option 1
const int LED1 = 19;
const int LED2 = 20;
const int LED3 = 21;
const int LED4 = 22;

const int SW1_PIN = 18;
const int SW2_PIN = 17;
const int SW3_PIN = 16;
const int SW4_PIN = 15;

const int SPEAKER_PIN = 8;

const int MAX_CS_PIN = 10;
const int MAX_CLK_PIN = 12;
const int MAX_SO_PIN = 13;
//MOSFET PIN
const int PWM_PIN = 11;
// optocoupler
const int OPTO_PIN = 38;
// tilt and pan 
const int TILT_SERVO_PIN = 35;
const int PAN_SERVO_PIN = 36; 

//DAN- add lines 35-40 to finish main document steps 13-15
//Speaker channel
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

int pangle = 90;//angle to 90 degrees
int tangle = 90;//angle to 90 degrees
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
    //led pins
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);



    // switch pin inputs
    pinMode(SW1_PIN, INPUT_PULLUP);
    pinMode(SW2_PIN, INPUT_PULLUP);
    pinMode(SW3_PIN, INPUT_PULLUP);
    pinMode(SW4_PIN, INPUT_PULLUP);

    // speaker pin 
    pinMode(SPEAKER_PIN, OUTPUT);

    // MAX6675 pins
    pinMode(MAX_CS_PIN,OUTPUT);
    pinMode(MAX_CLK_PIN,OUTPUT);
    pinMode(MAX_SO_PIN,INPUT);
    digitalWrite(MAX_CS_PIN, HIGH);
    
    ESP32PWM::allocateTimer(3); // set servo objects to 20 milliseconds
    // MOSFET Pin
    pinMode(PWM_PIN,OUTPUT);

    // optocouple pin
    pinMode(OPTO_PIN,INPUT);
    
    //tilt and pan servo pins
    pservo.setPeriodHertz(50);
    tservo.setPeriodHertz(50);
    pinMode(TILT_SERVO_PIN,OUTPUT);
    pinMode(PAN_SERVO_PIN,OUTPUT);
    tservo.attach(TILT_SERVO_PIN);
    pservo.attach(PAN_SERVO_PIN);
    tservo.write(90);
    pservo.write(90);
    delay(500);
    tservo.detach();
    pservo.detach();

    //call to setup & attachpin correction
    ledcSetup(SPK_CHANNEL, 496, 8);
    ledcAttachPin(SPEAKER_PIN, SPK_CHANNEL);

    
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
                HandleGamepad();
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
                tservo.detach(); // detaching servos when leaving option2
                pservo.detach();
                servosAttached = false; // setting servos to detached
                ShowDisplay(SC_MAIN, ' ', ' ', false);
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
    // set background color
    oLCD.fillscreen(RCB_PURPLE);
    // print music playing
    oLCD.print("MUSIC PLAYING", CENTER, 10);
    oLCD.print(".....", CENTER, 30);


    //not sure if this is how to call the playsong
    //if(Playing()){
       // playSong();

    //}
    //else{

    //}
    bool songFinished = playSong();

    if(songFinished == true){

    }else {

    }

}
bool playSong(){
    // song contains the sounds 
    // note contains the length
    //uint16_t song[1] = {13}; temp array because the actual array is over 1400 elements
    //uint16_t note[1] = {14}; temp array because the actual array is over 1400 elements

    uint16_t song[] = {659,0,349,261,0,659,622,740,0,587,0,349,0,622,740,587,0,466,587,0,622,740,523,0,349,0,261,622,740,0,523,0,466,0,622,740,311,0,466,0,391,622,740,0,466,0,659,0,622,740,349,0,261,0,659,622,740,0,587,0,349,0,622,740,587,0,466,0,587,622,740,0,523,0,349,0,622,740,261,0,293,0,311,622,740,0,329,0,293,0,622,740,293,0,329,0,349,622,740,0,523,0,587,0,622,740,523,0,587,0,523,622,740,0,466,440,0,391,622,740,349,0,261,0,293,622,740,0,311,0,329,0,622,740,293,0,293,0,329,622,740,349,0,523,0,698,622,740,0,587,0,466,0,622,740,440,0,391,349,0,622,740,261,277,0,293,0,622,740,311,0,329,0,293,622,740,0,293,0,329,0,622,740,349,0,523,0,587,622,740,0,523,0,587,0,622,740,523,0,466,0,440,622,740,0,391,349,0,523,622,740,0,698,0,587,0,622,740,523,0,466,0,440,622,740,0,466,0,493,523,622,740,0,587,0,440,0,622,740,349,0,293,0,329,622,740,0,349,0,311,0,622,740,329,0,349,0,311,622,740,0,311,0,349,369,622,740,0,554,0,622,0,622,740,554,0,622,0,554,622,740,0,493,0,466,415,622,740,0,369,0,277,0,622,740,311,0,329,0,349,622,740,0,311,0,311,0,622,740,349,0,369,0,554,622,740,0,739,0,622,0,622,740,493,466,0,415,0,622,740,369,0,415,0,391,622,740,0,369,329,0,415,622,740,493,0,622,0,739,622,740,0,622,0,493,0,622,740,415,0,329,0,415,622,740,0,493,0,622,0,622,740,739,0,622,0,369,622,740,0,415,0,440,0,622,740,415,0,369,0,293,622,740,0,277,0,293,311,622,740,0,329,0,587,0,622,740,554,0,493,0,440,622,740,415,0,329,0,329,622,740,0,369,0,415,0,622,740,349,0,440,0,523,622,740,0,659,0,783,0,622,740,659,0,523,0,440,622,740,0,349,0,440,523,622,740,0,659,0,783,0,622,740,659,0,391,0,440,622,740,0,466,0,440,0,622,740,391,0,349,0,293,622,740,311,0,329,0,349,622,740,0,349,0,311,0,622,740,349,0,311,261,0,622,740,220,0,466,0,391,622,740,0,466,0,440,466,622,740,0,440,0,466,523,622,740,0,587,0,659,0,622,740,698,0,493,493,0,622,740,523,0,554,0,523,622,740,0,587,0,466,0,622,740,587,0,587,0,523,622,740,0,587,0,622,0,622,740,587,0,659,0,523,622,740,0,659,0,659,0,622,740,587,0,659,0,698,622,740,0,440,0,440,0,622,740,391,0,391,0,391,622,740,0,349,0,349,0,622,740,329,0,329,0,329,622,740,0,523,0,523,0,622,740,466,0,466,0,466,622,740,0,415,0,415,0,622,740,369,0,369,0,369,622,740,0,261,0,277,0,622,740,293,0,277,261,0,622,740,277,0,293,0,277,622,740,0,261,0,293,0,622,740,311,329,0,349,0,622,740,369,0,391,415,0,622,740,440,0,466,0,493,622,740,523,0,587,0,622,622,740,0,659,0,587,0,622,740,587,0,659,0,698,622,740,0,523,0,587,0,622,740,523,0,587,0,523,622,740,0,466,0,440,0,622,740,391,349,0,261,0,622,740,293,0,311,0,329,622,740,0,293,0,293,0,622,740,329,0,349,0,523,622,740,0,698,0,587,0,622,740,466,0,440,391,0,622,740,349,0,261,277,0,622,740,293,0,311,0,329,622,740,0,293,0,293,0,622,740,329,349,0,523,0,622,740,587,0,523,0,587,622,740,0,523,0,466,0,622,740,440,391,0,349,0,622,740,523,0,698,0,587,622,740,0,523,0,466,0,622,740,440,0,466,0,493,622,740,523,0,587,0,440,622,740,0,349,0,293,0,622,740,329,0,349,0,311,622,740,0,329,0,349,0,622,740,311,0,311,0,349,622,740,369,0,554,0,622,622,740,0,554,0,622,0,622,740,554,0,493,0,466,622,740,415,0,369,0,277,622,740,0,311,0,329,0,622,740,349,0,311,0,311,622,740,0,349,0,369,0,622,740,554,0,739,0,622,622,740,0,493,466,0,415,622,740,0,369,0,415,0,622,740,391,0,369,329,0,622,740,415,493,0,622,0,622,740,739,0,622,0,493,622,740,0,415,0,329,0,622,740,415,493,0,622,0,622,740,739,0,622,0,369,622,740,0,415,0,440,0,622,740,415,0,369,0,293,622,740,0,277,293,0,311,622,740,0,329,0,587,0,622,740,554,0,493,0,440,622,740,415,0,329,0,329,622,740,0,369,0,415,0,622,740,349,0,440,0,523,622,740,0,659,0,783,0,622,740,659,0,523,0,440,622,740,0,349,0,440,523,622,740,0,659,0,783,0,622,740,659,0,391,0,440,622,740,0,466,0,440,0,622,740,391,0,349,0,293,622,740,311,0,329,349,0,622,740,349,0,311,0,349,622,740,311,0,261,0,220,622,740,0,466,0,391,0,622,740,466,0,440,466,0,622,740,440,0,466,523,0,622,740,587,0,659,0,698,622,740,0,493,493,0,523,622,740,0,554,0,523,0,622,740,587,0,466,0,587,622,740,0,587,0,523,0,622,740,587,0,622,0,587,622,740,0,659,0,523,0,622,740,659,0,659,0,587,622,740,0,659,0,698,0,622,740,440,0,440,0,391,622,740,0,391,0,391,0,622,740,349,0,349,0,329,622,740,0,329,0,329,0,622,740,523,0,523,0,466,622,740,0,466,0,466,0,622,740,415,0,415,0,369,622,740,0,369,0,369,0,622,740,261,0,277,0,293,622,740,0,277,261,0,277,622,740,0,293,0,277,0,622,740,261,0,293,311,0,622,740,329,0,349,0,369,622,740,391,0,415,0,440,622,740,0,466,493,0,523,622,740,0,587,0,622,0,622,740,659,0,587,0,587,622,740,0,659,0,698,0,622,740,523,0,587,0,523,622,740,0,587,0,523,0,622,740,466,0,440,0,391,622,740,349,0,523,0,587,622,740,0,622,0,659,0,622,740,587,0,587,0,659,622,740,0,698,0,523,0,622,740,698,0,587,0,466,622,740,0,440,391,0,349,622,740, 
                        };
    uint16_t note[] = {113,114,113,113,115,113,248,496,114,113,114,113,115,248,496,113,114,113,113,115,248,496,113,114,113,1,113,248,496,114,113,114,113,114,248,496,113,115,113,114,113,248,496,1,113,114,113,114,248,496,113,1,113,114,113,248,496,114,113,115,113,114,248,496,113,114,113,1,113,248,496,114,113,114,113,1,248,496,113,1364,151,77,151,248,496,76,226,115,169,58,248,496,151,76,113,1,151,248,496,76,151,76,151,77,248,496,151,76,151,76,151,248,496,77,113,113,1,113,248,496,151,190,151,77,151,248,496,76,151,76,226,115,248,496,169,58,151,77,113,248,496,151,77,151,76,151,248,496,76,151,76,113,1,248,496,113,1,113,226,570,248,496,113,113,1,151,76,248,496,151,77,226,114,169,248,496,59,151,76,113,1,248,496,151,76,151,76,151,248,496,76,151,77,151,76,248,496,151,76,113,1,113,248,496,1,113,151,190,151,248,496,76,151,77,151,76,248,496,151,76,151,76,113,248,496,1,113,1,113,340,248,496,1,151,77,151,76,248,496,151,76,113,1,151,248,496,76,188,835,151,76,248,496,151,76,226,115,169,248,496,58,151,77,113,151,248,496,77,151,76,151,76,248,496,151,76,151,77,151,248,496,76,113,1,113,113,248,496,1,151,190,151,76,248,496,151,76,151,77,226,248,496,115,169,58,151,76,248,496,113,1,151,76,151,248,496,76,151,77,151,76,248,496,113,113,1,113,1,248,496,226,569,75,1,75,248,496,1,75,151,77,113,248,496,151,77,151,76,188,248,496,39,151,76,151,190,248,496,151,77,151,76,113,248,496,1,151,76,151,76,248,496,188,39,151,190,151,248,496,77,151,76,151,76,248,496,151,76,151,77,151,248,496,76,113,1,113,113,248,496,1,151,303,340,1,248,496,151,77,151,76,113,248,496,188,40,151,76,113,248,496,1,151,76,151,76,248,496,151,76,113,1,151,248,496,76,151,77,188,39,248,496,151,76,151,190,151,248,496,76,151,77,113,151,248,496,77,151,76,188,39,248,496,151,190,151,76,151,248,496,77,151,76,151,76,248,496,151,76,151,77,113,248,496,113,1,113,1,151,248,496,417,151,76,75,1,248,496,75,1,75,113,1,248,496,113,114,226,1,113,248,496,1,113,1,113,113,248,496,1,113,1,56,56,248,496,1,56,1,56,1,248,496,1817,456,454,302,1,248,496,302,1,302,1,63,248,496,2,616,1,226,1,248,496,908,683,151,77,302,248,496,1,302,1,302,1,248,496,63,1,616,1,226,248,496,2,908,683,151,76,248,496,302,1,302,1,302,248,496,1,113,342,113,227,248,496,169,59,169,172,113,248,496,341,113,342,113,228,248,496,169,58,169,172,113,248,496,341,113,342,113,228,248,496,169,58,169,172,113,248,496,342,113,341,113,228,248,496,169,58,169,172,113,248,496,2160,169,1,56,1,248,496,169,2,56,169,2,248,496,56,1,169,1,56,248,496,1,75,1,75,1,248,496,75,75,1,75,1,248,496,75,1,75,75,1,248,496,75,1,75,1,75,248,496,75,1,151,76,151,248,496,77,226,115,169,58,248,496,151,76,113,1,151,248,496,76,151,76,151,77,248,496,151,76,151,76,151,248,496,76,113,1,113,1,248,496,113,151,190,151,76,248,496,151,77,151,76,226,248,496,115,169,58,151,76,248,496,113,1,151,76,151,248,496,77,151,76,151,76,248,496,113,1,113,113,1,248,496,226,570,113,113,1,248,496,151,76,151,76,226,248,496,115,169,59,151,76,248,496,113,151,77,151,76,248,496,151,76,151,77,151,248,496,76,151,76,113,1,248,496,113,113,1,151,190,248,496,151,76,151,77,151,248,496,76,151,76,151,76,248,496,113,1,113,1,113,248,496,340,1,151,76,151,248,496,77,151,76,113,1,248,496,151,76,188,835,151,248,496,76,151,76,226,115,248,496,169,58,151,77,113,248,496,151,77,151,76,151,248,496,76,151,76,151,77,248,496,151,76,113,1,113,248,496,113,1,151,190,151,248,496,76,151,76,151,77,248,496,226,114,169,59,151,248,496,76,113,1,151,76,248,496,151,76,151,76,151,248,496,77,113,113,1,113,248,496,1,226,569,75,1,248,496,75,1,75,151,77,248,496,113,151,77,151,76,248,496,188,39,151,76,151,248,496,190,151,77,151,76,248,496,113,151,77,151,76,248,496,188,39,151,190,151,248,496,76,151,77,151,76,248,496,151,76,151,77,151,248,496,76,113,113,1,113,248,496,1,151,303,340,1,248,496,151,76,151,77,113,248,496,188,40,151,76,113,248,496,1,151,76,151,76,248,496,151,76,113,1,151,248,496,76,151,77,188,39,248,496,151,76,151,190,151,248,496,76,151,77,113,151,248,496,76,151,77,188,39,248,496,151,190,151,76,151,248,496,76,151,77,151,76,248,496,151,76,151,77,113,248,496,113,1,113,151,418,248,496,151,76,75,1,75,248,496,75,1,113,1,113,248,496,114,226,1,113,1,248,496,113,1,113,113,1,248,496,113,1,56,56,1,248,496,56,1,56,1,1817,248,496,456,454,302,1,302,248,496,1,302,1,63,2,248,496,616,1,226,1,908,248,496,683,151,76,302,1,248,496,302,2,302,1,63,248,496,1,616,1,226,2,248,496,908,683,151,76,302,248,496,1,302,1,302,1,248,496,113,341,113,228,169,248,496,59,169,172,113,341,248,496,113,342,113,228,169,248,496,58,169,172,113,341,248,496,113,342,113,228,169,248,496,58,169,172,113,341,248,496,113,342,113,228,169,248,496,58,169,172,113,2160,248,496,169,1,56,1,169,248,496,2,56,169,2,56,248,496,1,169,1,56,1,248,496,75,1,75,75,1,248,496,75,1,75,1,75,248,496,75,1,75,1,75,248,496,1,75,75,1,75,248,496,1,151,76,151,77,248,496,226,114,169,59,151,248,496,76,113,1,151,76,248,496,151,76,151,76,151,248,496,77,151,76,151,76,248,496,113,1,113,1,113,248,496,151,190,151,76,151,248,496,77,151,76,226,115,248,496,169,58,151,76,113,248,496,1,151,76,151,77,248,496,151,76,151,76,113,248,496,1,113,113,1,340,248,496, 

                         };


    int N = sizeof(song)/sizeof(song[0]); // # elements in array song is 1439 and notes is 1414, use size of operato
    
    // NEEDS MORE HELP TO GET SKIPPING RIGHT
    for(int i = 0; i < N; i++){
        if((i-6) % 7 == 0 || (i - 6) % 7 == 1 ){
            if (i >= 6) continue; 
        }
        int duration = note[i];

        // step 5 - for varing speed
        if(digitalRead(SW1_PIN) == LOW){
            duration = duration / 2; 
        }

        else if(digitalRead(SW3_PIN) == LOW){
            duration = duration*2;  
        }
        //step 5a - leds blink with notes
        int ledIndex = i%4; 

        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);

        if(ledIndex == 0) digitalWrite(LED1, HIGH);
        if(ledIndex == 1) digitalWrite(LED2, HIGH);
        if(ledIndex == 2) digitalWrite(LED3, HIGH);
        if(ledIndex == 3) digitalWrite(LED4, HIGH);

        //5b + c - playing song
        ledcWriteTone(SPK_CHANNEL, song[i]);
        delay(duration);
        
        //5c + d: cleaning up
        ledcWriteTone(SPK_CHANNEL, 0);

        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);


        // exits song when sw4 pressed
        if(digitalRead(SW4_PIN) == LOW){
            ledcWriteTone(SPK_CHANNEL, 0);
            digitalWrite(LED1, LOW);
            digitalWrite(LED2, LOW);
            digitalWrite(LED3, LOW);
            digitalWrite(LED4, LOW);

            oLCD.clrScr();
            oLCD.print("MSC STOPPED", CENTER, 10);
            oLCD.print("PRESS", CENTER, 30);
            oLCD.print("<MODE>", CENTER, 50);
            oLCD.print("TO GO BACK", CENTER, 70);


            return false;
        }
    }
    //song plays all the way through
    oLCD.clrScr();
    oLCD.print("MSC FINISHED", CENTER, 10);
    oLCD.print("PRESS", CENTER, 30);
    oLCD.print("<RETURN>", CENTER, 50);
    oLCD.print("TO GO BACK", CENTER, 70);


    ledcWriteTone(SPK_CHANNEL, 0);

    return true; //song finished

}
    

//End of option 1

void AttachServos()//initialize a function to attach servos
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

    sprintf(text, "PRESS <RETURN>")
        oLCD.print(text, CENTER, 90); // output "PRESS <RETURN>" to the screen

    sprintf(text, "TO GO BACK")
        oLCD.print(text, CENTER, 105); // output "TO GO BACK" to the screen
}
void HandleGamepad()
{
    Dabble.processInput(); // starts collecting data from dabble

    if (GamePad.isUpPressed()) // Up button pressed
    {
        tangle += 5           // increase tilt angle by 5 degrees
            if (tangle > 165) // see if tilt angle is going above maximum
        {
            tangle = 165; // set tilt angle at maximum
        }
        tservo.write(tangle); // write the tilt angle to the specified amount
    }
    if (Gamepad.isDownPressed()) // Down button is pressed
    {
        tangle -= 5          // decrease tilt angle by 5 degrees
            if (tangle < 15) // see if tilt angle is going below minimum
        {
            tangle = 15; // set tilt angle at minimum
        }
        tservo.write(tangle);
    }
    if (Gamepad.isLeftPressed()) // Left button is pressed
    {
        pangle += 5;      // pan 5 degrees to the left
        if (pangle > 165) // see if pan angle is at the max
        {
            pangle = 165; // set pan angle to max if going above
        }
        pservo.write(pangle); // move servo to specified angle
    }
    if (Gamepad.isRightPressed()) // Right button is pressed
    {
        pangle -= 5;     // pan 5 degrees to the right
        if (pangle < 15) // see if the pan angle is at the min
        {
            pangle = 15; // set pan angle at min if going below
        }
        pservo.write(pangle); // move servo to specified angle
    }
    // update the display of new angles
    ServoScreenUpdate() // calls function to update ther servo screen

    ////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Option2(char optionstate)
{
    ////////////////////////////////////////////////////////////////////////////////////
    AttachServos();//call function to attach servos
    oLCD.LCDInitialize(LANDSCAPE, false);
    //////////////////////////////////////////////////////////
    char = text[20]; // create character array
    sprintf(text, "PAN ANGLE");
    oLCD.print(text, CENTER, 30);          // output text(PAN ANGLE) to screen
    oLCD.printNumF(pangle, 4, CENTER, 45); // output pan angle number to screen

    sprintf(text, "TILT ANGLE");
    oLCD.print(text, CENTER, 60);          // output text(tilt ANGLE) to screen
    oLCD.printNumF(tangle, 4, CENTER, 75); // output tilt angle number to screen

    sprintf(text, "PRESS <RETURN>")
        oLCD.print(text, CENTER, 90); // output "PRESS <RETURN>" to the screen

    sprintf(text, "TO GO BACK")
        oLCD.print(text, CENTER, 105); // output "TO GO BACK" to the screen
    ////////////////////////////////////////////////////////////
    if ()
        pservo.attach(PAN_SERVO_PIN);
    tservo.attach(TILT_SERVO_PIN);
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
