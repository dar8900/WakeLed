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
    GET_TIME = 0,
    GET_DATE,
    GET_WEATHER,
    GET_ALARM_TIME,
    GET_LED_TIME,
    GET_SNOOZE_TIME,
    GET_RESTART_ALARM_TIME,
    GET_DISPLAY_BRIGHTNESS_MODE,
    GET_BACKLIGHT_TIME,
    GET_FW_VERSION,
    GET_UPTIME,
    MAX_RESTAPI_REQ
};

typedef struct 
{
    ServerString time;
    ServerString date;
    ServerString wheater;

}RESTAPI_REQ_DATA;


void serverInit();
void serverRun();
#endif