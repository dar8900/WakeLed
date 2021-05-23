#include "../headers/mainApp.h"
#include <ctime>

#define METEO_X_START   92

#define MAX_MENU_VOICES_ONLINE 7
#define MAX_MENU_VOICES_OFFLINE 9

#define TO_RADIANTS(angle)  (DEG_TO_RAD * angle)

DispString menuVoicesOnline[MAX_MENU_VOICES_ONLINE] = 
{
    "Imposta allarme",
    "Imposta tempo led",
    "Imposta snooze all.",
    "Imposta riavvio all.",
    "Tempo backlight",
    "Info meteo",
    "Luminosita display",
};

DispString menuVoicesOffline[MAX_MENU_VOICES_OFFLINE] = 
{
    "Imposta allarme",
    "Imposta tempo led",
    "Imposta snooze all.",
    "Imposta riavvio all.",
    "Tempo backlight",
    "Info meteo",
    "Luminosita display",
    "Imposta ora",
    "Imposta data",
};

void WAKE_LED::manageAlarmLed()
{
    uint32_t PreAccensionSeconds = preAccensionTime * 60;
    uint8_t ActualHour = 0, ActualMinute = 0;
    uint32_t AlarmTimestamp = 0;
    AlarmTimestamp = wakeLedAlarm->getAlarmTimestamp();
    if(wakeLedAlarm->isAlarmSet() && PreAccensionSeconds != 0 && !wakeLedAlarm->isAlarmSnoozed())
    {   
        uint32_t TsDiff = AlarmTimestamp - wifiStation->timeDateInfo.timestamp;
        if(TsDiff <= PreAccensionSeconds && TsDiff != 0)
        {
            if(preAccensionTimer->hasPassed(1, true))
            {
                if(accensionLedPwmIncrement < LEDS::PWM_RANGE)
                {
                    accensionLedPwmIncrement += (LEDS::PWM_RANGE / PreAccensionSeconds);
                    alarmLed->writePwm(accensionLedPwmIncrement);
                    // WakeledDebug.writeDebugString("Valore pwm: " + DebugString(accensionLedPwmIncrement));
                }
            }
        }
        else
        {
            accensionLedPwmIncrement = 0;
            preAccensionTimer->restart();
            alarmLed->writeDigital(OFF);
        }
    }
    else
    {
        preAccensionTimer->restart();
        ledDutyCycle = 0;
        if(!alarmLedManual)
            alarmLed->writePwm(ledDutyCycle);
        accensionLedPwmIncrement = 0;       
    }
}

void WAKE_LED::adjustAutoBrightness()
{
    if(displayBrightnessAuto)
    {
        if(autoBrightnessTimer->hasPassed(600, true))
        {
            time_t ts = (time_t)wifiStation->timeDateInfo.timestamp;
            std::tm *locTime = std::localtime(&ts);
            if(locTime->tm_hour >= 22 && locTime->tm_hour < 9)
            {
                autoBrightnessValue = 5;
            }
            else if(locTime->tm_hour >= 11 && locTime->tm_hour < 17)        
            {
                autoBrightnessValue = 100;
            }
            else if(locTime->tm_hour >= 9 && locTime->tm_hour < 11)    
            {
                autoBrightnessValue = 50;
            }
            else if(locTime->tm_hour >= 17 && locTime->tm_hour < 22)    
            {
                autoBrightnessValue = 25;
            }
            display->setDisplayLedBrightness(autoBrightnessValue);
        }
    }
    else
    {
        autoBrightnessTimer->stop();
        autoBrightnessValue = display->DISPLAY_BRIGHTNESS_DFLT;
    }
}

void WAKE_LED::backGroundTasks()
{
    wifiStation->run();
    wakeLedAlarm->runAlarm(wifiStation->timeDateInfo.timestamp);
    manageAlarmLed();
    display->displayLedManage();
    irSensor->readSensor();
    adjustAutoBrightness();
}

