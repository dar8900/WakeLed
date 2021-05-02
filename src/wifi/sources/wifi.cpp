#include "../headers/wifi.h"
#include "../wifi_cred.h"

WIFI_STATION::WIFI_STATION()
{
    ntpUDP = new WiFiUDP();
    timeClient = new NTPClient(*ntpUDP, NTP_SERVER.c_str(), localHourShift, timeRefreshFrq);
    httpWeatherReq = new HTTPClient();
    weatherTimer = new Chrono(Chrono::SECONDS);
    takeTimeBackUp = new Chrono(Chrono::MILLIS);
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
        weatherInfo.weatherID = weatherInfoJASON["weather"][0]["id"];
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
        if(weatherTimer->hasPassed(300, true))
        {
            weatherHttpJson();
        }
    }
    else
    {
        weatherHttpJson();
    }
    
}


uint32_t WIFI_STATION::getTimestamp(bool WifiConn)
{
    uint32_t ts = 0;
    if(WifiConn)
    {
        ts = timeClient->getEpochTime();
        epochTimestamp = ts;
    }
    else
    {
        ts = epochTimestamp;
    }
    return ts;
}

DispString WIFI_STATION::getTimeFormatted()
{
    uint8_t Hour = 0, Minute = 0;
    WifiString TimeStr = "";
    std::tm *locTime = std::localtime((time_t *)&epochTimestamp);
    Hour = locTime->tm_hour;
    timeDateInfo.dayHour = Hour;
    Minute = locTime->tm_min;
    TimeStr = (Hour < 10? "0" + String(Hour) : String(Hour)) + ":" + (Minute < 10? "0" + String(Minute) : String(Minute));
    return DispString(TimeStr.c_str());
}

DispString WIFI_STATION::getDateFormatted()
{
    uint8_t Day = 0, Month = 0, Year = 0;
    // WifiString DayStr = "", Month = "", YearStr = "";
    WifiString DateStr = "";
    std::tm *locTime = std::localtime((time_t *)&epochTimestamp);
    Year = (1900 + locTime->tm_year) % 100;
    Month = 1 + locTime->tm_mon;
    Day = locTime->tm_mday;
    DateStr = (Day < 10? "0" + String(Day) : String(Day)) + "/" + (Month < 10? "0" + String(Month) : String(Month)) + "/" + String(Year);
    return DispString(DateStr.c_str());
}

void WIFI_STATION::legalHourShift()
{
    uint8_t WeekDay = 0, Month = 0, MonthDay = 0;
    bool LegalHourShift = false;
    std::tm *locTime = std::localtime((time_t *)&epochTimestamp);
    Month = 1 + locTime->tm_mon;
    WeekDay = locTime->tm_wday;
    MonthDay = locTime->tm_mday;
    if(Month >= 3 && Month <= 10)
    {
        if((Month == 3 && MonthDay >= 25 && WeekDay == 0)  || (Month > 3))
        {
            LegalHourShift = true;
        }
        if((Month == 10 && MonthDay >= 25 && WeekDay == 0)  || (Month > 10))
        {
            LegalHourShift = false;
        }
    }
    if(!legalHourIsSetted && LegalHourShift)
    {
        timeClient->setTimeOffset(7200);
        legalHourIsSetted = true;
    }
    if(legalHourIsSetted && !LegalHourShift)
    {
        timeClient->setTimeOffset(3600);
        legalHourIsSetted = false;
    }
}

bool WIFI_STATION::isWifiConnected()
{
    return wifiConnected;
}

void WIFI_STATION::setOfflineTime(uint8_t Hour, uint8_t Minute)
{
    struct tm t;
    uint32_t timestamp = 0;
    std::tm *locTime = std::localtime((time_t *)&epochTimestamp);
    uint8_t Year = (1900 + locTime->tm_year) % 100;
    uint8_t Month = 1 + locTime->tm_mon;
    uint8_t Day = locTime->tm_mday;
    t.tm_year = (2000 + Year) - 1900;  // Year - 1900
    t.tm_mon = Month - 1;           // Month, where 0 = jan
    t.tm_mday = Day;          // Day of the month    
    t.tm_hour = Hour;
    t.tm_min = Minute;
    t.tm_sec = 0;
    timestamp = (uint32_t)mktime(&t); 
    if(!isWifiConnected())
        epochTimestamp = timestamp;
}

void WIFI_STATION::setOfflineDate(uint8_t Day, uint8_t Month, uint8_t Year)
{
    struct tm t;
    uint32_t timestamp = 0;
    std::tm *locTime = std::localtime((time_t *)&epochTimestamp);
    uint8_t Hour = locTime->tm_hour;
    uint8_t Minute = locTime->tm_min;    
    t.tm_year = (2000 + Year) - 1900;  // Year - 1900
    t.tm_mon = Month - 1;           // Month, where 0 = jan
    t.tm_mday = Day;          // Day of the month
    t.tm_hour = Hour;
    t.tm_min = Minute;
    t.tm_sec = 0;    
    timestamp = (uint32_t)mktime(&t); 
    if(!isWifiConnected())
        epochTimestamp = timestamp;
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
        takeTimeBackUp->stop();
        timeClient->update();
        timeDateInfo.timestamp = getTimestamp(wifiConnected);
        timeDateInfo.dateFormatted = getDateFormatted();
        timeDateInfo.timeFormatted = getTimeFormatted();
        getWeatherInfo(false);
        backupTimerStarted = false;
        legalHourShift();
    }
    else
    {
        if(!backupTimerStarted)
        {
            takeTimeBackUp->restart();
            backupTimerStarted = true;
        }
        if(takeTimeBackUp->hasPassed(1000, true))
        {
            epochTimestamp++;
        }
        timeDateInfo.timestamp = getTimestamp(wifiConnected);
        timeDateInfo.dateFormatted = getDateFormatted();
        timeDateInfo.timeFormatted = getTimeFormatted();
    }
    if(WiFi.status() != WL_CONNECTED)
    {
        wifiConnected = false;
    }
    else
    {
        wifiConnected = true;
    }
    
}


