#ifndef WIFI_H
#define WIFI_H
#include <Arduino.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <string>
#include <sstream>
#include <Chrono.h>
#include <ctime>

typedef std::string WifiString;

class WIFI_STATION
{
    private:
        const char *SSID = "FASTWEB-85B1EA";
        const char *PASSWD = "ZGFyaW9zYXJhYmFzZTY0";
        WiFiUDP *ntpUDP;
        NTPClient *timeClient; //(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
        const WifiString NTP_SERVER = "europe.pool.ntp.org";
        const WifiString BOLOGNA_ID = "3181927";
        const WifiString WEATHER_API_KEY = "ec79bf368720417f78abda66d4e47828";
        WifiString apiWeatherCall;
        uint16_t localHourShift = 3600; // shift di 1h rispetto all'ora di greenwitch
        uint16_t timeRefreshFrq = 1000; // frequenza di rischiesta di orario (in ms)
        bool wifiConnected = false;
        HTTPClient *httpWeatherReq;
        Chrono *weatherTimer;
        uint32_t epochTimestamp;
        void connectToWifi();
        void getWeatherParameters();
        uint32_t getTimestamp();
        WifiString getTimeFormatted();
        WifiString getDateFormatted();

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
            WifiString timeFormatted;
            WifiString dateFormatted;
        }TIME_VARS;
        
        
        WEATHER_VARS weatherInfo;
        TIME_VARS timeDateInfo;

        WIFI_STATION();
        void initWifiStation();
        void run();
};

#endif

