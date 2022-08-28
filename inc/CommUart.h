#ifndef COMMUART_H_
#define COMMUART_H_

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class CommUart {
    private:
    int uart0_filestream;
    struct termios options;
    unsigned char read_buffer[7];

    public:
    CommUart();
    void setup();
    void send(int msgSize, unsigned char *message);
    void receive();
    void stop();
    float getInternalTemp();
    float getReferenceTemp();
    int getUserInput();
};

#endif
