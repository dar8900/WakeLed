#include "src/display/headers/display.h"
#include "src/rotary/headers/rotary.h"
#include "src/wifi/headers/wifi.h"
#include "src/debug/headers/debug.h"

NHDST7565 Display;
WIFI_STATION WakeLedWifi;


void setup()
{
    WakeledDebug.writeDebugString("", "Setup", true);
    WakeLedWifi.initWifiStation();
    Display.clearDisplay();
}

void testDraw()
{
    DispString Temp = DispString(String(WakeLedWifi.weatherInfo.temperature).c_str());
    Display.drawString(NHDST7565::RIGHT_POS, NHDST7565::TOP_POS, NHDST7565::W_5_H_8, Temp);
    Display.drawString(NHDST7565::CENTER_POS, NHDST7565::MIDDLE_POS, NHDST7565::W_6_H_13_B, "ciaone");
}

void loop()
{
    WakeLedWifi.run();
    Display.drawDisplay(testDraw);
    
    delay(500);
}

// #include "src/display/headers/mainScreen.h"
// void setup()
// {
//     MainScreen();
// }

// void loop()
// {

// }