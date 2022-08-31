#ifndef MODBUS_H_
#define MODBUS_H_

#include "../inc/Crc.h"

class Modbus {
    private:
    Crc crcCalculator;

    public:
    Modbus();
    unsigned char * createMessage(unsigned char code, unsigned char subcode, int msgSize);
    unsigned char * internalTempMessage();
    unsigned char * referenceTempMessage();
    unsigned char * userInputMessage();
    unsigned char * sendIntSignalMessage(unsigned char *signal);
    unsigned char * sendFloatSignalMessage(unsigned char *signal);
    unsigned char * sendSystemStateMessage(unsigned char state);
    unsigned char * sendSystemStatusMessage(unsigned char status);
    unsigned char * sendTimerMessage(unsigned char *timer);
};

#endif