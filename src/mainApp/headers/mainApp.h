#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "../../debug/headers/debug.h"
#include "../../display/headers/display.h"
#include "../../rotary/headers/rotary.h"
#include "../../wifi/headers/wifi.h"
#include "../../alarm/headers/alarms.h"
#include "../../led/headers/led.h"
#include "../../ir_sensor/headers/ir_sensor.h"
#include "../../version.h"
// #include "../../eeprom/headers/eeprom_esp.h"

class WAKE_LED
{
    private:
        enum
        {
            MAIN_SCREEN = 0,
            MENU_SCREEN,
            ALARM_ACTIVE_SCREEN,
            ALARM_SCREEN,
            PRE_ACCENSION_SCREEN,
            SNOOZE_TIME_SCREEN,
            REACTIVE_ALARM_SCREEN,
            BACKLIGHT_TIME_SCREEN,
            METEO_INFO_SCREEN,
            SET_TIME_OFFLINE,
            SET_DATE_OFFLINE,
            MAX_SCREEN
        };

        NHDST7565 *display;
        ROTARY *rotary;
        WIFI_STATION *wifiStation;
        ALARM *wakeLedAlarm;
        LEDS *alarmLed;
        SENSOR *irSensor;
        Chrono *preAccensionTimer;
        uint8_t wakeScreen = MAIN_SCREEN;
        uint16_t preAccensionTime = 300; // In secondi
        uint16_t accensionLedPwmIncrement = 0;
        uint16_t ledDutyCycle = 0;
        uint8_t oldMenuItem = 0;
        
        void manageAlarmLed();
        void backGroundTasks();
        void drawTopInfo();
        void drawWeatherInfo();
        void drawAnalogClock();

        void mainScreen();
        void menu();
        void alarmActiveScreen();
        void alarmScreen();
        void preLedAccension();
        void snoozeTime();
        void reactiveAlarmTime();
        void backlightTime();
        void meteoInfo();
        void setOfflineTime();
        void setOfflineDate();

    public:
        WAKE_LED();
        void init();
        void run();
};

#endif