void WAKE_LED::drawTopInfo()
{
    if(wifiStation->isWifiConnected())
    {
        display->drawUnicodeChar(NHDST7565::LEFT_POS, NHDST7565::TOP_POS, NHDST7565::W_8_H_8_ICON, 247);
    }
    if(wakeLedAlarm->isAlarmSet())
    {
        display->drawUnicodeChar(50, NHDST7565::TOP_POS, NHDST7565::W_8_H_8_ICON, 93);
    }
    if(wakeLedAlarm->isAlarmActive())
    {
        display->drawUnicodeChar(22, NHDST7565::TOP_POS, NHDST7565::W_8_H_8_ICON, 84);
    }    
    if(wakeScreen != MAIN_SCREEN)
    {
        display->drawString(NHDST7565::CENTER_POS , NHDST7565::TOP_POS, NHDST7565::W_5_H_8, wifiStation->timeDateInfo.timeFormatted);
    }
}

void WAKE_LED::drawWeatherInfo()
{
    uint16_t IconCode = 0;
    DispString Temp = "", Press = "";
    if(wifiStation->isWifiConnected())
    {
        switch (wifiStation->weatherInfo.weatherID)
        {
        case WIFI_STATION::TEMPESTA:
        case WIFI_STATION::PIOVIGGINE:
        case WIFI_STATION::PIOGGIA:
            IconCode = 67;
            break;
        case WIFI_STATION::NEVE:
            IconCode = 68;
            break;
        case WIFI_STATION::NEBBIA:
            IconCode = 65;
            break;
        case WIFI_STATION::LIMPIDO:
            if((wifiStation->timeDateInfo.dayHour >= 19 && wifiStation->timeDateInfo.dayHour <= 23) || 
                (wifiStation->timeDateInfo.dayHour >= 0 && wifiStation->timeDateInfo.dayHour <= 7)) 
            {
                IconCode = 66;
            }
            else
            {
                IconCode = 69;
            }
            break;
        case WIFI_STATION::NUVOLOSO:
            IconCode = 64;
            break;                                            
        default:
            IconCode = 69;
            break;
        }
        display->drawUnicodeChar(100, 1, NHDST7565::W_16_H_16_WEATHER_ICON, IconCode);
        Temp = DispString(String(wifiStation->weatherInfo.temperature, 1).c_str());
        Press = DispString(String(wifiStation->weatherInfo.pressure, 0).c_str());
        display->drawString(METEO_X_START, 20, NHDST7565::W_5_H_8, "Temp.");
        display->drawString(METEO_X_START, 29, NHDST7565::W_5_H_8, Temp + "C");
        display->drawString(METEO_X_START, 44, NHDST7565::W_5_H_8, Press);
        display->drawString(METEO_X_START, 54, NHDST7565::W_5_H_8, "mbar");
    }
    else
    {
        display->drawString(METEO_X_START, 20, NHDST7565::W_5_H_8, "no");
        display->drawString(METEO_X_START, 29, NHDST7565::W_5_H_8, "info");
        display->drawString(METEO_X_START, 38, NHDST7565::W_5_H_8, "meteo");
    }
    
}

