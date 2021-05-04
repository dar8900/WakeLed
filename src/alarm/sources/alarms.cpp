#include "../headers/alarms.h"
#include <ctime>

void ALARM::getActualTime(uint32_t GlobalTimestamp)
{
    std::tm *locTime = std::localtime((time_t *)&GlobalTimestamp);
    actualTime.hour = locTime->tm_hour;
    actualTime.minute = locTime->tm_min;
    actualTime.ts = GlobalTimestamp;
}

void ALARM::checkAlarm(uint32_t GlobalTimestamp)
{
    getActualTime(GlobalTimestamp);
    if(alarmSetted)
    {
        if(actualTime.hour == alarmTime.hour && actualTime.minute == alarmTime.minute && !alarmActive && !alarmSnoozed)
        {
            if(!alarmActive)
            {
                snoozeTimer->restart();
            }
            alarmActive = true;
        }
        // Inserire un tempo di snooze della sveglia, servono due timer uno per lo snooze e uno per riattivare la sveglia
        if(snoozeTimer->hasPassed(snoozeTime) && alarmActive)
        {
            alarmSnoozed = true;
            alarmActive = false;
            reactiveAlarmTimer->restart();
            snoozeTimer->stop();
        }
        if(reactiveAlarmTimer->hasPassed(reactiveAlarmTime) && alarmSnoozed)
        {
            alarmSnoozed = false;
            alarmActive = true;
            reactiveAlarmTimer->stop();
            snoozeTimer->restart();
        }
    }
    else
    {
        alarmActive = false;
        alarmSnoozed = false;
        snoozeTimer->stop();
        reactiveAlarmTimer->stop();
    }
    
}

ALARM::ALARM()
{
    checkAlarmTimer = new Chrono(Chrono::MILLIS, true);
    snoozeTimer = new Chrono(Chrono::SECONDS, false);
    reactiveAlarmTimer = new Chrono(Chrono::SECONDS, false);
}

bool ALARM::isAlarmActive()
{
    return alarmActive;
}

bool ALARM::isAlarmSet()
{
    return alarmSetted;
}

bool ALARM::isAlarmSnoozed()
{
    return alarmSnoozed;
}

void ALARM::setAlarm()
{
    alarmSetted = true;
}

void ALARM::resetAlarm()
{
    alarmSetted = false;
}

void ALARM::getAlarmTime(uint8_t &AlarmHour, uint8_t &AlarmMinute)
{
    AlarmHour = alarmTime.hour;
    AlarmMinute = alarmTime.minute;
}

void ALARM::setAlarmTime(uint8_t AlarmHour, uint8_t AlarmMinute)
{
    uint32_t ActualTime = actualTime.ts;
    const int DayInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    std::tm *locTime = std::localtime((time_t *)&ActualTime);
    alarmTime.hour = AlarmHour;
    alarmTime.minute = AlarmMinute;
    locTime->tm_hour = alarmTime.hour;
    locTime->tm_min = alarmTime.minute;
    if(alarmTime.hour < actualTime.hour)
    {
        if(locTime->tm_mday + 1 <= DayInMonth[locTime->tm_mon])
            locTime->tm_mday += 1;
        else
        {
            locTime->tm_mday = 1;
            if(locTime->tm_mon + 1 < 12)
                locTime->tm_mon += 1;
            else
                locTime->tm_mon = 0;
        }
    }
    alarmTime.ts = mktime(locTime);
}

void ALARM::setSnoozeTime(uint16_t SnoozeTime)
{
    snoozeTime = SnoozeTime * 60;
}

uint16_t ALARM::getSnoozeTime()
{
    return (snoozeTime / 60);
}

void ALARM::setReactiveAlarmTime(uint16_t ReactiveAlarmTime)
{
    reactiveAlarmTime = ReactiveAlarmTime * 60;
}

uint16_t ALARM::getReactiveAlarmTime()
{
    return (reactiveAlarmTime / 60);
}

uint32_t ALARM::getAlarmTimestamp()
{
    return alarmTime.ts;
}

void ALARM::runAlarm(uint32_t GlobalTimestamp)
{
    if(checkAlarmTimer->hasPassed(500, true))
    {
        checkAlarm(GlobalTimestamp);
    }
}