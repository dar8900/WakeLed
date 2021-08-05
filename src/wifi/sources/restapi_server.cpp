#include "../headers/restapi_server.h"
#include "../../debug/headers/debug.h"

typedef std::string ServerString;

ESP8266WebServer server_RA(SERVER_PORT);

ServerString respMsg;
ServerString reqMsg;

HTTPMethod reqMethod;




void onNotFoundPageHandle()
{
    respMsg = "{\"code\": \"ERROR\", \"message\": \"Pagina non trovata\"}";
    server_RA.send(404, "application/json", respMsg.c_str());
    respMsg.clear();
}


void onRootHandle()
{
    reqMethod = server_RA.method();  
    if(server_RA.method() == HTTP_GET)
    {
        respMsg = "{\"code\": \"OK\", \"message\": \"Ciao da WakeLed!\"}";
    } 
    else
    {
        respMsg = "{\"code\": \"ERROR\", \"message\": \"Metodo non permesso qui\"}";
    }
    server_RA.send(200, "application/json", respMsg.c_str());
    respMsg.clear();
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

    server_RA.onNotFound(onNotFoundPageHandle);

    server_RA.begin();
    WakeledDebug.writeDebugString("Server started", "serverInit");
}


void serverRun()
{
    server_RA.handleClient();
}