void WAKE_LED::drawAnalogClock()
{
    float Meno90 = (float)TO_RADIANTS(90.0);
    const uint8_t CLOCK_X_CENTER =  43;
    const uint8_t CLOCK_Y_CENTER = 34;
    const uint8_t CLOCK_RADIUS = 25;
    uint8_t ActualHour = 0, ActualMinute = 0, ActualSeconds = 0;
    std::tm *locTime = std::localtime((time_t *)&wifiStation->timeDateInfo.timestamp);
    ActualHour = locTime->tm_hour;
    ActualMinute = locTime->tm_min;
    ActualSeconds = wifiStation->timeDateInfo.timestamp % 60;
    int16_t XPosHour =   (int16_t) (CLOCK_RADIUS - 10) * cos((double)TO_RADIANTS(30 * (ActualHour % 12))- Meno90);
    int16_t YPosHour =   (int16_t) (CLOCK_RADIUS - 10) * sin((double)TO_RADIANTS(30 * (ActualHour % 12))- Meno90); 
    int16_t XPosMinute = (int16_t) (CLOCK_RADIUS - 4) * cos((double)TO_RADIANTS(6 * ActualMinute ) - Meno90);
    int16_t YPosMinute = (int16_t) (CLOCK_RADIUS - 4) * sin((double)TO_RADIANTS(6 * ActualMinute ) - Meno90);
    int16_t XPosSecond = (int16_t) (CLOCK_RADIUS - 2) * cos((double)TO_RADIANTS(6 * ActualSeconds ) - Meno90);
    int16_t YPosSecond = (int16_t) (CLOCK_RADIUS - 2) * sin((double)TO_RADIANTS(6 * ActualSeconds ) - Meno90);
    display->drawCircle(CLOCK_X_CENTER, CLOCK_Y_CENTER, CLOCK_RADIUS, false);
    display->drawCircle(CLOCK_X_CENTER, CLOCK_Y_CENTER, 2, false);

    // ORE
    display->drawAngleLine(CLOCK_X_CENTER, CLOCK_Y_CENTER, CLOCK_X_CENTER + XPosHour,   CLOCK_Y_CENTER + YPosHour);
            
    // MINUTI
    display->drawAngleLine(CLOCK_X_CENTER, CLOCK_Y_CENTER, CLOCK_X_CENTER + XPosMinute, CLOCK_Y_CENTER + YPosMinute);
            
    // SECONDI  
    display->drawAngleLine(CLOCK_X_CENTER, CLOCK_Y_CENTER, CLOCK_X_CENTER + XPosSecond, CLOCK_Y_CENTER + YPosSecond); 
}

void WAKE_LED::mainScreen()
{   
    bool ExitMainScreen = false;
    int8_t Button = ROTARY::NO_ACTION;
    String Seconds = "";
    bool SwitchClockView = false;
    while(!ExitMainScreen)
    {
        Seconds = wifiStation->timeDateInfo.timestamp % 60 < 10 ? "0" + String(wifiStation->timeDateInfo.timestamp % 60) : String(wifiStation->timeDateInfo.timestamp % 60);
        display->clearBuff();
        drawTopInfo();
        if(!SwitchClockView)
        {
            display->drawString(NHDST7565::LEFT_POS, 14, NHDST7565::W_17_H_29, wifiStation->timeDateInfo.timeFormatted);
            display->drawString(37, 40, NHDST7565::W_5_H_8, DispString(Seconds.c_str()));
            display->drawString(19, 52, NHDST7565::W_6_H_10, wifiStation->timeDateInfo.dateFormatted);
        }
        else
        {
            // drawAnalogClock();
            display->drawString(13, 20, NHDST7565::W_6_H_13_B, wifiStation->timeDateInfo.dateFormatted);
            display->drawString(20, 33, NHDST7565::W_6_H_10, wifiStation->timeDateInfo.weekDay);
            String TimeAndSec = String(wifiStation->timeDateInfo.timeFormatted.c_str()) + ":" + Seconds;
            display->drawString(15, 48, NHDST7565::W_6_H_10, DispString(TimeAndSec.c_str()));
        }
        // display->drawString(NHDST7565::CENTER_POS , NHDST7565::TOP_POS, NHDST7565::W_5_H_8, "v" + DispString(VERSION));
        if(wakeLedAlarm->isAlarmSet())
        {
            uint8_t AlarmH = 0, AlarmM = 0;
            wakeLedAlarm->getAlarmTime(AlarmH, AlarmM);
            String AlarmT = AlarmH > 9 ? String(AlarmH) : "0" + String(AlarmH);
            AlarmT += ":";
            AlarmT += AlarmM > 9 ? String(AlarmM) : "0" + String(AlarmM);
            display->drawString(60 , NHDST7565::TOP_POS, NHDST7565::W_5_H_8, AlarmT.c_str());
        }
        drawWeatherInfo();
        display->drawLine(86, 0, display->DISPLAY_HIGH, NHDST7565::VERTICAL);
        display->drawLine(88, 0, display->DISPLAY_HIGH, NHDST7565::VERTICAL);
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
        case ROTARY::INCREMENT:
            SwitchClockView = !SwitchClockView;
            break;
        case ROTARY::BUTTON_PRESS:
            wakeScreen = MENU_SCREEN;
            ExitMainScreen = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            if(wakeLedAlarm->isAlarmSet())
            {
                wakeLedAlarm->resetAlarm();
            }
            else
            {
                if(alarmLed->getPwmValue() == LEDS::PWM_RANGE)
                {
                    alarmLed->writeDigital(OFF);
                    alarmLedManual = false;
                }
                else
                {
                    alarmLed->writeDigital(ON);
                    alarmLedManual = true;
                }
            }
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }
        if(wakeLedAlarm->isAlarmActive())
        {
            ExitMainScreen = true;
            wakeScreen = ALARM_ACTIVE_SCREEN;
        }
        delay(1);
    }
}

