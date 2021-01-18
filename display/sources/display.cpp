#include "../headers/display.h"

// U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D8, /* dc=*/ D6);


NHDST7565::NHDST7565()
{
    u8g2 = new U8G2_ST7565_NHD_C12864_F_4W_HW_SPI(U8G2_R0, /* cs=*/ D8, /* dc=*/ D6);
    
}