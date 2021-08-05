#include "../headers/restapi_server.h"
#include "../../debug/headers/debug.h"

ESP8266WebServer server_RA(SERVER_PORT);


ServerString reqMsg;

uint8_t reqMethod = UNKNOWN_REQ;

void getMethod()
{
    HTTPMethod Method = server_RA.method();
    if(Method == HTTP_GET)
    {
        reqMethod = GET_REQ;
    }
    else if(Method == HTTP_POST)
    {
        reqMethod = POST_REQ;
    }
    else
    {
        reqMethod = UNKNOWN_REQ;
    }
}

void onNotFoundPageHandle()
{
    server_RA.send(404, MESSAGE_TYPE, 
    "{\"code\": \"ERROR\", \"message\": \"Pagina non trovata\"}");
}

void onUnvailablePage()
{
    server_RA.send(503, MESSAGE_TYPE, 
    "{\"code\": \"ERROR\", \"message\": \"Pagina non ancora disponibile\"}");
}

void onRootHandle()
{
    getMethod(); 
    ServerString respMsg = "";
    if(reqMethod == GET_REQ)
    {
        respMsg = "{\"code\": \"OK\", \"message\": \"Ciao da WakeLed!\"}";
    } 
    else
    {
        respMsg = "{\"code\": \"ERROR\", \"message\": \"Metodo non permesso qui\"}";
    }
    server_RA.send(200, MESSAGE_TYPE, respMsg.c_str());
}

void serverInit()
{
    // Activate mDNS this is used to be able to connect to the server
    // with local DNS hostmane hostname.local
    if (MDNS.begin(HOSTNAME)) 
    {
        WakeledDebug.writeDebugString("MSDN service started", "serverInit");
    }
    server_RA.on("/", onRootHandle);
    server_RA.on("/get_time", onUnvailablePage);
    server_RA.on("/get_date", onUnvailablePage);
    server_RA.on("/get_weather", onUnvailablePage);
    server_RA.on("/get_alarm_time", onUnvailablePage);
    server_RA.on("/get_led_time", onUnvailablePage);
    server_RA.on("/get_soonze_time", onUnvailablePage);
    server_RA.on("/get_restart_alarm_time", onUnvailablePage);
    server_RA.on("/get_display_brightness_mode", onUnvailablePage);
    server_RA.on("/get_backlight_time", onUnvailablePage);
    server_RA.on("/get_fw_version", onUnvailablePage);
    server_RA.on("/get_uptime", onUnvailablePage);

    server_RA.onNotFound(onNotFoundPageHandle);

    server_RA.begin();
    WakeledDebug.writeDebugString("Server started", "serverInit");
}


void serverRun()
{
    server_RA.handleClient();
}