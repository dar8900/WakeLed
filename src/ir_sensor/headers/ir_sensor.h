#ifndef IR_SENSOR_H
#define IR_SENSOR_H
#include <Arduino.h>
#include <Chrono.h>
#include "../../led/headers/led.h"

class SENSOR
{
    private:
        int16_t analogVal = 0;
        float voltageVal;
        Chrono *checkSensorTimer;
        void readAnalog();
    public:
        SENSOR();
        int16_t getAnalogVal();
        float getVoltage();
        bool digitalVal(int16_t Threshold, bool OverThreshold);
        void readSensor();
};


#endif