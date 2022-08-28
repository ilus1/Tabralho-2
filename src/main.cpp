#include <cstring>

#include "../inc/CommUart.h"
#include "../inc/crc.h"

const int SEND_MSG_SIZE = 9;
const int RECV_MSG_SIZE = 9;

int main(void) {

    CommUart uart(RECV_MSG_SIZE);
    uart.setup();

    unsigned char abc[9] = {ADDRESS, CODE_23, SCODE_C1, MATRICULA[0], MATRICULA[1], MATRICULA[2], MATRICULA[3] };
	uint16_t crc = computeCrc(abc, 7);
	memcpy(&abc[7], &crc, sizeof(crc));

    uart.send(SEND_MSG_SIZE, abc);

    sleep(1);
    uart.receive(RECV_MSG_SIZE);
    unsigned char *msg_read = uart.getMsgRead();
    switch (msg_read[2]) {
        case SCODE_C1:
            printf("SCODE_C1: %d\n", msg_read[3]);
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