#include "../headers/alarms.h"

void ALARM::getActualTime(uint32_t GlobalTimestamp)
{
    std::tm *locTime = std::localtime((time_t *)&GlobalTimestamp);
    actualTime.hour = locTime->tm_hour;
    actualTime.minute = locTime->tm_min;
}

void ALARM::checkAlarm(uint32_t GlobalTimestamp)
{
    if(alarmSetted)
    {
        getActualTime(GlobalTimestamp);
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
    checkAlarmTimer = new Chrono(Chrono::MILLIS, false);
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
    checkAlarmTimer->restart();
    alarmSetted = true;
}

void ALARM::resetAlarm()
{
    checkAlarmTimer->stop();
    alarmSetted = false;
}

void ALARM::getAlarmTime(uint8_t &AlarmHour, uint8_t &AlarmMinute)
{
    AlarmHour = alarmTime.hour;
    AlarmMinute = alarmTime.minute;
}

void ALARM::setAlarmTime(uint8_t AlarmHour, uint8_t AlarmMinute)
{
    alarmTime.hour = AlarmHour;
    alarmTime.minute = AlarmMinute;
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


void ALARM::runAlarm(uint32_t GlobalTimestamp)
{
    if(checkAlarmTimer->hasPassed(1000, true))
    {
        checkAlarm(GlobalTimestamp);
    }
}