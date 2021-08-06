#include "../headers/restapi_server.h"
#include "../../debug/headers/debug.h"


// ESP8266WebServer server_RA(SERVER_PORT);

void onNotFoundRes()
{
    Server_RA.server->send(404, MESSAGE_TYPE, 
    "{\"tipo_messaggio\": \"ERRORE\", \"messaggio\": \"Pagina non trovata\"}");
}

void onUnvailableRes()
{
    Server_RA.server->send(503, MESSAGE_TYPE, 
    "{\"tipo_messaggio\": \"ERRORE\", \"messaggio\": \"Pagina non ancora disponibile\"}");
}

void onRoot()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    if(Server_RA.reqMethod == GET_REQ)
    {
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"messaggio\": \"Ciao da WakeLed!\"}";
    } 
    else
    {
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso qui\"}";
    }
    Server_RA.server->send(200, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_attuale\": \"";
        Server_RA.respBody += Server_RA.dataGet.time;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onDate()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"data_attuale\": \"";
        Server_RA.respBody += Server_RA.dataGet.date;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onWeatherInfo()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"temperatura\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.temperature;
        Server_RA.respBody += "\", \"umidita_percentuale\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.humidity;
        Server_RA.respBody += "\", \"meteo\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.weather;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onAlarmTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"ora_allarme\": \"";
        Server_RA.respBody += Server_RA.dataGet.alarmTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            uint8_t NewAlarmHour = Server_RA.JSON_Doc["ora"];
            uint8_t NewAlarmMinute = Server_RA.JSON_Doc["minuto"];
            WakeledDebug.writeDebugString(String(NewAlarmHour), "onAlarmTime");
            WakeledDebug.writeDebugString(String(NewAlarmMinute), "onAlarmTime");
            HttpCode = 200;
            Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"messaggio\": \"Allarme impostato\"}";
        }
        else
        {
            HttpCode = 500;
            Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Errore nell'elaborare la richiesta JSON\"}";
        }
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onLedTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_led\": \"";
        Server_RA.respBody += Server_RA.dataGet.ledTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onSnoozeTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_snooze\": \"";
        Server_RA.respBody += Server_RA.dataGet.snoozeTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onRestartAlarmTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_reset_allarme\": \"";
        Server_RA.respBody += Server_RA.dataGet.restartAlarmTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onBrightnessMode()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"modalità_luminosità\": \"";
        Server_RA.respBody += Server_RA.dataGet.displayBrightnessMode;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onBrightness()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"luminosità_attuale\": \"";
        Server_RA.respBody += Server_RA.dataGet.displayBrightness;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());   
}

void onBacklightTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_backlight\": \"";
        Server_RA.respBody += Server_RA.dataGet.backlightTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onFwVersion()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"versione_fw\": \"";
        Server_RA.respBody += Server_RA.dataGet.fwVersion;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onUptime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = 200;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"uptime\": \"";
        Server_RA.respBody += Server_RA.dataGet.uptime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = 405;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}


RESTAPI_SERVER::RESTAPI_SERVER()
{
    server = new ESP8266WebServer(SERVER_PORT); 
}

void RESTAPI_SERVER::clearMessages()
{
    reqBody.clear();
    respBody.clear();
}

bool RESTAPI_SERVER:: parseJSONReqDone()
{
    bool ParsingDone = false;
    DeserializationError error = deserializeJson(JSON_Doc, reqBody.c_str());
    if(error)
    {
        WakeledDebug.writeDebugString("Errore nel parsing il JSON", "parseJSONReqDone");
        WakeledDebug.writeDebugString(String(error.f_str()), "parseJSONReqDone");
    }
    else
        ParsingDone = true;
    return ParsingDone;
}

void RESTAPI_SERVER::getMethod()
{
    HTTPMethod Method = server->method();
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


void RESTAPI_SERVER::serverInit()
{
    // Activate mDNS this is used to be able to connect to the server
    // with local DNS hostmane hostname.local
    if (MDNS.begin(HOSTNAME)) 
    {
        WakeledDebug.writeDebugString("MSDN service started", "serverInit");
    }
    server->on("/", onRoot);
    server->on("/time", onTime);
    server->on("/date", onDate);
    server->on("/weather", onWeatherInfo);
    server->on("/alarm_time", onAlarmTime);
    server->on("/led_time", onLedTime);
    server->on("/soonze_time", onSnoozeTime);
    server->on("/restart_alarm_time", onRestartAlarmTime);
    server->on("/display_brightness_mode", onBrightnessMode);
    server->on("/display_brightness", onBrightness);
    server->on("/backlight_time", onBacklightTime);
    server->on("/fw_version", onFwVersion);
    server->on("/uptime", onUptime);

    server->onNotFound(onNotFoundRes);

    server->begin();
    WakeledDebug.writeDebugString("Server started", "serverInit");
}


void RESTAPI_SERVER::serverRun()
{
    server->handleClient();
}


RESTAPI_SERVER Server_RA;