void WAKE_LED::alarmActiveScreen()
{
    bool ExitAlarmActive = false;
    int8_t Button = ROTARY::NO_ACTION;
    display->manualManageDisplayLed = true;
    while(!ExitAlarmActive)
    {
        display->clearBuff();
        drawTopInfo();
        display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, "SVEGLIA");
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
        case ROTARY::INCREMENT:
            break;
        case ROTARY::BUTTON_PRESS:
            wakeLedAlarm->resetAlarm();
            ExitAlarmActive = true;
            wakeScreen = MAIN_SCREEN;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            break;
        default:
            break;
        }
        if(irSensor->digitalVal(700, false) == ON)
        {
            wakeLedAlarm->resetAlarm();
            ExitAlarmActive = true;
            wakeScreen = MAIN_SCREEN;
        }
        display->manualSwitchLedDisplay(ON);
        if(!wakeLedAlarm->isAlarmActive())
        {
            ExitAlarmActive = true;
            wakeScreen = MAIN_SCREEN;
            display->manualSwitchLedDisplay(OFF);
            display->manualManageDisplayLed = false;
            display->restartDisplayLedTimer();
        }
        delay(1);
    }   
}

void WAKE_LED::menu()
{
    bool ExitMenu = false;
    int8_t Button = ROTARY::NO_ACTION;
    uint8_t ItemSel = oldMenuItem, TopItem = 0;
    const uint8 MaxItemList = 4;
    uint8_t MenuVoices = 0;
    if(wifiStation->isWifiConnected())
    {
        MenuVoices = MAX_MENU_VOICES_ONLINE;
    }
    else
    {
        MenuVoices = MAX_MENU_VOICES_OFFLINE;
    }
    while(!ExitMenu)
    {
        display->clearBuff();
        drawTopInfo();
        for(int MenuItem = 0; MenuItem < MaxItemList; MenuItem++)
        {
            uint8_t NextItem = TopItem + MenuItem;
            if(NextItem >= MenuVoices)
                break;
            if(wifiStation->isWifiConnected())
            {
                display->drawString(8, 14 + (12 * MenuItem), NHDST7565::W_6_H_10, menuVoicesOnline[NextItem]); 
            }
            else
            {
                display->drawString(8, 14 + (12 * MenuItem), NHDST7565::W_6_H_10, menuVoicesOffline[NextItem]); 
            }
            if(NextItem == ItemSel)
            {
                display->drawCircle(4, 17 + (12 * MenuItem), 2, true);
            }
        }
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(ItemSel > 0)
                ItemSel--;
            else
                ItemSel = MenuVoices - 1;
            break;
        case ROTARY::INCREMENT:
            if(ItemSel < MenuVoices - 1)
                ItemSel++;
            else
                ItemSel = 0;        
            break;
        case ROTARY::BUTTON_PRESS:
            wakeScreen = ItemSel + ALARM_SCREEN;
            oldMenuItem = ItemSel;
            ExitMenu = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitMenu = true;
            oldMenuItem = 0;
            wakeScreen = MAIN_SCREEN;
            oldMenuItem = 0;
            break;
        default:
            break;
        }
        if(ItemSel >= MaxItemList - 1)
        {
            TopItem = ItemSel - (MaxItemList - 1);
        }
        else
        {
            TopItem = 0;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }
        delay(1);
    }
}

