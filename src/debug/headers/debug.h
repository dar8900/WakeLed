#ifndef DEBUG_H
#define DEBUG_H
#include <Arduino.h>

typedef String DebugString;

class DEBUG
{
    private:
        uint32_t baudrate;
        DebugString dbgMessage;
    public:
        DEBUG(uint32_t Baudrate = 115200);
        bool enableDebugLog = true;
        void writeDebugString(DebugString Message, DebugString Function, bool SendNewLine);
};

extern DEBUG WakeledDebug;

#endif


