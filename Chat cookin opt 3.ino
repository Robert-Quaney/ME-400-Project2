void DrawOption3Screen()
{
    oLCD.LCDInitialize(LANDSCAPE, false);

    oLCD.print("DESIRED DUTY", 10, 10);
    oLCD.print("CYCLE (%)", 10, 25);
    oLCD.print("000", 10, 40);

    oLCD.print("CALC'D SPEED", 10, 70);
    oLCD.print("0.0", 10, 85);
}

void UpdateDutyDisplay(int duty)
{
    // Clear old value
    oLCD.print("    ", 10, 40);

    // Print as integer
    oLCD.printNumI(duty, 10, 40, 3, '0');
}

void ClearDutyDisplay()
{
    oLCD.print("    ", 10, 40);
}

void UpdateSpeedDisplay(float speed)
{
    // Clear old speed
    oLCD.print("        ", 10, 85);

    // Show one decimal place
    oLCD.printNumF(speed, 1, 10, 85);
}

int dutyToPWM(int duty)
{
    if (duty < 0) duty = 0;
    if (duty > 100) duty = 100;
    return map(duty, 0, 100, 0, 255);
}

void IRAM_ATTR countPulse()
{
    pulseCount++;
}

float measureSpeed()
{
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 1000.0;

    if (elapsedTime <= 0)
        return 0.0;

    float rpm = (pulseCount / (float)PULSES_PER_REV) * 60.0 / elapsedTime;

    pulseCount = 0;
    lastTime = currentTime;

    return rpm;
}

int GetDutyFromIR()
{
    int duty = 0;
    unsigned long key = 0;
    unsigned long lastKey = 0;

    ClearDutyDisplay();

    while (true)
    {
        key = oIR.GetKeyPressed();

        if (key != 0 && key != lastKey)
        {
            if (key >= KEY_0 && key <= KEY_9)
            {
                int digit = key - KEY_0;

                // build number from digits entered
                duty = duty * 10 + digit;

                // cap at 999 just so display doesn't run away
                if (duty > 999)
                    duty = 999;

                UpdateDutyDisplay(duty);
            }
            else if (key == KEY_RETURN)
            {
                return duty;
            }
        }

        lastKey = key;
        delay(100);
    }
}

unsigned long RunMotorUntilExit(int duty)
{
    int pwmValue = dutyToPWM(duty);

    // Ramp up
    for (int i = 0; i <= pwmValue; i++)
    {
        ledcWrite(PWM_CHANNEL, i);
        delay(10);
    }

    while (true)
    {
        ledcWrite(PWM_CHANNEL, pwmValue);

        float speed = measureSpeed();
        UpdateSpeedDisplay(speed);

        unsigned long key = oIR.GetKeyPressed();

        if (key == KEY_MENU || key == KEY_PLAY_PAUSE)
        {
            // Ramp down
            for (int i = pwmValue; i >= 0; i--)
            {
                ledcWrite(PWM_CHANNEL, i);
                delay(10);
            }

            ledcWrite(PWM_CHANNEL, 0);
            return key;
        }

        delay(1000);
    }
}

void Option3(char optionstate, char keypressed)
{
    while (true)
    {
        DrawOption3Screen();

        int duty = GetDutyFromIR();

        if (duty < 0) duty = 0;
        if (duty > 100) duty = 100;

        UpdateDutyDisplay(duty);

        unsigned long exitKey = RunMotorUntilExit(duty);

        if (exitKey == KEY_MENU)
        {
            ShowDisplay(SC_MAIN, ' ', ' ', false);
            break;
        }

        if (exitKey == KEY_PLAY_PAUSE)
        {
            // stay in Option 3 and allow a new duty entry
            DrawOption3Screen();
        }
    }
}
