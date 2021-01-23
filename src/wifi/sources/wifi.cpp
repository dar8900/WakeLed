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
    if(weatherTimer->hasPassed(900, true))
    {
        httpWeatherReq->begin(apiWeatherCall.c_str());
        int httpResponseCode = httpWeatherReq->GET();
        if(httpResponseCode > 0)
        {
            WifiString Payload = WifiString(httpWeatherReq->getString().c_str());
            DynamicJsonDocument weatherInfoJASON(1024);
            deserializeJson(weatherInfoJASON, Payload.c_str());
            weatherInfo.temperature = weatherInfoJASON["main"]["temp"];
            weatherInfo.perceivedTemp = weatherInfoJASON["main"]["feels_like"];
            weatherInfo.pressure = weatherInfoJASON["main"]["pressure"];
            weatherInfo.humidity = weatherInfoJASON["main"]["humidity"];
            weatherInfo.weatherID = weatherInfoJASON["weather"]["id"];
        }
        httpWeatherReq->end();
    }
}


uint32_t WIFI_STATION::getTimestamp()
{
    uint32_t ts = 0;
    ts = timeClient->getEpochTime();
    epochTimestamp = ts;
    return ts;
}

WifiString WIFI_STATION::getTimeFormatted()
{
    return WifiString(timeClient->getFormattedTime().c_str());
}

WifiString WIFI_STATION::getDateFormatted()
{
    uint8_t Day = 0, Month = 0, Year = 0;
    WifiString DateFormatted = "";
    std::stringstream ss;
    std::tm *locTime = std::localtime((time_t *)&epochTimestamp);
    Year = (1900 + locTime->tm_year) % 100;
    Month = 1 + locTime->tm_mon;
    Day = locTime->tm_mday;
    ss << Day << "/" << Month << "/" << Year;
    DateFormatted = ss.str();
    return DateFormatted;
}

void WIFI_STATION::initWifiStation()
{
    apiWeatherCall = "api.openweathermap.org/data/2.5/weather?id=" + BOLOGNA_ID + "&appid=" + WEATHER_API_KEY;
    connectToWifi();
    if(wifiConnected)
    {
        timeClient->begin();
    }
    else
    {
        timeDateInfo.timestamp = 0;
        timeDateInfo.dateFormatted = "--/--/--";
        timeDateInfo.timeFormatted = "--:--:--";
    }
    
}

void WIFI_STATION::run()
{
    if(wifiConnected)
    {
        timeClient->update();
        timeDateInfo.timestamp = getTimestamp();
        timeDateInfo.dateFormatted = getDateFormatted();
        timeDateInfo.timeFormatted = getTimeFormatted();
        getWeatherParameters();
    }
}
