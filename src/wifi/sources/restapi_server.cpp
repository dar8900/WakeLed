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
    "{\"tipo_messaggio\": \"ERRORE\", \"messaggio\": \"Pagina non trovata\"}");
}

void onUnvailablePage()
{
    Server_RA.server->send(503, MESSAGE_TYPE, 
    "{\"tipo_messaggio\": \"ERRORE\", \"messaggio\": \"Pagina non ancora disponibile\"}");
}

void onRootHandle()
{
    getMethod(); 
    ServerString respMsg = "";
    if(Server_RA.reqMethod == GET_REQ)
    {
        respMsg = "{\"tipo_messaggio\": \"OK\", \"messaggio\": \"Ciao da WakeLed!\"}";
    } 
    else
    {
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso qui\"}";
    }
    Server_RA.server->send(200, MESSAGE_TYPE, respMsg.c_str());
}

void onGetTime()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"tempo_attuale\": \"";
        respMsg += Server_RA.dataGet.time;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetDate()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"data_attuale\": \"";
        respMsg += Server_RA.dataGet.date;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetWeatherInfo()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"temperatura\": \"";
        respMsg += Server_RA.dataGet.weatherInfo.temperature;
        respMsg += "\", \"umidita_percentuale\": \"";
        respMsg += Server_RA.dataGet.weatherInfo.humidity;
        respMsg += "\", \"meteo\": \"";
        respMsg += Server_RA.dataGet.weatherInfo.weather;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetAlarmTime()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"ora_allarme\": \"";
        respMsg += Server_RA.dataGet.alarmTime;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetLedTime()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"tempo_led\": \"";
        respMsg += Server_RA.dataGet.ledTime;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetSnoozeTime()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"tempo_snooze\": \"";
        respMsg += Server_RA.dataGet.snoozeTime;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetRestartAlarmTime()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"tempo_reset_allarme\": \"";
        respMsg += Server_RA.dataGet.restartAlarmTime;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetBrightnessMode()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"modalità_luminosità\": \"";
        respMsg += Server_RA.dataGet.displayBrightnessMode;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetBrightness()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"luminosità_attuale\": \"";
        respMsg += Server_RA.dataGet.displayBrightness;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());   
}

void onGetBacklightTime()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"tempo_backlight\": \"";
        respMsg += Server_RA.dataGet.backlightTime;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetFwVersion()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"versione_fw\": \"";
        respMsg += Server_RA.dataGet.fwVersion;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
}

void onGetUptime()
{
    getMethod(); 
    ServerString respMsg = "";
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        respMsg = "{\"tipo_messaggio\": \"OK\", \"uptime\": \"";
        respMsg += Server_RA.dataGet.uptime;
        respMsg += "\"}";
    } 
    else
    {
        HttpCode = 405;
        respMsg = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, respMsg.c_str());
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
    server->on("/time", onGetTime);
    server->on("/date", onGetDate);
    server->on("/weather", onGetWeatherInfo);
    server->on("/get_alarm_time", onGetAlarmTime);
    server->on("/get_led_time", onGetLedTime);
    server->on("/get_soonze_time", onGetSnoozeTime);
    server->on("/get_restart_alarm_time", onGetRestartAlarmTime);
    server->on("/get_display_brightness_mode", onGetBrightnessMode);
    server->on("/get_display_brightness", onGetBrightness);
    server->on("/get_backlight_time", onGetBacklightTime);
    server->on("/fw_version", onGetFwVersion);
    server->on("/uptime", onGetUptime);

    server->onNotFound(onNotFoundPageHandle);

    server->begin();
    WakeledDebug.writeDebugString("Server started", "serverInit");
}


void RESTAPI_SERVER::serverRun()
{
    server->handleClient();
}


RESTAPI_SERVER Server_RA;