void WAKE_LED::alarmScreen()
{
    bool ExitSetAlarm = false;
    bool SetHour = true;
    uint8_t AlarmHour = 0, AlarmiMinute = 0, Button = ROTARY::NO_ACTION;
    wakeLedAlarm->getAlarmTime(AlarmHour, AlarmiMinute);
    while(!ExitSetAlarm)
    {
        display->clearBuff();
        drawTopInfo();
        if(SetHour)
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(AlarmHour).c_str()));
            display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Ora di allarme");
        }
        else
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(AlarmiMinute).c_str()));
            display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Minuto di allarme");
        }
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(SetHour)
            {
                if(AlarmHour > 0)
                    AlarmHour--;
                else
                    AlarmHour = 23;
            }
            else
            {
                if(AlarmiMinute > 0)
                    AlarmiMinute--;
                else
                    AlarmiMinute = 59;
            }
            break;
        case ROTARY::INCREMENT:
            if(SetHour)
            {
                if(AlarmHour < 23)
                    AlarmHour++;
                else
                    AlarmHour = 0;
            }
            else
            {
                if(AlarmiMinute < 59)
                    AlarmiMinute++;
                else
                    AlarmiMinute = 0;
            }
            break;
        case ROTARY::BUTTON_PRESS:
            if(SetHour)
            {
                SetHour = false;
            }
            else
            {
                wakeLedAlarm->setAlarmTime(AlarmHour, AlarmiMinute);
                wakeLedAlarm->setAlarm();
                display->drawPopUp("Allarme impostato", 1500);
                ExitSetAlarm = true;
            }
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            if(SetHour)
            {
                ExitSetAlarm = true;
            }
            else
            {
                SetHour = true;
            }
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }
        delay(1);
    }
}

void WAKE_LED::preLedAccension()
{
    bool ExitPreAccensionSet = false;
    uint8_t Button = ROTARY::NO_ACTION;
    uint16_t Accension = preAccensionTime;
    while(!ExitPreAccensionSet)
    {
        display->clearBuff();
        drawTopInfo();
        if(Accension == 0)
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_6_H_13_B, "Disabilitato");
        }
        else
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(Accension).c_str()));
        }
        display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Imposta accensione (min)");
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(Accension > 0)
                Accension--;
            else
                Accension = 30;
            break;
        case ROTARY::INCREMENT:
            if(Accension < 30)
                Accension++;
            else
                Accension = 0;
            break;
        case ROTARY::BUTTON_PRESS:
            display->drawPopUp("Accensione impostata", 1500);
            preAccensionTime = Accension;
            ExitPreAccensionSet = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitPreAccensionSet = true;
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }
        delay(1);
    }
}

void WAKE_LED::snoozeTime()
{
    bool ExitSnoozeSet = false;
    uint8_t Button = ROTARY::NO_ACTION;
    uint16_t SnoozeTime = wakeLedAlarm->getSnoozeTime();
    while(!ExitSnoozeSet)
    {
        display->clearBuff();
        drawTopInfo();
        display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(SnoozeTime).c_str()));
        display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Imposta snooze(min)");
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(SnoozeTime > 1)
                SnoozeTime--;
            else
                SnoozeTime = 30;
            break;
        case ROTARY::INCREMENT:
            if(SnoozeTime < 30)
                SnoozeTime++;
            else
                SnoozeTime = 1;
            break;
        case ROTARY::BUTTON_PRESS:
            wakeLedAlarm->setSnoozeTime(SnoozeTime);
            display->drawPopUp("Snooze impostato", 1500);
            ExitSnoozeSet = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitSnoozeSet = true;
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }        
        delay(1);
    }
}

