#ifndef ALARMS_H
#define ALARMS_H
#include <Arduino.h>
#include "../../eeprom/headers/eeprom_esp.h"
#include "../../wifi/headers/wifi.h"
#include <Chrono.h>
#include <ctime>

typedef struct 
{
    uint8_t hour;
    uint8_t minute;
}ALARM_TIME;


class ALARM
{


    private:

        ALARM_TIME alarmTime;
        ALARM_TIME actualTime;
        bool alarmActive = false;
        bool alarmSetted = false;
        Chrono *checkAlarmTimer;
        void getActualTime(uint32_t GlobalTimestamp);
        void checkAlarm(uint32_t GlobalTimestamp);


    public:

        ALARM();
        bool isAlarmActive();
        bool isAlarmSet();
        void setAlarm();
        void resetAlarm();
        void getAlarmTime(uint8_t &AlarmHour, uint8_t &AlarmMinute);
        void setAlarmTime(uint8_t AlarmHour, uint8_t AlarmMinute);
        void runAlarm(uint32_t GlobalTimestamp);
};

#endif