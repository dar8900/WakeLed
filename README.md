# WAKE LED
## Creazione di una sveglia con display lcd, strisce led pilotabili e wifi

### Versione software esp8266 2.7.4, non compatibile con 3.0.0

### Si deve creare un file in src/wifi/ chiamato wifi_cred.h con questo skeleton:
```
#ifndef WIFI_CRED_H
#define WIFI_CRED_H

#define MAX_WIFI_SSID   2

const String SSIDS[MAX_WIFI_SSID] = 
{
    "<SSID1>",
    "<SSID2>",
    "<SSID3>",
};

const String PASSWDS[MAX_WIFI_SSID] = 
{
    "<PASSWD1>",
    "<PASSWD2>",
    "<PASSWD3>",
};

#endif
```