#include "src/mainApp/headers/mainApp.h"

WAKE_LED WakeLed;

void setup()
{
    WakeLed.init();
}


void loop()
{
    WakeLed.run();
}

