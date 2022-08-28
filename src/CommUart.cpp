#include "../inc/CommUart.h"
#include "../inc/crc.h"

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

const unsigned char ADDRESS = 0x01;
const unsigned char CODE_16 = 0x16;
const unsigned char CODE_23 = 0x23;
const unsigned char SCODE_C1 = 0xC1;
const unsigned char SCODE_C2 = 0xC2;
const unsigned char SCODE_C3 = 0xC3;
const unsigned char SCODE_D1 = 0xD1;
const unsigned char SCODE_D2 = 0xD2;
const unsigned char SCODE_D3 = 0xD3;
const unsigned char SCODE_D5 = 0xD5;
const unsigned char SCODE_D6 = 0xD6;
const unsigned char ID[4] = { 0x06, 0x05, 0x06, 0x05 };

const int READ_MSG_SIZE = 7;

CommUart::CommUart() {
    this->uart0_filestream = -1;
}

void CommUart::setup() {
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

void CommUart::send(int msgSize, unsigned char *message) {
    int bytes_written = write(this->uart0_filestream, message, 9);
    if (bytes_written <= 0) {
        printf("Failed to send data\n");
        return;
    }
}

void CommUart::receive() {
    int bytes_read = read(this->uart0_filestream, (void *)this->read_buffer, READ_MSG_SIZE);
    if (bytes_read <= 0) {
        printf("Failed to receive data\n");
        return;
    }
}

void CommUart::stop() {
    close(this->uart0_filestream);
}

float CommUart::getInternalTemp() {
    unsigned char message[9] = { ADDRESS, CODE_23, SCODE_C1, ID[0], ID[1], ID[2], ID[3] };
    uint16_t crc = computeCrc(message, 7);

	memcpy(&message[7], &crc, sizeof(crc));
    this->send(9, message);

    sleep(1);

    this->receive();
    return this->read_buffer[3];
}

float CommUart::getReferenceTemp() {
    unsigned char message[9] = { ADDRESS, CODE_23, SCODE_C2, ID[0], ID[1], ID[2], ID[3] };
    uint16_t crc = computeCrc(message, 7);

    memcpy(&message[7], &crc, sizeof(crc));
    this->send(9, message);

    sleep(1);

    this->receive();
    return this->read_buffer[3];
}

int CommUart::getUserInput() {
    unsigned char message[9] = { ADDRESS, CODE_23, SCODE_C3, ID[0], ID[1], ID[2], ID[3] };
    uint16_t crc = computeCrc(message, 7);

    memcpy(&message[7], &crc, sizeof(crc));
    this->send(9, message);

    sleep(1);

    this->receive();
    return this->read_buffer[3];
}
