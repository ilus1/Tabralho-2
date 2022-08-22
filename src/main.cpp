#include "../inc/CommUart.h"

const int SEND_MSG_SIZE = 7;
const int RECV_MSG_SIZE = 7;

const char ADDRESS = 0x01;
const char CODE_16 = 0x16;
const char CODE_23 = 0x23;
const char SCODE_C1 = 0xC1;
const char SCODE_C2 = 0xC2;
const char SCODE_C3 = 0xC3;
const char MATRICULA[4] = { '6', '5', '6', '5' };



int main(void) {

    CommUart uart(RECV_MSG_SIZE);
    uart.setup();

    unsigned char msg_to_send[SEND_MSG_SIZE] = { ADDRESS, CODE_23, SCODE_C1, MATRICULA[0], MATRICULA[1], MATRICULA[2], MATRICULA[3] };
    uart.setMsgToSend(msg_to_send);
    uart.send(SEND_MSG_SIZE);

    delay(1000);
    uart.receive(RECV_MSG_SIZE);
    unsigned char *msg_read = uart.getMsgRead();
    switch (msg_read[2]) {
        case SCODE_C1:
            printf("SCODE_C1: %s\n", &msg_read[3]);
            break;
        case SCODE_C2:
            printf("SCODE_C2: %s\n", &msg_read[3]);
            break;
        case SCODE_C3:
            printf("SCODE_C3: %s\n", &msg_read[3]);
            break;
        default:
            printf("SCODE_C0: %s\n", &msg_read[3]);
            break;
    }
    uart.stop();
    return 0;
}