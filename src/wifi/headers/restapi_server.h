#ifndef RESTAPI_SERVER_H
#define RESTAPI_SERVER_H

// #include <ArduinoJson.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <stdint.h>

#define SERVER_PORT 8080
#define HOSTNAME    "wakeled"
#define MESSAGE_TYPE "application/json"

typedef std::string ServerString;

enum
{
    GET_REQ,
    POST_REQ,
    UNKNOWN_REQ
};

enum
{
    HTTP_OK = 200,
    HTTP_BAD_REQUEST = 400,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_SERVER_ERROR = 500,
    NO_CODE = 0
};

class RESTAPI_SERVER
{
    public:
    typedef struct 
    {
        ServerString weather;
        ServerString temperature;
        ServerString humidity;
    }SERVER_WEATHER_INFO;

    typedef struct 
    {
        ServerString time;
        ServerString date;
        SERVER_WEATHER_INFO weatherInfo;
        ServerString alarmTime;
        ServerString alarmSettingStr;
        ServerString ledTime;
        ServerString snoozeTime;
        ServerString restartAlarmTime;
        ServerString displayBrightnessMode;
        ServerString displayBrightness;
        ServerString backlightTime;
        ServerString fwVersion;
        ServerString uptime;
    }RESTAPI_GET_REQ_DATA;


    typedef struct 
    {
        bool alarmTime = false;
        bool alarmSetting = false;
        bool setLedTime = false;
        bool setSnoozeTime = false;
        bool setRestartTime = false;
        bool setDisplayBrightnessMode = false;
        bool setDisplayBrightness = false;
        bool setBacklight = false;
    }SERVER_SETTINGS_FLAG;

    typedef struct 
    {
        SERVER_SETTINGS_FLAG flags;
        uint8_t alarmHour;
        uint8_t alarmMinute;
        bool alarmSet;
        uint16_t ledTime;
        uint16_t snoozeTime;
        uint16_t restartAlarmTime;
        bool displayBrightnessMode;
        int displayBrightness;
        uint16_t backlightTime;
    }RESTAPI_POST_REQ_DATA;

    RESTAPI_SERVER();
    ESP8266WebServer *server;
    uint8_t reqMethod = UNKNOWN_REQ;
    ServerString respBody;
    ServerString reqBody;
    StaticJsonDocument<200> JSON_Doc;
    RESTAPI_GET_REQ_DATA dataGet;
    RESTAPI_POST_REQ_DATA dataPost;
    void clearMessages();
    bool parseJSONReqDone();
    void getMethod(); 
    void serverInit();
    void serverRun();
};


extern RESTAPI_SERVER Server_RA;

#endif