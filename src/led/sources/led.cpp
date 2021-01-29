#include "../headers/led.h"

LEDS::LEDS(uint8_t Pin, uint8_t Mode)
{
    pinModality = Mode;
    pinNumber = Pin;
    if(pinModality == DIGITAL_IN)
    {
        pinMode(Pin, INPUT);
    }
    else if(pinModality == DIGITAL_OUT)
    {
        pinMode(Pin, OUTPUT);
    }
    else if(pinModality == PWM)
    {
        analogWriteRange(PWM_RANGE);
    }
    

}

void LEDS::writePwm(uint16_t DutyCycle)
{
    if(pinModality == PWM)
    {
        analogWrite(pinNumber, DutyCycle);
    }
}

void LEDS::writeDigital(bool Status)
{
    if(pinModality == DIGITAL_OUT)
    {
        digitalWrite(pinNumber, Status == ON ? HIGH : LOW);
        digitalOutStatus = Status;
    }
}

bool LEDS::readDigital()
{
    bool DigitalStatus = OFF;
    if(pinModality == DIGITAL_IN)
    {    
        if(digitalRead(pinNumber) == LOW)
        {
            DigitalStatus = OFF;
        }
        else
        {
            DigitalStatus = ON;
        }
    }
    return DigitalStatus;
}

bool LEDS::getDigitalOutStatus()
{
    return digitalOutStatus;
}

uint16_t LEDS::readAnalog()
{
    uint16_t AnalogVal = 0;
    if(pinModality == ANALOG)
    {
        AnalogVal = analogRead(pinNumber);
    }
    return AnalogVal;
}