void WAKE_LED::reactiveAlarmTime()
{
    bool ExitReactiveAlarmSet = false;
    uint8_t Button = ROTARY::NO_ACTION;
    uint16_t ReactiveAlarmTime = wakeLedAlarm->getReactiveAlarmTime();
    while(!ExitReactiveAlarmSet)
    {
        display->clearBuff();
        drawTopInfo();
        display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(ReactiveAlarmTime).c_str()));
        display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Riattiva allarme (min)");
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(ReactiveAlarmTime > 1)
                ReactiveAlarmTime--;
            else
                ReactiveAlarmTime = 30;
            break;
        case ROTARY::INCREMENT:
            if(ReactiveAlarmTime < 30)
                ReactiveAlarmTime++;
            else
                ReactiveAlarmTime = 1;
            break;
        case ROTARY::BUTTON_PRESS:
            wakeLedAlarm->setReactiveAlarmTime(ReactiveAlarmTime);
            display->drawPopUp("Riattivo impostato", 1500);
            ExitReactiveAlarmSet = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitReactiveAlarmSet = true;
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }        
        delay(1);
    }   
}

void WAKE_LED::meteoInfo()
{
    bool ExitMeteoInfo = false;
    uint8_t Button = ROTARY::NO_ACTION;
    while(!ExitMeteoInfo)
    {
        display->clearBuff();
        drawTopInfo();
        display->drawString(NHDST7565::LEFT_POS, 10, NHDST7565::W_5_H_8, "Temperatura");
        display->drawString(NHDST7565::LEFT_POS, 22, NHDST7565::W_6_H_13_B, DispString(String(wifiStation->weatherInfo.temperature, 1).c_str()) + " C");
        display->drawString(NHDST7565::LEFT_POS, 38, NHDST7565::W_3_H_6, "Percepita");
        display->drawString(NHDST7565::LEFT_POS, 50, NHDST7565::W_6_H_13_B, DispString(String(wifiStation->weatherInfo.perceivedTemp, 1).c_str()) + " C");     
        display->drawString(NHDST7565::RIGHT_POS, 10, NHDST7565::W_5_H_8, "Umidita");
        display->drawString(NHDST7565::RIGHT_POS, 22, NHDST7565::W_6_H_13_B, DispString(String(wifiStation->weatherInfo.humidity).c_str()) + " %");
        display->drawString(NHDST7565::RIGHT_POS, 38, NHDST7565::W_3_H_6, "Pressione(mbar)");
        display->drawString(NHDST7565::RIGHT_POS, 50, NHDST7565::W_6_H_13_B, DispString(String(wifiStation->weatherInfo.pressure, 0).c_str()));    
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:

            break;
        case ROTARY::INCREMENT:

            break;
        case ROTARY::BUTTON_PRESS:
            wakeLedAlarm->resetAlarm();
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitMeteoInfo = true;
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }     
        if(wakeLedAlarm->isAlarmActive())
        {
            ExitMeteoInfo = true;
            wakeScreen = ALARM_ACTIVE_SCREEN;
        }           
        delay(1);
    }  
}

void WAKE_LED::backlightTime()
{
    bool ExitBacklightTime = false;
    uint8_t Button = ROTARY::NO_ACTION;
    uint16_t BacklightTime = display->getDisplayLedTurnoffTime();
    while(!ExitBacklightTime)
    {
        display->clearBuff();
        drawTopInfo();
        display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(BacklightTime).c_str()));
        display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Tempo backlight (s)");
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(BacklightTime > 5)
                BacklightTime -= 5;
            else
                BacklightTime = 120;
            break;
        case ROTARY::INCREMENT:
            if(BacklightTime < 120)
                BacklightTime += 5;
            else
                BacklightTime = 5;
            break;
        case ROTARY::BUTTON_PRESS:
            display->setDisplayLedTurnoffTime(BacklightTime);
            display->restartDisplayLedTimer();
            display->drawPopUp("Backlight impostata", 1500);
            ExitBacklightTime = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitBacklightTime = true;
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }        
        delay(1);
    }    
}

