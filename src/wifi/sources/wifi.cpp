#include "../headers/wifi.h"

WIFI_STATION::WIFI_STATION()
{
    ntpUDP = new WiFiUDP();
    timeClient = new NTPClient(*ntpUDP, NTP_SERVER.c_str(), localHourShift, timeRefreshFrq);
    httpWeatherReq = new HTTPClient();
    weatherTimer = new Chrono(Chrono::SECONDS);
}

void WIFI_STATION::connectToWifi()
{
    uint16_t Timeout = 100;
    bool WifiConnected = false;
    WiFi.begin(SSID, PASSWD);
    while(!WifiConnected && Timeout > 0)
    {
        if(WiFi.status() != WL_CONNECTED)
        {
            Timeout--;
        }
        else
        {
            WifiConnected = true;
        }
        delay(100);
    }
    wifiConnected = WifiConnected;
}


void WIFI_STATION:: getWeatherParameters()
{
    if(weatherTimer->hasPassed(5, true))
    {
        httpWeatherReq->begin(apiWeatherCall.c_str());
        int httpResponseCode = httpWeatherReq->GET();
        if(httpResponseCode > 0)
        {
            WifiString Payload = WifiString(httpWeatherReq->getString().c_str());
            DynamicJsonDocument weatherInfoJASON(1024);
            deserializeJson(weatherInfoJASON, Payload.c_str());
            weatherInfo.temperature = weatherInfoJASON["temperature"]["value"];
            
        }
        httpWeatherReq->end();
    }
}



void WIFI_STATION::initWifiStation()
{
    apiWeatherCall = "api.openweathermap.org/data/2.5/weather?id=" + BOLOGNA_ID + "&appid=" + 
                        WEATHER_API_KEY;
    connectToWifi();
    if(wifiConnected)
    {
        timeClient->begin();
    }
}

uint32_t WIFI_STATION::getTimestamp()
{
    return timeClient->getEpochTime();
}

WifiString WIFI_STATION::getTimeFormatted()
{
    return WifiString(timeClient->getFormattedTime().c_str());
}

void WIFI_STATION::run()
{
    
}
