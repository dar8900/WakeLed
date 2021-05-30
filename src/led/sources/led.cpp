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

void LEDS::writePwm(int DutyCycle)
{
    if(pinModality == PWM && (DutyCycle >= 0 && DutyCycle <= PWM_RANGE))
    {
        analogWrite(pinNumber, DutyCycle);
        pwmValueSetted = DutyCycle;
    }
}

void LEDS::writeDigital(bool Status)
{
    if(pinModality == DIGITAL_OUT)
    {
        digitalWrite(pinNumber, Status == ON ? HIGH : LOW);
        digitalOutStatus = Status;
    }
    else if(pinModality == PWM)
    {
        if(Status == ON)
        {
            pwmValueSetted = PWM_RANGE;
        }
        else
        {
            pwmValueSetted = 0;
        }
        analogWrite(pinNumber, pwmValueSetted);
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


uint16_t LEDS::getPwmValue()
{
    return pwmValueSetted;
}