#ifndef ALARMS_H
#define ALARMS_H
#include <Arduino.h>
#include "../../eeprom/headers/eeprom_esp.h"
#include "../../wifi/headers/wifi.h"
#include <Chrono.h>
#include <ctime>
#include "../../debug/headers/debug.h"

typedef struct 
{
    uint8_t hour;     /* ora della struttura */
    uint8_t minute;  /* minuto della struttura */
    uint32_t ts;    /* timestamp della struttura */
}ALARM_TIME;


class ALARM
{


    private:

        ALARM_TIME alarmTime;
        ALARM_TIME actualTime;
        bool alarmActive = false;
        bool alarmSetted = false;
        bool alarmSnoozed = false;
        uint16_t snoozeTime = 300; // in secondi
        uint16_t reactiveAlarmTime = 300;
        Chrono *checkAlarmTimer;
        Chrono *snoozeTimer;
        Chrono *reactiveAlarmTimer;
        void getActualTime(uint32_t GlobalTimestamp);
        void checkAlarm(uint32_t GlobalTimestamp);


    public:

        ALARM();
        bool isAlarmActive();
        bool isAlarmSet();
        bool isAlarmSnoozed();
        void setAlarm();
        void resetAlarm();
        void getAlarmTime(uint8_t &AlarmHour, uint8_t &AlarmMinute);
        void setAlarmTime(uint8_t AlarmHour, uint8_t AlarmMinute);
        void setSnoozeTime(uint16_t SnoozeTime = 5);
        uint16_t getSnoozeTime(); /* in minuti */
        void setReactiveAlarmTime(uint16_t ReactiveAlarmTime = 5);
        uint16_t getReactiveAlarmTime(); /* in minuti */
        void runAlarm(uint32_t GlobalTimestamp);
        uint32_t getAlarmTimestamp();
};

#endif