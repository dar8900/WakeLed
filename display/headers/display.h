#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

class NHDST7565
{
    public:
        NHDST7565();

    private:
        U8G2_ST7565_NHD_C12864_F_4W_HW_SPI *u8g2;
};

#endif