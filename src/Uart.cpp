#include "../inc/Uart.h"
#include "../inc/Modbus.h"

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


const int READ_MSG_SIZE = 9;

Uart::Uart() {
    uart0_filestream = -1;
    modbus = Modbus();
    this->read_buffer = new unsigned char[READ_MSG_SIZE];
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
        return;
    }
}

int Uart::receive() {
    usleep(50000);
    int bytes_read = read(uart0_filestream, (void *)read_buffer, READ_MSG_SIZE);
    if (bytes_read <= 0) {
        return 0;
    }
    return bytes_read;
}

bool Uart::isCorrectTemperature(unsigned char sCode) {
    return sCode == read_buffer[2];
}

void Uart::stop() {
    close(uart0_filestream);
}

float Uart::getInternalTemp() {
    float internalTemp;
    unsigned char *message = modbus.internalTempMessage();

    this->send(9, message);
    this->receive();
    if (!isCorrectTemperature(message[2]))
        return getInternalTemp();

    memcpy(&internalTemp, &this->read_buffer[3], sizeof(float));
    return internalTemp;
}

float Uart::getReferenceTemp() {
    float referenceTemp;
    unsigned char *message = modbus.referenceTempMessage();

    send(9, message);
    receive();
    if (!isCorrectTemperature(message[2]))
        return getReferenceTemp();

    memcpy(&referenceTemp, &this->read_buffer[3], sizeof(float));
    return referenceTemp;
}

int Uart::getUserInput() {
    int userInput;

    send(9, modbus.userInputMessage());
    receive();

    memcpy(&userInput, &this->read_buffer[3], sizeof(int));
    this->read_buffer = new unsigned char[READ_MSG_SIZE];
    return userInput;
}

void Uart::sendControlSignal(int signal) {
    send(13, modbus.sendIntSignalMessage(signal));
    // receive();
}


void Uart::setSystemState(unsigned char state) {
    send(10, modbus.setSystemStateMessage(state));
    receive();
}

// void Uart::sendReferenceSignal(float signal) {
//     send(13, modbus.sendFloatSignalMessage(signal));
//     receive();
// }

void Uart::setSystemStatus(unsigned char status) {
    send(10, modbus.setSystemStatusMessage(status));
    // receive();
}

void Uart::sendTimerSignal(int timer) {
    send(13, modbus.sendTimerMessage(timer));
}
