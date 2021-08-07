#include "../headers/restapi_server.h"
#include "../../debug/headers/debug.h"


// ESP8266WebServer server_RA(SERVER_PORT);

void onNotFoundRes()
{
    Server_RA.clearMessages();
    Server_RA.composeAndSendMessage(HTTP_NOT_FOUND);
}

void onUnvailableRes()
{
    Server_RA.clearMessages();
    Server_RA.composeAndSendMessage(HTTP_SERVICE_UVAILABLE);
}

void onRoot()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        Server_RA.respBody = "{\"messaggio\": \"Ciao da WakeLed!\", \"pagine\": [ \
            \"commands (no POST)\", \
            \"time (no POST)\", \
            \"date (no POST)\", \
            \"weather (no POST)\", \
            \"alarm_time\", \
            \"alarm_setting\", \
            \"led_time\", \
            \"snooze_time\", \
            \"restart_alarm_time\", \
            \"display_brightness_mode\", \
            \"display_brightness\", \
            \"backlight_time\", \
            \"fw_version (no POST)\", \
            \"uptime (no POST)\" \
            ], \"comandi\": [ \
            [\"ora_allarme\", \"minuto_allarme\"], \
            \"impostazione_allarme\", \
            \"tempo_led\", \
            \"tempo_snooze\", \
            \"tempo_restart_allarme\", \
            \"modalità_luminosità\", \
            \"luminosità_backlight\", \
            \"tempo_backlight\", \
        ]}";
        HttpCode = HTTP_OK;
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onCommands()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        Server_RA.respBody = "{\"pagine\": [ \
            \"commands (no POST)\", \
            \"time (no POST)\", \
            \"date (no POST)\", \
            \"weather (no POST)\", \
            \"alarm_time\", \
            \"alarm_setting\", \
            \"led_time\", \
            \"snooze_time\", \
            \"restart_alarm_time\", \
            \"display_brightness_mode\", \
            \"display_brightness\", \
            \"backlight_time\", \
            \"fw_version (no POST)\", \
            \"uptime (no POST)\" \
            ], \"comandi\": [ \
            [\"ora_allarme\", \"minuto_allarme\"], \
            \"impostazione_allarme\", \
            \"tempo_led\", \
            \"tempo_snooze\", \
            \"tempo_restart_allarme\", \
            \"modalità_luminosità\", \
            \"luminosità_backlight\", \
            \"tempo_backlight\", \
        ]}";
        HttpCode = HTTP_OK;
    }
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
    }
    Server_RA.composeAndSendMessage(HttpCode);     
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
    }
    Server_RA.composeAndSendMessage(HttpCode);
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
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onWeatherInfo()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"temperatura (°C)\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.temperature;
        Server_RA.respBody += "\", \"umidità (%)\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.humidity;
        Server_RA.respBody += "\", \"meteo\": \"";
        Server_RA.respBody += Server_RA.dataGet.weatherInfo.weather;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
        
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onAlarmTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"ora_allarme\": \"";
        Server_RA.respBody += Server_RA.dataGet.alarmHour;
        Server_RA.respBody += "\", \"minuto_allarme\":\"";
        Server_RA.respBody += Server_RA.dataGet.alarmMinute;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            if(Server_RA.checkExistingKey("ora_allarme") && Server_RA.checkExistingKey("minuto_allarme"))
            {
                uint8_t NewAlarmHour = Server_RA.JSON_Doc["ora_allarme"];
                uint8_t NewAlarmMinute = Server_RA.JSON_Doc["minuto_allarme"];
                if((NewAlarmHour >= 0 && NewAlarmHour <= 23) &&
                    (NewAlarmMinute >= 0 && NewAlarmMinute <= 59))
                {
                    Server_RA.dataPost.alarmHour = NewAlarmHour;
                    Server_RA.dataPost.alarmMinute = NewAlarmMinute;
                    Server_RA.dataPost.flags.alarmTime = true;
                    Server_RA.respBody = "{\"messaggio\": \"Orario allarme impostato\"}";
                    HttpCode = HTTP_OK;
                }
                else
                {
                    HttpCode = HTTP_BAD_REQUEST;           
                }
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"messaggio\": \"chiave comando mancante o errata\"}";           
            }            
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
        }
    }
    Server_RA.composeAndSendMessage(HttpCode); 
}

