#include "../headers/display.h"

// U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D8, /* dc=*/ D6);


NHDST7565::NHDST7565()
{
    u8g2 = new U8G2_ST7565_NHD_C12864_F_4W_HW_SPI(U8G2_R0, /* cs=*/ D8, /* dc=*/ 9, /* reset=*/ 10);
    u8g2->begin();
}


uint8_t NHDST7565::setTextLeft()
{
	return 0; 
}

uint8_t NHDST7565::setTextCenter()
{
	return ((DISPLAY_WIDTH - displayText.textWidth) / 2); 
}

uint8_t NHDST7565::setTextRight()
{
	return (DISPLAY_WIDTH - displayText.textWidth - 1); 
}

uint8_t NHDST7565::setTextTop()
{
	return (displayText.textHigh + 1); 
}

uint8_t NHDST7565::setTextMiddle()
{
	return ((DISPLAY_HIGH / 2) + (displayText.textHigh / 2));
}

uint8_t NHDST7565::setTextBottom()
{
	return (DISPLAY_HIGH - 1);
}

void NHDST7565::setTextParams(DispString Text, uint8_t Font)
{
    bool RicalcTextParams = false;
    displayText.textString = Text.c_str();
    if(displayText.oldTextFont != Font)
    {
        displayText.textFont = Font;
        displayText.oldTextFont = Font;
        RicalcTextParams = true;
    }
    if(RicalcTextParams)
    {
        displayText.textWidth = 0;
        displayText.textHigh = 0;
        u8g2->setFont(displayFonts[displayText.textFont]);
        displayText.textWidth = u8g2->getStrWidth(displayText.textString);
        displayText.textHigh = u8g2->getAscent();
    }
}

void NHDST7565::setCoordinates(uint8_t OldXPos, uint8_t OldYPos, uint8_t &NewXPos, uint8_t &NewYPos)
{
	if(OldXPos <= DISPLAY_WIDTH && OldYPos <= DISPLAY_HIGH)
	{
		NewXPos = OldXPos;
		NewYPos = OldYPos + displayText.textHigh;
	}
	else if(OldXPos > DISPLAY_WIDTH && OldYPos <= DISPLAY_HIGH)
	{
		NewYPos = OldYPos + displayText.textHigh;;
		switch(OldXPos)
		{
			case LEFT_POS:
				NewXPos = setTextLeft();
				break;
			case CENTER_POS:
				NewXPos = setTextCenter();
				break;
			case RIGHT_POS:
				NewXPos = setTextRight();
				break;
			default:
				NewXPos = setTextCenter();
				break;
		}
	}
	else if(OldXPos <= DISPLAY_WIDTH && OldYPos > DISPLAY_HIGH)
	{
		NewXPos = OldXPos;
		switch(OldYPos)
		{
			case TOP_POS:
				NewYPos = setTextTop();
				break;
			case MIDDLE_POS:
				NewYPos = setTextMiddle();
				break;
			case BOTTOM_POS:
				NewYPos = setTextBottom();
				break;
			default:
				NewYPos = setTextMiddle();
				break;
		}
	}
	else
	{
		switch(OldXPos)
		{
			case LEFT_POS:
				NewXPos = setTextLeft();
				break;
			case CENTER_POS:
				NewXPos = setTextCenter();
				break;
			case RIGHT_POS:
				NewXPos = setTextRight();
				break;
			default:
				NewXPos = setTextCenter();
				break;
		}
		switch(OldYPos)
		{
			case TOP_POS:
				NewYPos = setTextTop();
				break;
			case MIDDLE_POS:
				NewYPos = setTextMiddle();
				break;
			case BOTTOM_POS:
				NewYPos = setTextBottom();
				break;
			default:
				NewYPos = setTextMiddle();
				break;
		}
	}
}

void NHDST7565::clearDisplay()
{
    u8g2->clearBuffer();
    u8g2->sendBuffer();
}

void NHDST7565::clearBuff()
{
	u8g2->clearBuffer();
}
void NHDST7565::sendBuff()
{
	u8g2->sendBuffer();
}

void NHDST7565::drawUnicodeChar(uint8_t XPos, uint8_t YPos, uint8_t Font, uint16_t CharCode)
{
	uint8_t NewXPos = 0, NewYPos = 0;
	setTextParams(" ", Font);
	setCoordinates(XPos, YPos, NewXPos, NewYPos);
	u8g2->drawGlyph(NewXPos, NewYPos, CharCode);
}

void NHDST7565::drawString(uint8_t XPos, uint8_t YPos, uint8_t Font, DispString String)
{
    uint8_t NewXPos = 0, NewYPos = 0;
    setTextParams(String, Font);
    setCoordinates(XPos, YPos, NewXPos, NewYPos);
    u8g2->drawStr(NewXPos, NewYPos, displayText.textString);
}

void NHDST7565::drawDisplay(void displayRoutine(void))
{
    u8g2->clearBuffer();
    displayRoutine();
    u8g2->sendBuffer();
}

