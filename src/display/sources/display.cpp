#include "../headers/display.h"

// U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D8, /* dc=*/ D6);


NHDST7565::NHDST7565()
{
    u8g2 = new U8G2_ST7565_NHD_C12864_F_4W_HW_SPI(U8G2_R0, /* cs=*/ D8, /* dc=*/ 9, /* reset=*/ 10);
    u8g2->begin();
}

void NHDST7565::testDisplay()
{
    u8g2->clearBuffer();					// clear the internal memory
    u8g2->setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    u8g2->drawStr(0,10,"Merdona");       	// write something to the internal memory
    u8g2->sendBuffer();				    	// transfer internal memory to the display 
}