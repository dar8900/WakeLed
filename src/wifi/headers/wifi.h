#ifndef WIFI_H
#define WIFI_H
#include <Arduino.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include "../../debug/headers/debug.h"
#include "../../display/headers/display.h"
#include <Chrono.h>
#include <ctime>
#include <string>
// #include "../../debug/headers/debug.h"

typedef String WifiString;

class WIFI_STATION
{
    private:
        WiFiUDP *ntpUDP;
        NTPClient *timeClient; //(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
        const WifiString NTP_SERVER = "ntp1.inrim.it";;
        const WifiString BOLOGNA_ID = "3181927";
        const WifiString WEATHER_API_KEY = "ec79bf368720417f78abda66d4e47828";
        bool wifiConnected = false;
        WifiString apiWeatherCall;
        uint16_t localHourShift = 3600; // shift di 1h rispetto all'ora di greenwitch
        uint16_t timeRefreshFrq = 30000; // frequenza di rischiesta di orario (in ms)
        HTTPClient *httpWeatherReq;
        Chrono *weatherTimer;
        Chrono *takeTimeBackUp;
        bool backupTimerStarted = false;
        uint32_t epochTimestamp = 1609459200;
        bool legalHourIsSetted = false;
        bool initWeather = true;
        void connectToWifi();
        void weatherHttpJson();
        void getWeatherInfo(bool TakeInfoNow);
        uint32_t getTimestamp(bool WifiConn);
        DispString getTimeFormatted();
        DispString getDateFormatted();
        DispString getWeekday();
        void legalHourShift();
        

    public:
        typedef struct 
        {
            uint16_t weatherID; // da controllare a pag https://openweathermap.org/weather-conditions
            float temperature; // in Kelvin, togliere 273.15
            float perceivedTemp; // in Kelvin, togliere 273.15
            int32_t humidity; // percentuale
            float pressure; // in mbar, dividere per 1000
        }WEATHER_VARS;
        
        typedef struct 
        {
            uint32_t timestamp;
            DispString timeFormatted;
            DispString dateFormatted;
            uint8_t dayHour;
            DispString weekDay;
        }TIME_VARS;
        
        enum
        {
            TEMPESTA = 0,
            PIOVIGGINE,
            PIOGGIA,
            NEVE,
            NEBBIA,
            LIMPIDO,
            NUVOLOSO,
            UNKNOWN
        };
        
        WEATHER_VARS weatherInfo;
        TIME_VARS timeDateInfo;
        
        WIFI_STATION();
        bool isWifiConnected();
        void initWifiStation();
        void setOfflineTime(uint8_t Hour, uint8_t Minute);
        void setOfflineDate(uint8_t Day, uint8_t Month, uint8_t Year);
        void run();
};

#endif