void WAKE_LED::setOfflineTime()
{
    bool ExitSetTime = false;
    uint8_t Button = ROTARY::NO_ACTION;
    uint8_t Hour = 0, Minute = 0;
    bool IsHour = true;
    while(!ExitSetTime)
    {
        display->clearBuff();
        drawTopInfo();
        if(IsHour)
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(Hour).c_str()));
            display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Imposta ora");
        }
        else
        {        
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(Minute).c_str()));
            display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Imposta minuti");
        }
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(IsHour)
            {
                if(Hour > 0)
                    Hour--;
                else
                    Hour = 23;
            }
            else
            {
                if(Minute > 0)
                    Minute--;
                else
                    Minute = 59;                
            }
            break;
        case ROTARY::INCREMENT:
            if(IsHour)
            {
                if(Hour < 23)
                    Hour++;
                else
                    Hour = 0;
            }
            else
            {
                if(Minute < 59)
                    Minute++;
                else
                    Minute = 0;
            }
            break;
        case ROTARY::BUTTON_PRESS:
            if(IsHour)
            {
                IsHour = false;
            }
            else
            {
                display->drawPopUp("Ora impostata", 1500);
                wifiStation->setOfflineTime(Hour, Minute);
                ExitSetTime = true;
            }
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            if(IsHour)
                ExitSetTime = true;
            else
                IsHour = true;
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }        
        delay(1);
    } 
}

void WAKE_LED::setOfflineDate()
{
    bool ExitSetDate = false;
    uint8_t Button = ROTARY::NO_ACTION;
    uint8_t Day = 1, Month = 1, Year = 21;
    const uint8_t DAY = 0, MONTH = 1, YEAR = 2;
    const int DayInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    uint8_t WichChange = YEAR;
    while(!ExitSetDate)
    {
        display->clearBuff();
        drawTopInfo();
        if(WichChange == DAY)
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(Day).c_str()));
            display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Imposta giorno");
        }
        else if(WichChange == MONTH)
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(Month).c_str()));
            display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Imposta mese");              
        }
        else
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_17_H_29, DispString(String(Year + 2000).c_str()));
            display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "Imposta anno");
        }        
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(WichChange == DAY)
            {
                if(Day > 1)
                    Day--;
                else
                    Day = DayInMonth[Month - 1];
            }
            else if(WichChange == MONTH)
            {
                if(Month > 1)
                    Month--;
                else
                    Month = 12;                
            }
            else
            {
                if(Year > 21)
                    Year--;
                else
                    Year = 99; 
            }
            break;
        case ROTARY::INCREMENT:
            if(WichChange == DAY)
            {
                if(Day < DayInMonth[Month - 1])
                    Day++;
                else
                    Day = 1;
            }
            else if(WichChange == MONTH)
            {
                if(Month < 12)
                    Month++;
                else
                    Month = 1;                
            }
            else
            {
                if(Year < 99)
                    Year++;
                else
                    Year = 21; 
            }
            break;
        case ROTARY::BUTTON_PRESS:
            if(WichChange == DAY)
            {
                wifiStation->setOfflineDate(Day, Month, Year);
                display->drawPopUp("Data impostata", 1500);
                ExitSetDate = true;
            }
            else if(WichChange == MONTH)
            {
                WichChange = DAY;
            }
            else
            {
                WichChange = MONTH;
            }
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            if(WichChange == DAY)
            {
                WichChange = MONTH;
            }
            else if(WichChange == MONTH)
            {
                WichChange = YEAR;
            }
            else
            {
                ExitSetDate = true;
            }
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }        
        delay(1);
    }     
}

