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
    uint16_t Timeout = 80;
    bool WifiConnected = false;
    WiFi.begin(SSID, PASSWD);
    WakeledDebug.writeDebugString("Connessione in corso...", "connectToWifi", true);
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
        delay(250);
    }
    if(WifiConnected)
    {
        WakeledDebug.writeDebugString("Connessione riuscita", "connectToWifi", true);
    }
    else
    {
        WakeledDebug.writeDebugString("Connessione NON riuscita", "connectToWifi", true);
    }
    
    wifiConnected = WifiConnected;
}


void WIFI_STATION::weatherHttpJson()
{
    httpWeatherReq->begin(apiWeatherCall.c_str());
    int httpResponseCode = httpWeatherReq->GET();
    if(httpResponseCode == 200)
    {
        WifiString Payload = httpWeatherReq->getString();
        DynamicJsonDocument weatherInfoJASON(1024);
        deserializeJson(weatherInfoJASON, Payload.c_str());
        weatherInfo.temperature = weatherInfoJASON["main"]["temp"];
        weatherInfo.temperature -= 273.15;
        weatherInfo.perceivedTemp = weatherInfoJASON["main"]["feels_like"];
        weatherInfo.perceivedTemp -= 273.15;
        weatherInfo.pressure = weatherInfoJASON["main"]["pressure"];
        weatherInfo.humidity = weatherInfoJASON["main"]["humidity"];
        weatherInfo.weatherID = weatherInfoJASON["weather"]["id"];
        if(weatherInfo.weatherID >= 200 && weatherInfo.weatherID < 300)
        {
            weatherInfo.weatherID = TEMPESTA;
        }
        else if (weatherInfo.weatherID >= 300 && weatherInfo.weatherID < 322)
        {
            weatherInfo.weatherID = PIOVIGGINE;
        }
        else if (weatherInfo.weatherID >= 500 && weatherInfo.weatherID < 532)
        {
            weatherInfo.weatherID = PIOGGIA;
        }
        else if (weatherInfo.weatherID >= 600 && weatherInfo.weatherID < 623)
        {
            weatherInfo.weatherID = NEVE;
        }
        else if (weatherInfo.weatherID >= 701 && weatherInfo.weatherID < 782)
        {
            weatherInfo.weatherID = NEBBIA;
        }
        else if (weatherInfo.weatherID == 800)
        {
            weatherInfo.weatherID = LIMPIDO;
        }
        else if (weatherInfo.weatherID >= 801 && weatherInfo.weatherID < 805)
        {
            weatherInfo.weatherID = NUVOLOSO;
        }
        else
        {
            weatherInfo.weatherID = UNKNOWN;
        }
    }
    httpWeatherReq->end();
}

void WIFI_STATION::getWeatherInfo(bool TakeInfoNow)
{ 
    if(!TakeInfoNow)
    {
        if(weatherTimer->hasPassed(900, true))
        {
            weatherHttpJson();
        }
    }
    else
    {
        weatherHttpJson();
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
    uint8_t Hour = 0, Minute = 0;
    WifiString TimeStr = "";
    std::tm *locTime = std::localtime((time_t *)&epochTimestamp);
    Hour = locTime->tm_hour;
    Minute = locTime->tm_min;
    TimeStr = (Hour < 10? "0" + String(Hour) : String(Hour)) + ":" + (Minute < 10? "0" + String(Minute) : String(Minute));
    return TimeStr;
}

WifiString WIFI_STATION::getDateFormatted()
{
    uint8_t Day = 0, Month = 0, Year = 0;
    WifiString DateStr = "";
    std::tm *locTime = std::localtime((time_t *)&epochTimestamp);
    Year = (1900 + locTime->tm_year) % 100;
    Month = 1 + locTime->tm_mon;
    Day = locTime->tm_mday;
    DateStr = (Day < 10? "0" + String(Day) : String(Day)) + "/" + (Month < 10? "0" + String(Month) : String(Month)) + "/" + String(Year);
    return DateStr;
}

void WIFI_STATION::initWifiStation()
{
    apiWeatherCall = "http://api.openweathermap.org/data/2.5/weather?id=" + BOLOGNA_ID + "&appid=" + WEATHER_API_KEY;
    connectToWifi();
    if(wifiConnected)
    {
        timeClient->begin();
        getWeatherInfo(true);
    }
    else
    {
        timeDateInfo.timestamp = 0;
        timeDateInfo.dateFormatted = "--/--/--";
        timeDateInfo.timeFormatted = "--:--";
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
        getWeatherInfo(false);
    }
}


