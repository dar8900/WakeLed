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

class RESTAPI_SERVER
{
    public:

    typedef struct 
    {
        ServerString time;
        ServerString date;
        ServerString wheater;
    }RESTAPI_GET_REQ_DATA;

        RESTAPI_SERVER();
        ESP8266WebServer *server;
        uint8_t reqMethod = UNKNOWN_REQ;
        RESTAPI_GET_REQ_DATA dataGet;
        void serverInit();
        void serverRun();
};


extern RESTAPI_SERVER Server_RA;

#endif