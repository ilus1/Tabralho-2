#include <cstring>

#include "../inc/CommUart.h"
#include "../inc/crc.h"

const int SEND_MSG_SIZE = 9;
const int RECV_MSG_SIZE = 9;

const unsigned char ADDRESS = 0x01;
const unsigned char CODE_16 = 0x16;
const unsigned char CODE_23 = 0x23;
const unsigned char SCODE_C1 = 0xC1;
const unsigned char SCODE_C2 = 0xC2;
const unsigned char SCODE_C3 = 0xC3;
const unsigned char MATRICULA[4] = { 0x06, 0x05, 0x06, 0x05 };


int main(void) {

    CommUart uart(RECV_MSG_SIZE);
    uart.setup();

    unsigned char abc[7] = {ADDRESS, CODE_23, SCODE_C1, MATRICULA[0], MATRICULA[1], MATRICULA[2], MATRICULA[3] };
	uint16_t crc = computeCrc(abc, 7);
	memcpy(&abc[7], &crc, sizeof(crc));

    uart.setMsgToSend(abc);
    uart.send(SEND_MSG_SIZE, abc);

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
            printf("SCODE_C0: %s\n", &msg_read[3]);
            break;
    }
    uart.stop();
    return 0;
}