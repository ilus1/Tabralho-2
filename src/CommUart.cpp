#include "../inc/CommUart.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

CommUart::CommUart(int msgSize) {
    this->uart0_filestream = -1;
    this->msgSize = msgSize;
    this->send_buffer = new unsigned char[msgSize];
    this->read_buffer = new unsigned char[msgSize];
}

void CommUart::setup() {
    this->uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->uart0_filestream == -1) {
        printf("Erro ao abrir o arquivo filestream\n");
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

void CommUart::send() {
    int bytes_written = write(this->uart0_filestream, &this->send_buffer[0], this->msgSize);
    if (bytes_written < 0) {
        printf("Erro ao enviar dados\n");
        return;
    }
}

void CommUart::receive() {
    int bytes_read = read(this->uart0_filestream, (void *)this->read_buffer, this->msgSize);
    if (bytes_read < 0) {
        printf("Erro ao receber dados\n");
        return;
    }
}

void CommUart::setMsgToSend(unsigned char *message) {
    this->send_buffer = message;
}

unsigned char * CommUart::getMsgRead() {
    return this->read_buffer;
}

void CommUart::stop() {
    close(this->uart0_filestream);
}

