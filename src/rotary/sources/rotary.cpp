#include "../headers/rotary.h"


ROTARY::ROTARY()
{
    pinMode(D3, INPUT_PULLUP);
    encoder = new RotaryEncoder(D2, D4);
}

void ROTARY::acqRotaryState()
{
    actualState = NO_ACTION;
    encoder->tick();
    actualState = (int8_t)encoder->getDirection();
    if(actualState == (int8_t)RotaryEncoder::Direction::NOROTATION)
    {
        int8_t SwitchState = digitalRead(D3);
        if(SwitchState == LOW)
        {
            uint32_t LongPressTime = millis();
            while(SwitchState == LOW)
            {
                SwitchState = digitalRead(D3);
                if(millis() - LongPressTime > 1000)
                {
                    SwitchState = LONG_BUTTON_PRESS;
                    break;
                }
            }
            if(SwitchState == LONG_BUTTON_PRESS)
            {
                actualState = LONG_BUTTON_PRESS;
                wasLongPress = true;
            }
            else
            {
                if(!wasLongPress)
                {
                    actualState = BUTTON_PRESS;
                }
                else
                {
                    wasLongPress = false;
                }
                
            }
        }
        else
        {
            actualState = NO_ACTION;
        }
    }
    else
    {
        if(actualState == (int8_t)RotaryEncoder::Direction::CLOCKWISE)
        {
            actualState = DECREMENT;
        }
        else
        {
            actualState = INCREMENT;
        }
        
    }
}

int8_t ROTARY::getRotaryState()
{
    acqRotaryState();
    return actualState;
}