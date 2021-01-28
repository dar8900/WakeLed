#include "../headers/mainApp.h"

#define METEO_X_START   92

DispString menuVoices[] = 
{
    "Imposta allarme",
    "Imposta tempo led",
    "Imposta snooze all.",
    "Imposta riavvio all.",
    "Info meteo",
};

void WAKE_LED::manageAlarmLed()
{
    uint8_t AlarmHour = 0, AlarmMinute = 0, PreAccensionMinute = preAccensionTime / 60;
    uint8_t ActualHour = 0, ActualMinute = 0;
    wakeLedAlarm->getAlarmTime(AlarmHour, AlarmMinute);
    if(wakeLedAlarm->isAlarmSet() && preAccensionTime != 0)
    {
        std::tm *locTime = std::localtime((time_t *)&wifiStation->timeDateInfo.timestamp);
        ActualHour = locTime->tm_hour;
        ActualMinute = locTime->tm_min;
        uint16_t TimeDiff = 0;
        if(AlarmMinute - PreAccensionMinute >= 0)
        {
            if(ActualMinute >= (AlarmMinute - PreAccensionMinute))
            {
                if(preAccensionTimer->hasPassed(1, true))
                {
                    alarmLed->writePwm(accensionLedPwmIncrement);
                    accensionLedPwmIncrement += (LEDS::PWM_RANGE / preAccensionTime);
                }
            }
        }
        else
        {
            uint16_t TimeToMidNite = PreAccensionMinute - AlarmMinute;
            if(ActualMinute >= (60 - TimeToMidNite) || ActualMinute <= AlarmMinute)
            {
                if(preAccensionTimer->hasPassed(1, true))
                {
                    alarmLed->writePwm(accensionLedPwmIncrement);
                    accensionLedPwmIncrement += (LEDS::PWM_RANGE / preAccensionTime);
                }
            }

        }
        
    }
    else
    {
        preAccensionTimer->restart();
        ledDutyCycle = 0;
        alarmLed->writePwm(ledDutyCycle);
        accensionLedPwmIncrement = 0;
    }
}

void WAKE_LED::backGroundTasks()
{
    wifiStation->run();
    wakeLedAlarm->runAlarm(wifiStation->timeDateInfo.timestamp);
    manageAlarmLed();
}

void WAKE_LED::drawTopInfo()
{
    if(wifiStation->isWifiConnected())
    {
        display->drawUnicodeChar(NHDST7565::LEFT_POS, NHDST7565::TOP_POS, NHDST7565::W_8_H_8_ICON, 248);
    }
    if(wakeLedAlarm->isAlarmSet())
    {
        display->drawUnicodeChar(12, NHDST7565::TOP_POS, NHDST7565::W_8_H_8_ICON, 93);
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

void WAKE_LED::mainScreen()
{   
    bool ExitMainScreen = false;
    int8_t Button = ROTARY::NO_ACTION;
    while(!ExitMainScreen)
    {
        display->clearBuff();
        drawTopInfo();
        display->drawString(NHDST7565::LEFT_POS, 14, NHDST7565::W_17_H_29, wifiStation->timeDateInfo.timeFormatted);
        display->drawString(NHDST7565::LEFT_POS, 46, NHDST7565::W_6_H_10, wifiStation->timeDateInfo.dateFormatted);
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
            break;
        case ROTARY::BUTTON_PRESS:
            wakeScreen = MENU_SCREEN;
            ExitMainScreen = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            wakeLedAlarm->resetAlarm();
            break;
        default:
            break;
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
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            break;
        default:
            break;
        }
        delay(1);
    }   
}

void WAKE_LED::menu()
{
    bool ExitMenu = false;
    int8_t Button = ROTARY::NO_ACTION;
    uint8_t ItemSel = 0, TopItem = 0;
    const uint8 MaxItemList = 3;
    while(!ExitMenu)
    {
        display->clearBuff();
        drawTopInfo();
        for(int MenuItem = 0; MenuItem < MaxItemList; MenuItem++)
        {
            uint8_t NextItem = TopItem + MenuItem;
            if(NextItem >= MAX_SCREEN - 3)
                break;
            display->drawString(8, 14 + (12 * MenuItem), NHDST7565::W_6_H_10, menuVoices[NextItem]); 
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
                ItemSel = (MAX_SCREEN - 3) - 1;
            break;
        case ROTARY::INCREMENT:
            if(ItemSel < (MAX_SCREEN - 3) - 1)
                ItemSel++;
            else
                ItemSel = 0;        
            break;
        case ROTARY::BUTTON_PRESS:
            wakeScreen = ItemSel + ALARM_SCREEN;
            ExitMenu = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitMenu = true;
            wakeScreen = MAIN_SCREEN;
            break;
        default:
            break;
        }
        if(ItemSel > MaxItemList - 1)
        {
            TopItem = ItemSel - MaxItemList - 1;
        }
        else
        {
            TopItem = 0;
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
                    AlarmHour = 23;
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
        delay(1);
    }
}

void WAKE_LED::preLedAccension()
{
    bool ExitPreAccensionSet = false;
    uint8_t Button = ROTARY::NO_ACTION;
    uint16_t Accension = preAccensionTime / 60;
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
            preAccensionTime = Accension * 60;
            ExitPreAccensionSet = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitPreAccensionSet = true;
            break;
        default:
            break;
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
        delay(1);
    }   
}

void WAKE_LED::meteoInfo()
{
    
}

WAKE_LED::WAKE_LED()
{
    display = new NHDST7565();
    rotary = new ROTARY();
    wifiStation = new WIFI_STATION();
    wakeLedAlarm = new ALARM();
    alarmLed = new LEDS(D1, LEDS::PWM);
    preAccensionTimer = new Chrono(Chrono::SECONDS, false);
}


void WAKE_LED::init()
{
    display->drawPopUp("Home Microtech", 1500);
    display->clearBuff();
    display->drawString(NHDST7565::CENTER_POS, NHDST7565::TOP_POS, NHDST7565::W_6_H_13_B, "Collegamento");
    display->drawString(NHDST7565::CENTER_POS, 15, NHDST7565::W_6_H_13_B, "al wifi...");
    display->sendBuff();
    wifiStation->initWifiStation();
    display->clearDisplay();
    if(wifiStation->isWifiConnected())
    {
        display->clearBuff();
        display->drawString(NHDST7565::CENTER_POS, NHDST7565::TOP_POS, NHDST7565::W_6_H_13_B, "Wifi");
        display->drawString(NHDST7565::CENTER_POS, 15, NHDST7565::W_6_H_13_B, "collegato");
        display->sendBuff();
    }
    else
    {
        display->clearBuff();
        display->drawString(NHDST7565::CENTER_POS, NHDST7565::TOP_POS, NHDST7565::W_6_H_13_B, "Wifi");
        display->drawString(NHDST7565::CENTER_POS, 15, NHDST7565::W_6_H_13_B, "non collegato");
        display->sendBuff();
    }
    delay(1000);
    display->clearDisplay();
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
        // meteoInfo();
        wakeScreen = MENU_SCREEN;
        break;
    case ALARM_ACTIVE_SCREEN:
        alarmActiveScreen();
        wakeScreen = MAIN_SCREEN;
        break;        
    default:
        break;
    }
    // delay(5);
}