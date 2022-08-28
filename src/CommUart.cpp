#include "../inc/CommUart.h"

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
const unsigned char ID[4] = { 0x06, 0x05, 0x06, 0x05 };

CommUart::CommUart() {}

CommUart::CommUart(int max_msg_size) {
    this->uart0_filestream = -1;
    this->send_buffer = new unsigned char[max_msg_size];
    this->read_buffer = new unsigned char[max_msg_size];
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
    // printf("mostrando mensagem: ");
	// for (int i = 0; i < 9; ++i) printf("%X ", message[i]);
    // printf("%s ", message);
    // printf("\n");

    int bytes_written = write(this->uart0_filestream, message, 9);
    if (bytes_written <= 0) {
        printf("Erro ao enviar dados: %d\n", bytes_written);
        return;
    }
}

void CommUart::receive(int msgSize) {
    int bytes_read = read(this->uart0_filestream, (void *)this->read_buffer, msgSize);
    if (bytes_read <= 0) {
        printf("Erro ao receber dados\n");
        return;
    }
}

unsigned char * CommUart::getMsgRead() {
    return this->read_buffer;
}

void CommUart::stop() {
    close(this->uart0_filestream);
}

void CommUart::getInternalTemp() {

}

