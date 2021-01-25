#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "../../debug/headers/debug.h"
#include "../../display/headers/display.h"
#include "../../rotary/headers/rotary.h"
#include "../../wifi/headers/wifi.h"
#include "../../eeprom/headers/eeprom_esp.h"

class WAKE_LED
{
    private:
        enum
        {
            MAIN_SCREEN = 0,
            TEST_SCREEN,
            MAX_SCREEN
        };

        NHDST7565 *display;
        ROTARY *rotary;
        WIFI_STATION *wifiStation;
        uint8_t wakeScreen = MAIN_SCREEN;
        void tasks();
        void drawTopInfo();
        void drawWeatherInfo();

        void mainScreen();

    public:
        WAKE_LED();
        void init();
        void run();
};

#endif