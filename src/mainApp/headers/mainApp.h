#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "../../debug/headers/debug.h"
#include "../../display/headers/display.h"
#include "../../rotary/headers/rotary.h"
#include "../../wifi/headers/wifi.h"
#include "../../alarm/headers/alarms.h"
// #include "../../eeprom/headers/eeprom_esp.h"

class WAKE_LED
{
    private:
        enum
        {
            MAIN_SCREEN = 0,
            MENU_SCREEN,
            ALARM_SCREEN,
            SNOOZE_TIME_SCREEN,
            REACTIVE_ALARM_SCREEN,
            METEO_INFO_SCREEN,
            MAX_SCREEN
        };

        NHDST7565 *display;
        ROTARY *rotary;
        WIFI_STATION *wifiStation;
        ALARM *wakeLedAlarm;
        uint8_t wakeScreen = MAIN_SCREEN;

        

        void backGroundTasks();
        void drawTopInfo();
        void drawWeatherInfo();

        void mainScreen();
        void menu();
        void alarmScreen();
        void snoozeTime();
        void reactiveAlarmTime();
        void meteoInfo();

    public:
        WAKE_LED();
        void init();
        void run();
};

#endif