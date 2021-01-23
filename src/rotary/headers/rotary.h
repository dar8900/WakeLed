#ifndef ROTARY_H
#define ROTARY_H
#include <Arduino.h>
#include <RotaryEncoder.h>
#include "../../debug/headers/debug.h"

class ROTARY
{
    private:
        
        RotaryEncoder *encoder;
        bool wasLongPress = false;
        int8_t actualState = 0;
        void acqRotaryState();

    public:
        enum
        {
            NO_ACTION = 0,
            DECREMENT,
            INCREMENT,
            BUTTON_PRESS,
            LONG_BUTTON_PRESS,
        };
        ROTARY();
        int8_t getRotaryState();
};

#endif