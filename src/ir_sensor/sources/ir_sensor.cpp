#include "../headers/ir_sensor.h"
#include "../../debug/headers/debug.h"

void SENSOR::readAnalog()
{
    analogVal = (int16_t)analogRead(A0);
    voltageVal = ((float)analogVal * 3.3) / 1023;
}

SENSOR::SENSOR()
{
    checkSensorTimer = new Chrono(Chrono::MILLIS, true);
}


int16_t SENSOR::getAnalogVal()
{
    return analogVal;
}

float SENSOR::getVoltage()
{
    return voltageVal;
}


bool SENSOR::digitalVal(int16_t Threshold, bool OverThreshold)
{
    bool DigitalRet = OFF;
    if(OverThreshold)
    {
        if(analogVal > Threshold)
        {
            DigitalRet = ON;
        }
        else
        {
            DigitalRet = OFF;
        }
    }
    else
    {
        if(analogVal < Threshold)
        {
            DigitalRet = ON;
        }
        else
        {
            DigitalRet = OFF;
        }
    }
    
    return DigitalRet;
}


void SENSOR::readSensor()
{
    if(checkSensorTimer->hasPassed(100, true))
    {
        readAnalog();
    }
}