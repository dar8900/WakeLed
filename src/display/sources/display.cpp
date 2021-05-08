#include "../headers/display.h"

// U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ D8, /* dc=*/ D6);


NHDST7565::NHDST7565()
{
    u8g2 = new U8G2_ST7565_NHD_C12864_F_4W_HW_SPI(U8G2_R0, /* cs=*/ D8, /* dc=*/ 9, /* reset=*/ 10);
    u8g2->begin();
	displayLed = new LEDS(D6, LEDS::PWM);
	displayLedPwmValue = LEDS::PWM_RANGE / 2;
	displayLedTurnoffTimer = new Chrono(Chrono::SECONDS, false);
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
	return (DISPLAY_WIDTH - displayText.textWidth - 5); 
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
		NewYPos = OldYPos + displayText.textHigh;
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

void NHDST7565::drawLine(uint8_t XStart, uint8_t YStart, uint8_t Len, uint8_t Direction)
{
	switch (Direction)
	{
	case HORIZONTAL:
		u8g2->drawHLine(XStart, YStart, Len);
		break;
	case VERTICAL:
		u8g2->drawVLine(XStart, YStart, Len);
		break;
	default:
		break;
	}
	
}

void NHDST7565::drawAngleLine(uint8_t XStart, uint8_t YStart, uint8_t XEnd, uint8_t YEnd)
{
	u8g2->drawLine(XStart, YStart, XEnd, YEnd);
}

void NHDST7565::drawCircle(uint8_t CenterX, uint8_t CenterY, uint8_t Radius, bool Filled)
{
	if(!Filled)
		u8g2->drawCircle(CenterX, CenterY, Radius);
	else	
		u8g2->drawDisc(CenterX, CenterY, Radius);
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

void NHDST7565::drawPopUp(DispString PopupText, uint16_t Delay)
{
	clearBuff();
	drawLine(0, 0, DISPLAY_WIDTH, HORIZONTAL);
	drawLine(0, 1, DISPLAY_WIDTH, HORIZONTAL);
	drawLine(0, 63, DISPLAY_WIDTH, HORIZONTAL);
	drawLine(0, 64, DISPLAY_WIDTH, HORIZONTAL);	
	drawLine(0, 0, DISPLAY_HIGH, VERTICAL);
	drawLine(1, 0, DISPLAY_HIGH, VERTICAL);
	drawLine(127, 0, DISPLAY_HIGH, VERTICAL);
	drawLine(128, 0, DISPLAY_HIGH, VERTICAL);	
	drawString(CENTER_POS, MIDDLE_POS, W_6_H_13_B, PopupText);
	sendBuff();
	if(Delay > 0)
	{
		delay(Delay);
	}

}

void NHDST7565::drawDisplay(void displayRoutine(void))
{
    u8g2->clearBuffer();
    displayRoutine();
    u8g2->sendBuffer();
}

uint16_t NHDST7565::getDisplayLedTurnoffTime()
{
	return displayLedTurnoffTime;
}

void NHDST7565::setDisplayLedTurnoffTime(uint16_t TurnoffTime)
{
	displayLedTurnoffTime = TurnoffTime;
	restartDisplayLedTimer();
}

void NHDST7565::restartDisplayLedTimer()
{
	displayLedTurnoffTimer->restart();
}

void NHDST7565::displayLedManage()
{
	if(!manualManageDisplayLed)
	{
		if(displayLedTurnoffTimer->hasPassed(displayLedTurnoffTime))
		{
			if(displayLedPwmValue != 0)
			{
				oldDisplayLedPwmValue = displayLedPwmValue;
				displayLedPwmValue = 0;
				displayLed->writePwm(displayLedPwmValue);
			}
		}
		else
		{
			if(displayLedPwmValue == 0)
			{
				displayLedPwmValue = oldDisplayLedPwmValue;
				displayLed->writePwm(displayLedPwmValue);
			}
		}
	}
	else
	{
		displayLedTurnoffTimer->stop();
	}
	// if(oldDisplayLedPwmValue != displayLedPwmValue)
	// {
	// 	oldDisplayLedPwmValue = displayLedPwmValue;
	// 	if(displayLedPwmValue == 0)
	// 	{
	// 		displayLed->writeDigital(OFF);
	// 	}
	// 	else
	// 	{
	// 		displayLed->writePwm(displayLedPwmValue);
	// 	}
	// }
}

void NHDST7565::manualSwitchLedDisplay(bool Status)
{
	if(manualManageDisplayLed)
	{
		if(Status == ON)
		{
			displayLed->writeDigital(ON);
		}
		else
		{
			displayLed->writeDigital(OFF);
		}
	}
}

void NHDST7565::setDisplayLedBrightness(uint8_t Brightness)
{
	if(Brightness >= 0 && Brightness <= 100)
	{
		displayLedPwmValue = (LEDS::PWM_RANGE * Brightness) / 100;
	}
	else
	{
		displayLedPwmValue = LEDS::PWM_RANGE;
	}
}

uint8_t NHDST7565::getDisplayLedBrightness()
{
	uint8_t BrightnessPercent = 0;
	BrightnessPercent = (displayLedPwmValue * 100) / LEDS::PWM_RANGE;
	return BrightnessPercent;
}