void onAlarmSetting()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"impostazione_allarme\": \"";
        Server_RA.respBody += Server_RA.dataGet.alarmSettingStr;
        Server_RA.respBody += "\"}";
    }
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            if(Server_RA.checkExistingKey("impostazione_allarme"))
            {
                const char *AlarmStatus = Server_RA.JSON_Doc["impostazione_allarme"];
                String AlarmStatusStr = String(AlarmStatus);
                HttpCode = HTTP_OK;
                if(AlarmStatusStr.equals("set"))
                {
                    Server_RA.dataPost.alarmSet = true;
                    Server_RA.dataPost.flags.alarmSetting = true;
                    Server_RA.respBody = "{\"messaggio\": \"Allarme impostato\"}";
                }
                else if(AlarmStatusStr.equals("reset"))
                {
                    Server_RA.dataPost.alarmSet = false;
                    Server_RA.dataPost.flags.alarmSetting = true;
                    Server_RA.respBody = "{\"messaggio\": \"Allarme non impostato\"}";
                }
                else
                {
                    HttpCode = HTTP_BAD_REQUEST;
                }
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"messaggio\": \"chiave comando mancante o errata\"}"; 
            }
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
        }
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onLedTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tempo_led (min)\": \"";
        Server_RA.respBody += Server_RA.dataGet.ledTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            if(Server_RA.checkExistingKey("tempo_led"))
            {
                uint16_t LedTime = Server_RA.JSON_Doc["tempo_led"];
                if((LedTime >= 5 && LedTime <= 30) && LedTime % 5 == 0)
                {
                    Server_RA.dataPost.ledTime = LedTime;
                    Server_RA.dataPost.flags.setLedTime = true;
                    HttpCode = HTTP_OK;
                }
                else
                {
                    HttpCode = HTTP_BAD_REQUEST;
                }
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"messaggio\": \"chiave comando mancante o errata\"}";           
            }            
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
        }
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onSnoozeTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tempo_snooze (min)\": \"";
        Server_RA.respBody += Server_RA.dataGet.snoozeTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            if(Server_RA.checkExistingKey("tempo_snooze"))
            {
                uint16_t Snoozetime = Server_RA.JSON_Doc["tempo_snooze"];
                if((Snoozetime >= 5 && Snoozetime <= 30) && Snoozetime % 5 == 0)
                {
                    Server_RA.dataPost.snoozeTime = Snoozetime;
                    Server_RA.dataPost.flags.setSnoozeTime = true;
                    HttpCode = HTTP_OK;
                }
                else
                {
                    HttpCode = HTTP_BAD_REQUEST;
                }
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"messaggio\": \"chiave comando mancante o errata\"}";           
            }            
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
        }
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onRestartAlarmTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tempo_restart_allarme (min)\": \"";
        Server_RA.respBody += Server_RA.dataGet.restartAlarmTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            if(Server_RA.checkExistingKey("tempo_restart_allarme"))
            {
                uint16_t RestartTime = Server_RA.JSON_Doc["tempo_restart_allarme"];
                if((RestartTime >= 5 && RestartTime <= 30) && RestartTime % 5 == 0)
                {
                    Server_RA.dataPost.restartAlarmTime = RestartTime;
                    Server_RA.dataPost.flags.setRestartTime = true;
                    HttpCode = HTTP_OK;
                }
                else
                {
                    HttpCode = HTTP_BAD_REQUEST;
                }
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"messaggio\": \"chiave comando mancante o errata\"}";           
            }   
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
        }
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onBrightnessMode()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"modalità_luminosità\": \"";
        Server_RA.respBody += Server_RA.dataGet.displayBrightnessMode;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            if(Server_RA.checkExistingKey("modalità_luminosità"))
            {
                const char *BrightnessModeAuto = Server_RA.JSON_Doc["modalità_luminosità"];
                String BrightnessModeAutoStr = String(BrightnessModeAuto);
                if(BrightnessModeAutoStr.equals("auto"))
                {
                    Server_RA.dataPost.displayBrightnessMode = true;
                    Server_RA.dataPost.flags.setDisplayBrightnessMode = true;
                    HttpCode = HTTP_OK;
                }
                else if(BrightnessModeAutoStr.equals("manuale"))
                {
                    Server_RA.dataPost.displayBrightnessMode = false;
                    Server_RA.dataPost.flags.setDisplayBrightnessMode = true;
                    HttpCode = HTTP_OK;
                }
                else
                {
                    HttpCode = HTTP_BAD_REQUEST;
                }
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"messaggio\": \"chiave comando mancante o errata\"}";           
            }               
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
        }
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onBrightness()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = 0;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"luminosità_backlight\": \"";
        Server_RA.respBody += Server_RA.dataGet.displayBrightness;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            if(Server_RA.checkExistingKey("luminosità_backlight"))
            {
                int Brightness = Server_RA.JSON_Doc["luminosità_backlight"];
                if((Brightness >= 5 && Brightness <= 100) && Brightness % 5 == 0)
                {
                    if(Server_RA.dataGet.displayBrightnessMode.compare("manuale") == 0)
                    {
                        Server_RA.dataPost.displayBrightness = Brightness;
                        Server_RA.dataPost.flags.setDisplayBrightness = true;
                        HttpCode = HTTP_OK;
                    }
                    else
                    {
                        HttpCode = HTTP_NOT_ACCETABLE;
                        Server_RA.respBody = "{\"messaggio\": \"Impossibile eseguire, modalità luminosità impostata su auto\"}";           
                    }
                }
                else
                {
                    HttpCode = HTTP_BAD_REQUEST;
                }
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"messaggio\": \"chiave comando mancante o errata\"}";           
            }   
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
        }
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onBacklightTime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"tempo_backlight (s)\": \"";
        Server_RA.respBody += Server_RA.dataGet.backlightTime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        Server_RA.reqBody = Server_RA.server->arg("plain").c_str();
        if(Server_RA.parseJSONReqDone())
        {
            if(Server_RA.checkExistingKey("tempo_backlight"))
            {
                uint16_t BacklightTime = Server_RA.JSON_Doc["tempo_backlight"];
                if((BacklightTime >= 5 && BacklightTime <= 120) && BacklightTime % 5 == 0)
                {
                    Server_RA.dataPost.backlightTime = BacklightTime;
                    Server_RA.dataPost.flags.setBacklight = true;
                    HttpCode = HTTP_OK;
                }
                else
                {
                    HttpCode = HTTP_BAD_REQUEST;
                }
            }
            else
            {
                HttpCode = HTTP_BAD_REQUEST;
                Server_RA.respBody = "{\"messaggio\": \"chiave comando mancante o errata\"}";           
            }   
        }
        else
        {
            HttpCode = HTTP_SERVER_ERROR;
        }
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onFwVersion()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"versione_fw\": \"";
        Server_RA.respBody += Server_RA.dataGet.fwVersion;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
    }
    Server_RA.composeAndSendMessage(HttpCode);
}

