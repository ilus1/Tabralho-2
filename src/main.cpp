#include "../inc/CommUart.h"

const int SEND_MSG_SIZE = 7;
const int RECV_MSG_SIZE = 7;

const unsigned char ADDRESS = 0x01;
const unsigned char CODE_16 = 0x16;
const unsigned char CODE_23 = 0x23;
const unsigned char SCODE_C1 = 0xC1;
const unsigned char SCODE_C2 = 0xC2;
const unsigned char SCODE_C3 = 0xC3;
const unsigned char MATRICULA1 = 0x06;
const unsigned char MATRICULA2 = 0x05;
const unsigned char MATRICULA3 = 0x06;
const unsigned char MATRICULA4 = 0x05;



int main(void) {

    CommUart uart(RECV_MSG_SIZE);
    uart.setup();

    unsigned char msg_to_send[SEND_MSG_SIZE] = { ADDRESS, CODE_23, SCODE_C1, MATRICULA1, MATRICULA2, MATRICULA3, MATRICULA4 };
    printf("mostrando mensagem: %s\n", msg_to_send);
    uart.setMsgToSend(msg_to_send);
    uart.send(SEND_MSG_SIZE);

    sleep(1);
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
            for (int i = 0; i < 7; i++)
                printf("%s\n", uart.getMsgRead());
            printf("SCODE_C0: %s\n", &msg_read[3]);
            break;
    }
    uart.stop();
    return 0;
}