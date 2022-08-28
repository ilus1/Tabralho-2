#include <cstring>

#include "../inc/CommUart.h"

const int SEND_MSG_SIZE = 9;

int main(void) {

    CommUart uart;
    uart.setup();

    sleep(1);
    printf("Temperatura interna: %d\n", uart.getInternalTemp());
    uart.stop();
    return 0;
}