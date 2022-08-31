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
    printf("Desligando sistema.\n");
    uart.sendSystemState(0);

    sleep(10);
    printf("Ligando o sistema.\n");
    uart.sendSystemState(1);

    sleep(5);
    printf("Desativando processo.\n");
    uart.sendSystemStatus(0);

    sleep(10);
    printf("Reativando processo.\n");
    uart.sendSystemStatus(1);

    sleep(10);
    uart.stop();
    return 0;
}