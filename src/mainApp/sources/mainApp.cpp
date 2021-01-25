#include "../headers/mainApp.h"

void WAKE_LED::tasks()
{
    wifiStation->run();
}

void WAKE_LED::drawTopInfo()
{
    if(wifiStation->wifiConnected)
    {
        display->drawUnicodeChar(NHDST7565::LEFT_POS, NHDST7565::TOP_POS, NHDST7565::W_8_H_8_ICON, 248);
    }
}

void WAKE_LED::drawWeatherInfo()
{
    uint16_t IconCode = 0;
    DispString Temp = "", Press = "";
    if(wifiStation->wifiConnected)
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
        display->drawUnicodeChar(110, 1, NHDST7565::W_16_H_16_WEATHER_ICON, IconCode);
        Temp = DispString(String(wifiStation->weatherInfo.temperature, 1).c_str());
        Press = DispString(String(wifiStation->weatherInfo.pressure, 0).c_str());
        display->drawString(NHDST7565::RIGHT_POS, 20, NHDST7565::W_5_H_8, "Temp.");
        display->drawString(NHDST7565::RIGHT_POS, 29, NHDST7565::W_5_H_8, Temp + "C");
        // display->drawString(NHDST7565::RIGHT_POS, 30, NHDST7565::W_5_H_8, "C");
        display->drawString(NHDST7565::RIGHT_POS, 44, NHDST7565::W_5_H_8, Press);
        display->drawString(NHDST7565::RIGHT_POS, 54, NHDST7565::W_5_H_8, "mbar");
    }
    else
    {
        display->drawString(NHDST7565::RIGHT_POS, 20, NHDST7565::W_5_H_8, "no");
        display->drawString(NHDST7565::RIGHT_POS, 20, NHDST7565::W_5_H_8, "info");
        display->drawString(NHDST7565::RIGHT_POS, 20, NHDST7565::W_5_H_8, "meteo");
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
        display->drawString(NHDST7565::LEFT_POS, 12, NHDST7565::W_17_H_29, wifiStation->timeDateInfo.timeFormatted);
        display->drawString(NHDST7565::LEFT_POS, 42, NHDST7565::W_6_H_10, wifiStation->timeDateInfo.dateFormatted);
        drawWeatherInfo();
        display->sendBuff();
        tasks();
        Button = rotary->getRotaryState();
        switch (Button)
        {
        case ROTARY::DECREMENT:
        case ROTARY::INCREMENT:
            break;
        case ROTARY::BUTTON_PRESS:
            break;
        case ROTARY::LONG_BUTTON_PRESS:
            break;
        default:
            break;
        }
        delay(1);
    }
}





WAKE_LED::WAKE_LED()
{
    display = new NHDST7565();
    rotary = new ROTARY();
    wifiStation = new WIFI_STATION();
}


void WAKE_LED::init()
{
    wifiStation->initWifiStation();
}


void WAKE_LED::run()
{
    switch (wakeScreen)
    {
    case MAIN_SCREEN:
        mainScreen();
        break;
    default:
        break;
    }
    // delay(5);
}