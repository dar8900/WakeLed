#ifndef RESTAPI_SERVER_H
#define RESTAPI_SERVER_H

// #include <ArduinoJson.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

class RESTAPI_SERVER
{
    private:
        ESP8266WebServer *server_RA;
        String *respMsg;
        String *reqMsg;
        const String hostname = "wakeled";
        HTTPMethod *reqMethod;
        void onNotFoundPage();
        void routerRAReq();
        void sendResponse();

    public:
        RESTAPI_SERVER();
        void serverInit();
        void serverRun();
};


#endif