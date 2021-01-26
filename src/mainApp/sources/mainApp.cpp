#include "../headers/mainApp.h"

#define METEO_X_START   92

DispString menuVoices[] = {
    "Imposta allarme",
    "Imposta snooze all.",
    "Imposta riavvio all.",
    "Info meteo",
};

void WAKE_LED::backGroundTasks()
{
    wifiStation->run();
    wakeLedAlarm->runAlarm(wifiStation->timeDateInfo.timestamp);
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
            ExitMainScreen = true;
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
    const uint8 MaxItemList = 4;
    while(!ExitMenu)
    {
        display->clearBuff();
        drawTopInfo();
        for(int MenuItem = 0; MenuItem < MaxItemList; MenuItem++)
        {
            uint8_t NextItem = TopItem + MenuItem;
            if(NextItem >= MAX_SCREEN - 2)
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
                ItemSel = (MAX_SCREEN - 2) - 1;
            break;
        case ROTARY::INCREMENT:
            if(ItemSel < (MAX_SCREEN - 2) - 1)
                ItemSel++;
            else
                ItemSel = 0;        
            break;
        case ROTARY::BUTTON_PRESS:
            ExitMenu = true;
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            ExitMenu = true;
            wakeScreen = MAIN_SCREEN;
            break;
        default:
            break;
        }
        if(ItemSel > MaxItemList)
        {
            TopItem = ItemSel - MaxItemList;
        }
        else
        {
            TopItem = 0;
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
}


void WAKE_LED::init()
{
    display->clearBuff();
    display->drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_6_H_13_B, "Home Microtech");
    display->sendBuff();
    delay(1500);
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
        wakeScreen = MENU_SCREEN;
        break;
    case MENU_SCREEN:
        menu();
        break;
    case ALARM_SCREEN:
        // alarmScreen();
        wakeScreen = MENU_SCREEN;
        break;
    case SNOOZE_TIME_SCREEN:
        // snoozeTime();
        wakeScreen = MENU_SCREEN;
        break;
    case REACTIVE_ALARM_SCREEN:
        // reactiveAlarmTime();
        wakeScreen = MENU_SCREEN;
        break;
    case METEO_INFO_SCREEN:
        // meteoInfo();
        wakeScreen = MENU_SCREEN;
        break;
    default:
        break;
    }
    // delay(5);
}