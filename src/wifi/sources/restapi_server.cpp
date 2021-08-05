#include "../headers/restapi_server.h"

RESTAPI_SERVER::RESTAPI_SERVER()
{
    server_RA = new ESP8266WebServer(8080);
    respMsg = new String();
    reqMsg = new String();
    reqMethod = new HTTPMethod();
}

void RESTAPI_SERVER::serverInit()
{
    // Activate mDNS this is used to be able to connect to the server
    // with local DNS hostmane hostname.local
    if (MDNS.begin(hostname.c_str())) 
    {
        Serial.println("MDNS responder started");
    }
}


void RESTAPI_SERVER::serverRun()
{
    
}