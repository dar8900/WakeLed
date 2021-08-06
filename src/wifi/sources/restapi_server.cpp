#include "../headers/restapi_server.h"
#include "../../debug/headers/debug.h"



// ESP8266WebServer server_RA(SERVER_PORT);

void getMethod()
{
    HTTPMethod Method = Server_RA.server->method();
    if(Method == HTTP_GET)
    {
        Server_RA.reqMethod = GET_REQ;
    }
    else if(Method == HTTP_POST)
    {
        Server_RA.reqMethod = POST_REQ;
    }
    else
    {
        Server_RA.reqMethod = UNKNOWN_REQ;
    }
}

void onNotFoundPageHandle()
{
    Server_RA.server->send(404, MESSAGE_TYPE, 
    "{\"code\": \"ERROR\", \"message\": \"Pagina non trovata\"}");
}

void onUnvailablePage()
{
    Server_RA.server->send(503, MESSAGE_TYPE, 
    "{\"code\": \"ERROR\", \"message\": \"Pagina non ancora disponibile\"}");
}

void onRootHandle()
{
    getMethod(); 
    ServerString respMsg = "";
    if(Server_RA.reqMethod == GET_REQ)
    {
        respMsg = "{\"code\": \"OK\", \"message\": \"Ciao da WakeLed!\"}";
    } 
    else
    {
        respMsg = "{\"code\": \"ERROR\", \"message\": \"Metodo non permesso qui\"}";
    }
    Server_RA.server->send(200, MESSAGE_TYPE, respMsg.c_str());
}


RESTAPI_SERVER::RESTAPI_SERVER()
{
    server = new ESP8266WebServer(SERVER_PORT); 
}


void RESTAPI_SERVER::serverInit()
{
    // Activate mDNS this is used to be able to connect to the server
    // with local DNS hostmane hostname.local
    if (MDNS.begin(HOSTNAME)) 
    {
        WakeledDebug.writeDebugString("MSDN service started", "serverInit");
    }
    server->on("/", onRootHandle);
    server->on("/get_time", onUnvailablePage);
    server->on("/get_date", onUnvailablePage);
    server->on("/get_weather", onUnvailablePage);
    server->on("/get_alarm_time", onUnvailablePage);
    server->on("/get_led_time", onUnvailablePage);
    server->on("/get_soonze_time", onUnvailablePage);
    server->on("/get_restart_alarm_time", onUnvailablePage);
    server->on("/get_display_brightness_mode", onUnvailablePage);
    server->on("/get_backlight_time", onUnvailablePage);
    server->on("/get_fw_version", onUnvailablePage);
    server->on("/get_uptime", onUnvailablePage);

    server->onNotFound(onNotFoundPageHandle);

    server->begin();
    WakeledDebug.writeDebugString("Server started", "serverInit");
}


void RESTAPI_SERVER::serverRun()
{
    server->handleClient();
}


RESTAPI_SERVER Server_RA;