void onUptime()
{
    Server_RA.getMethod(); 
    Server_RA.clearMessages();
    uint16_t HttpCode = NO_CODE;
    if(Server_RA.reqMethod == GET_REQ)
    {
        HttpCode = HTTP_OK;
        Server_RA.respBody = "{\"uptime\": \"";
        Server_RA.respBody += Server_RA.dataGet.uptime;
        Server_RA.respBody += "\"}";
    } 
    else
    {
        HttpCode = HTTP_METHOD_NOT_ALLOWED;
    }
    Server_RA.composeAndSendMessage(HttpCode);
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

bool RESTAPI_SERVER::checkExistingKey(ServerString KeyVal)
{
    bool KeyExists = false;
    JsonVariant Key =  Server_RA.JSON_Doc[KeyVal.c_str()];
    if(!Key.isNull())
    {
        KeyExists = true;
    }
    return KeyExists;
}

bool RESTAPI_SERVER::parseJSONReqDone()
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

void RESTAPI_SERVER::composeAndSendMessage(uint16_t HttpCode)
{
    switch(HttpCode)
    {
        case HTTP_BAD_REQUEST:
            if(respBody.empty())
                respBody = "{\"ERRORE\": \"Errore nella scrittura della richiesta\"}";
            break;
        case HTTP_NOT_FOUND:
            if(respBody.empty())
                respBody = "{\"ERRORE\": \"Pagina non trovata\"}";
            break;
        case HTTP_METHOD_NOT_ALLOWED:
            if(respBody.empty())
                respBody = "{\"ERRORE\": \"Metodo non permesso\"}";
            break;
        case HTTP_NOT_ACCETABLE:
            if(respBody.empty())
                respBody = "{\"ERRORE\": \"Dati immessi non accetabili\"}";
            break;
        case HTTP_SERVICE_UVAILABLE:
            if(respBody.empty())
                respBody = "{\"ERRORE\": \"Pagina non ancora disponibile\"}";
            break;
        case HTTP_OK:
            if(respBody.empty())
                respBody = "{\"messaggio\": \"OK\"}";
            break;
        case NO_CODE:
        case HTTP_SERVER_ERROR:
        default:
            respBody = "{\"ERRORE\": \"Errore nell'elaborare la richiesta JSON\"}";
            HttpCode = HTTP_SERVER_ERROR;
            break;
    }
    server->send(HttpCode, MESSAGE_TYPE, respBody.c_str());
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
    server->on("/commands", onCommands);
    server->on("/time", onTime);
    server->on("/date", onDate);
    server->on("/weather", onWeatherInfo);
    server->on("/alarm_time", onAlarmTime);
    server->on("/alarm_setting", onAlarmSetting);
    server->on("/led_time", onLedTime);
    server->on("/snooze_time", onSnoozeTime);
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