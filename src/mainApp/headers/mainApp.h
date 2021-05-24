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
            SET_DISPLAY_BRIGHTNESS,
            SYSTEM_INFO,
            SET_TIME_OFFLINE,
            SET_DATE_OFFLINE,
            MAX_SCREEN
        };

        enum
        {
            VERSION_SW = 0,
            ALARM_SET,
            PRE_ACC_TIME,
            SNOOZE_TIME,
            RESTART_TIME,
            BACKLIGHT_TIME,
            BRIGHTNESS,
            WIFI_STATUS,
            MAX_INFO
        };

        NHDST7565 *display;
        ROTARY *rotary;
        WIFI_STATION *wifiStation;
        ALARM *wakeLedAlarm;
        LEDS *alarmLed;
        SENSOR *irSensor;
        Chrono *preAccensionTimer;
        Chrono *autoBrightnessTimer;
        uint8_t autoBrightnessValue;
        uint8_t wakeScreen = MAIN_SCREEN;
        uint16_t preAccensionTime = 5; // In minuti
        uint16_t accensionLedPwmIncrement = 0;
        uint16_t ledDutyCycle = 0;
        uint8_t oldMenuItem = 0;
        bool alarmLedManual = false;
        bool displayBrightnessAuto = false;
        
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
        void setDisplayBrightness();
        void adjustAutoBrightness();
        void showSystemInfo();

    public:
        WAKE_LED();
        void init();
        void run();
};

#endif