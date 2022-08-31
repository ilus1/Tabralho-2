#include "../inc/Uart.h"
#include "../inc/Modbus.h"

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


const int READ_MSG_SIZE = 100;

Uart::Uart() {
    uart0_filestream = -1;
    modbus = Modbus();
}

void Uart::setup() {
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (uart0_filestream == -1) {
        printf("Failed to open filestream\n");
        return;
    }

    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void Uart::send(int msgSize, unsigned char *message) {
    int bytes_written = write(uart0_filestream, message, msgSize);
    if (bytes_written <= 0) {
        printf("Failed to send data\n");
        return;
    }
}

void Uart::receive() {
    int bytes_read = read(uart0_filestream, (void *)read_buffer, READ_MSG_SIZE);
    if (bytes_read <= 0) {
        printf("Failed to receive data\n");
        return;
    }
}

void Uart::stop() {
    close(uart0_filestream);
}

float Uart::getInternalTemp() {
    float internalTemp;
    unsigned char *message = modbus.internalTempMessage();

    this->send(9, message);
    sleep(1);
    this->receive();

    memcpy(&internalTemp, &this->read_buffer[3], sizeof(float));
    return internalTemp;
}

float Uart::getReferenceTemp() {
    float referenceTemp;

    send(9, modbus.referenceTempMessage());
    sleep(1);
    receive();

    memcpy(&referenceTemp, &this->read_buffer[3], sizeof(float));
    return referenceTemp;
}

int Uart::getUserInput() {
    int userInput;

    send(9, modbus.userInputMessage());
    sleep(1);
    receive();

    memcpy(&userInput, &this->read_buffer[3], sizeof(int));
    return userInput;
}

void Uart::sendControlSignal(int signal) {
    send(9, modbus.sendIntSignalMessage(signal));
}

void Uart::sendReferenceSignal(float signal) {
    send(9, modbus.sendFloatSignalMessage(signal));
}

void Uart::sendSystemState(unsigned char state) {
    send(9, modbus.sendSystemStateMessage(state));
}

void Uart::sendSystemStatus(unsigned char status) {
    send(9, modbus.sendSystemStatusMessage(status));
}

void Uart::sendTimerSignal(unsigned char *signal) {
    send(9, modbus.sendTimerMessage(signal));
}
