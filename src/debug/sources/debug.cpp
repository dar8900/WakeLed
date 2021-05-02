#include "../headers/debug.h"

DEBUG::DEBUG(uint32_t Baudrate)
{
    baudrate = Baudrate;
    Serial.begin(baudrate);
}


void DEBUG::writeDebugString(DebugString Message, DebugString Function, bool SendNewLine)
{
	DebugString Msg = "Funzione: " + Function + "() Messaggio: " + Message;
	if(enableDebugLog)
	{
		if(SendNewLine)
		{
			Serial.println(Msg);
		}
		else
		{
			Serial.print(Msg);
		}
	}
}

DEBUG WakeledDebug;