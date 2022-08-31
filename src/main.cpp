#include <cstring>

#include "../inc/Uart.h"

const int SEND_MSG_SIZE = 9;

int main(void) {

    Uart uart;
    uart.setup();

    sleep(1);
    printf("Temperatura interna: %f\n", uart.getInternalTemp());
    printf("Temperatura referencia: %f\n", uart.getReferenceTemp());
    printf("Input usuario: %d\n", uart.getUserInput());
    uart.stop();
    return 0;
}