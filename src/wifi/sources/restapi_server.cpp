#include "../headers/restapi_server.h"
#include "../../debug/headers/debug.h"


// ESP8266WebServer server_RA(SERVER_PORT);

void onNotFoundRes()
{
    Server_RA.clearMessages();
    Server_RA.composeErrorMessage(HTTP_NOT_FOUND);
    Server_RA.server->send(HTTP_NOT_FOUND, MESSAGE_TYPE, Server_RA.reqBody.c_str());
}

void onUnvailableRes()
{
    Server_RA.clearMessages();
    Server_RA.composeErrorMessage(HTTP_SERVICE_UVAILABLE);
    Server_RA.server->send(HTTP_SERVICE_UVAILABLE, MESSAGE_TYPE, Server_RA.reqBody.c_str());
}

void onRoot()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        Server_RA.respBody = "{\"messaggio\": \"Ciao da WakeLed!\"}";
        HttpCode = HTTP_OK;
    } 
    else
    {
        Server_RA.composeErrorMessage(HTTP_METHOD_NOT_ALLOWED);
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tempo_attuale\": \"";
        Server_RA.respBody += Server_RA.dataGet.time;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.composeErrorMessage(HTTP_METHOD_NOT_ALLOWED);
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onDate()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"data_attuale\": \"";
        Server_RA.respBody += Server_RA.dataGet.date;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.composeErrorMessage(HTTP_METHOD_NOT_ALLOWED);
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
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"temperatura\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.temperature;
        Server_RA.respBody += "\", \"umidita_percentuale\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.humidity;
        Server_RA.respBody += "\", \"meteo\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.weather;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.composeErrorMessage(HTTP_METHOD_NOT_ALLOWED);
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onAlarmTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "\"ora_allarme\": \"";
        Server_RA.respBody += Server_RA.dataGet.alarmHour;
        Server_RA.respBody += "\", \"minuto_allarme\":\"}";
        Server_RA.respBody += Server_RA.dataGet.alarmMinute;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            uint8_t NewAlarmHour = Server_RA.JSON_Doc["ora_allarme"];
            uint8_t NewAlarmMinute = Server_RA.JSON_Doc["minuto_allarme"];
            if((NewAlarmHour >= 0 && NewAlarmHour <= 23) &&
                (NewAlarmMinute >= 0 && NewAlarmMinute <= 59))
            {
                Server_RA.dataPost.alarmHour = NewAlarmHour;
                Server_RA.dataPost.alarmMinute = NewAlarmMinute;
                Server_RA.dataPost.flags.alarmTime = true;
                HttpCode = HTTP_OK;
                Server_RA.respBody = "{\"messaggio\": \"Orario allarme impostato\"}";
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.composeErrorMessage(HTTP_BAD_REQUEST);            
            }
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
            Server_RA.composeErrorMessage(HTTP_SERVER_ERROR); 
        }
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onAlarmSetting()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"impostazione_allarme\": \"";
        Server_RA.respBody += Server_RA.dataGet.alarmSettingStr;
        Server_RA.respBody += "\"}";
    }
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            const char *AlarmStatus = Server_RA.JSON_Doc["impostazione_allarme"];
            String AlarmStatusStr = String(AlarmStatus);
            HttpCode = HTTP_OK;
            if(AlarmStatusStr.equals("set"))
            {
                Server_RA.dataPost.alarmSet = true;
                Server_RA.dataPost.flags.alarmSetting = true;
                Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"impostazione_allarme\": \"Allarme impostato\"";
            }
            else if(AlarmStatusStr.equals("reset"))
            {
                Server_RA.dataPost.alarmSet = false;
                Server_RA.dataPost.flags.alarmSetting = true;
                Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"impostazione_allarme\": \"Allarme non impostato\"";
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Errore nella richiesta, dati errati\"}";
            }
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
            Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Errore nell'elaborare la richiesta JSON\"}";
        }
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onLedTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_led\": \"";
        Server_RA.respBody += Server_RA.dataGet.ledTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onSnoozeTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_snooze\": \"";
        Server_RA.respBody += Server_RA.dataGet.snoozeTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onRestartAlarmTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_reset_allarme\": \"";
        Server_RA.respBody += Server_RA.dataGet.restartAlarmTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onBrightnessMode()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"modalità_luminosità\": \"";
        Server_RA.respBody += Server_RA.dataGet.displayBrightnessMode;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
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
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"luminosità_attuale\": \"";
        Server_RA.respBody += Server_RA.dataGet.displayBrightness;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());   
}

void onBacklightTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"tempo_backlight\": \"";
        Server_RA.respBody += Server_RA.dataGet.backlightTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onFwVersion()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"versione_fw\": \"";
        Server_RA.respBody += Server_RA.dataGet.fwVersion;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        Server_RA.respBody = "{\"tipo_messaggio\": \"ERROR\", \"messaggio\": \"Metodo non permesso\"}";
    }
    Server_RA.server->send(HttpCode, MESSAGE_TYPE, Server_RA.respBody.c_str());
}

void onUptime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tipo_messaggio\": \"OK\", \"uptime\": \"";
        Server_RA.respBody += Server_RA.dataGet.uptime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
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
    JSON_Doc.clear();
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

void RESTAPI_SERVER::composeErrorMessage(uint16_t HttpCode)
{
    switch(HttpCode)
    {
        case HTTP_BAD_REQUEST:
            respBody = "{\"ERROR\": \"Errore nella richiesta, dati errati\"}";
            break;
        case HTTP_NOT_FOUND:
            respBody = "{\"ERRORE\": \"Pagina non trovata\"}";
            break;
        case HTTP_METHOD_NOT_ALLOWED:
            respBody = "{\"ERROR\": \"Metodo non permesso\"}";
            break;
        case HTTP_SERVER_ERROR:
            respBody = "{\"ERROR\": \"Errore nell'elaborare la richiesta JSON\"}";
            break;
        case HTTP_SERVICE_UVAILABLE:
            respBody = "{\"ERRORE\": \"Pagina non ancora disponibile\"}";
            break;
        case NO_CODE:
        default:
            WakeledDebug.writeDebugString("Codice errore non gestito", "sendErrorMessage");
            break;
    }
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
    server->on("/alarm_setting", onAlarmSetting);
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