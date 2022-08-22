#ifndef COMMUART_H_
#define COMMUART_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class CommUart {
    private:
    int uart0_filestream;
    struct termios options;
    unsigned char *send_buffer;
    unsigned *read_buffer;

    public:
    CommUart();
    CommUart(int max_msg_size);
    void setup();
    void send(int msgSize);
    void receive(int msgSize);
    void setMsgToSend(unsigned char *message);
    void stop();
    unsigned * getMsgRead();
};

#endif
