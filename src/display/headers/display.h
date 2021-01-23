#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

typedef std::string DispString;

class NHDST7565
{
    public:
        enum
        {
            LEFT_POS = 130,
            CENTER_POS,
            RIGHT_POS
        };
        enum
        {
            TOP_POS = 70,
            MIDDLE_POS,
            BOTTOM_POS
        };

        enum
        {
            W_3_H_6 = 0,
            W_5_H_8,
            W_6_H_10,
            W_6_H_13_B,
            W_21_H_33,
            W_16_H_16_ICON,
            MAX_FONTS
        };


        const uint8_t *displayFonts[MAX_FONTS] =
        {
            u8g2_font_tom_thumb_4x6_mr,
            u8g2_font_5x8_mr,
            u8g2_font_6x10_mr,
            u8g2_font_6x13B_mr,
            u8g2_font_inr24_mr,
            u8g2_font_open_iconic_all_2x_t,
        };
	
        NHDST7565();
        void clearDisplay();
        void clearBuff();
        void sendBuff();
        void drawUnicodeChar(uint8_t XPos, uint8_t YPos, uint8_t Font, uint16_t CharCode);
        void drawString(uint8_t XPos, uint8_t YPos, uint8_t Font, DispString String);
        void drawPopUp(DispString PopupText, uint16_t Delay);
        void drawDisplay(void displayRoutine(void));

    private:
    
        typedef struct 
        {
            uint8_t textWidth;
            uint8_t textHigh;
            uint8_t textFont;
            uint8_t oldTextFont;
            const char *textString;
        }DRAW_TEXT;

        U8G2_ST7565_NHD_C12864_F_4W_HW_SPI *u8g2;

        static const uint8_t DISPLAY_WIDTH = 128;
        static const uint8_t DISPLAY_HIGH = 64;

        DRAW_TEXT displayText;
        uint8_t setTextLeft();
        uint8_t setTextCenter();
        uint8_t setTextRight();
        uint8_t setTextTop();
        uint8_t setTextMiddle();
        uint8_t setTextBottom();
        void setTextParams(DispString Text, uint8_t Font);
        void setCoordinates(uint8_t OldXPos, uint8_t OldYPos, uint8_t &NewXPos, uint8_t &NewYPos);

};


#endif