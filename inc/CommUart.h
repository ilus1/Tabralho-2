#ifndef COMM_UART_H_
#define COMM_UART_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class CommUart {
    private:
    int msgSize;
    int uart0_filestream;
    struct termios options;
    unsigned char *send_buffer;
    unsigned char *read_buffer;

    public:
    CommUart(int msgSize);
    void setup();
    void send();
    void receive();
    void setMsgToSend(unsigned char *message);
    void stop();
    unsigned char * getMsgRead();
};

#endif