void WAKE_LED::setDisplayBrightness()
{
    bool ExitSetDisplayBrightness = false;
    uint8_t Button = ROTARY::NO_ACTION;
    uint16_t Brightness = display->getDisplayLedBrightness();
    String BrighStr = "";
    while(!ExitSetDisplayBrightness)
    {
        display->clearBuff();
        drawTopInfo();
        if(Brightness == 0)
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_6_H_13_B, "Disabilitato");
        }
        else if(Brightness == 105)
        {
            display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_6_H_13_B, "Auto");
        }
        else
        {
            BrighStr = String(Brightness) + "%";
            display->drawString(NHDST7565::CENTER_POS, 12, NHDST7565::W_17_H_29, DispString(BrighStr.c_str()));
        }
        display->drawString(NHDST7565::CENTER_POS, 40, NHDST7565::W_5_H_8, "Imposta luminosita");
        display->drawString(NHDST7565::CENTER_POS, 50, NHDST7565::W_5_H_8, "display");
        display->sendBuff();
        backGroundTasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
            if(Brightness > 0)
                Brightness -= 5;
            else
                Brightness = 105;
            break;
        case ROTARY::INCREMENT:
            if(Brightness < 105)
                Brightness += 5;
            else
                Brightness = 0;
            break;
        case ROTARY::BUTTON_PRESS:
            display->drawPopUp("Luminosita impostata", 1500);
            if(Brightness >= 0 && Brightness <= 100)
            {
                display->setDisplayLedBrightness(Brightness);
                displayBrightnessAuto = false;
            }
            else
            {
                displayBrightnessAuto = true;
            }
            ExitSetDisplayBrightness = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitSetDisplayBrightness = true;
            break;
        default:
            break;
        }
        if(Button != ROTARY::NO_ACTION || irSensor->digitalVal(700, false) == ON)
        {
            display->restartDisplayLedTimer();
        }
        delay(1);
    }
}

WAKE_LED::WAKE_LED()
{
    display = new NHDST7565();
    rotary = new ROTARY();
    wifiStation = new WIFI_STATION();
    wakeLedAlarm = new ALARM();
    alarmLed = new LEDS(D1, LEDS::PWM);
    irSensor = new SENSOR();
    preAccensionTimer = new Chrono(Chrono::SECONDS, false);
    autoBrightnessTimer = new Chrono(Chrono::SECONDS, false);
}


void WAKE_LED::init()
{
    display->manualManageDisplayLed = true;
	display->manualSwitchLedDisplay(ON);
	display->manualManageDisplayLed = false;
    display->drawPopUp("Home Microtech", 1500);
    display->clearBuff();
    display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_6_H_13_B, "Collegamento");
    display->drawString(NHDST7565::CENTER_POS, 42, NHDST7565::W_6_H_13_B, "al wifi...");
    display->sendBuff();
    wifiStation->initWifiStation();
    display->clearDisplay();
    if(wifiStation->isWifiConnected())
    {
        display->clearBuff();
        display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_6_H_13_B, "Wifi");
        display->drawString(NHDST7565::CENTER_POS, 42, NHDST7565::W_6_H_13_B, "collegato");
        display->sendBuff();
    }
    else
    {
        display->clearBuff();
        display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_6_H_13_B, "Wifi");
        display->drawString(NHDST7565::CENTER_POS, 42, NHDST7565::W_6_H_13_B, "non collegato");
        display->sendBuff();
    }
    delay(1000);
    display->clearDisplay();
    display->restartDisplayLedTimer();
}


void WAKE_LED::run()
{
    switch (wakeScreen)
    {
    case MAIN_SCREEN:
        mainScreen();
        break;
    case MENU_SCREEN:
        menu();
        break;
    case ALARM_SCREEN:
        alarmScreen();
        wakeScreen = MENU_SCREEN;
        break;
    case PRE_ACCENSION_SCREEN:
        preLedAccension();
        wakeScreen = MENU_SCREEN;
        break;        
    case SNOOZE_TIME_SCREEN:
        snoozeTime();
        wakeScreen = MENU_SCREEN;
        break;
    case REACTIVE_ALARM_SCREEN:
        reactiveAlarmTime();
        wakeScreen = MENU_SCREEN;
        break;        
    case METEO_INFO_SCREEN:
        meteoInfo();
        wakeScreen = MENU_SCREEN;
        break;
    case BACKLIGHT_TIME_SCREEN:
        backlightTime();
        wakeScreen = MENU_SCREEN;
        break;
    case ALARM_ACTIVE_SCREEN:
        alarmActiveScreen();
        break;  
    case SET_TIME_OFFLINE:
        setOfflineTime();
        wakeScreen = MENU_SCREEN;
        break;
    case SET_DATE_OFFLINE:
        setOfflineDate();
        wakeScreen = MENU_SCREEN;
        break;   
    case SET_DISPLAY_BRIGHTNESS:
        setDisplayBrightness();
        wakeScreen = MENU_SCREEN;
        break;      
    default:
        break;
    }
    // delay(5);
}