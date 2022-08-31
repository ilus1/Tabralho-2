#ifndef UART_H_
#define UART_H_

#include "../inc/Modbus.h"

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class Uart {
    private:
    int uart0_filestream;
    struct termios options;
    unsigned char read_buffer[100];
    Modbus modbus;
    Crc crc;


    public:
    Uart();
    void setup();
    void send(int msgSize, unsigned char *message);
    void receive();
    void stop();
    float getInternalTemp();
    float getReferenceTemp();
    int getUserInput();
    void sendControlSignal(int signal);
    void sendReferenceSignal(float signal);
    void sendReferenceSignal(unsigned char *signal);
    void sendSystemState(unsigned char state);
    void sendSystemStatus(unsigned char status);
    void sendTimerSignal(unsigned char *signal);
};

#endif
