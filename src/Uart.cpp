#include "../inc/Uart.h"
#include "../inc/Modbus.h"

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


const int READ_MSG_SIZE = 100;

Uart::Uart() {
    this->uart0_filestream = -1;
    this->modbus = Modbus();
}

void Uart::setup() {
    this->uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (this->uart0_filestream == -1) {
        printf("Failed to open filestream\n");
        return;
    }

    tcgetattr(this->uart0_filestream, &this->options);
    this->options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    this->options.c_iflag = IGNPAR;
    this->options.c_oflag = 0;
    this->options.c_lflag = 0;

    tcflush(this->uart0_filestream, TCIFLUSH);
    tcsetattr(this->uart0_filestream, TCSANOW, &this->options);
}

void Uart::send(int msgSize, unsigned char *message) {
    int bytes_written = write(this->uart0_filestream, message, msgSize);
    if (bytes_written <= 0) {
        printf("Failed to send data\n");
        return;
    }
}

void Uart::receive() {
    int bytes_read = read(this->uart0_filestream, (void *)this->read_buffer, READ_MSG_SIZE);
    if (bytes_read <= 0) {
        printf("Failed to receive data\n");
        return;
    }
}

void Uart::stop() {
    close(this->uart0_filestream);
}

float Uart::getInternalTemp() {
    this->send(9, modbus.internalTempMessage());

    sleep(1);

    this->receive();
    return this->read_buffer[3];
}

float Uart::getReferenceTemp() {
    this->send(9, modbus.referenceTempMessage());

    sleep(1);

    this->receive();
    return this->read_buffer[3];
}

int Uart::getUserInput() {
    this->send(9, modbus.userInputMessage());

    sleep(1);

    this->receive();
    return this->read_buffer[3];
}
