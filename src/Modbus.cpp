#include "../inc/Modbus.h"
#include "../inc/Crc.h"

const unsigned char ADDRESS = 0x01;
const unsigned char CODE_16 = 0x16;
const unsigned char CODE_23 = 0x23;
const unsigned char SCODE_C1 = 0xC1;
const unsigned char SCODE_C2 = 0xC2;
const unsigned char SCODE_C3 = 0xC3;
const unsigned char SCODE_D1 = 0xD1;
// const unsigned char SCODE_D2 = 0xD2;
const unsigned char SCODE_D3 = 0xD3;
const unsigned char SCODE_D4 = 0xD4;
const unsigned char SCODE_D5 = 0xD5;
const unsigned char SCODE_D6 = 0xD6;
const unsigned char ID[4] = { 0x06, 0x05, 0x06, 0x05 };


Modbus::Modbus() {
    this->crcCalculator = Crc();
}

unsigned char * Modbus::createMessage(unsigned char code, unsigned char subcode, int msgSize) {
    unsigned char *message = new unsigned char[msgSize];

    message[0] = ADDRESS;
    message[1] = code;
    message[2] = subcode;
    message[3] = ID[0];
    message[4] = ID[1];
    message[5] = ID[2];
    message[6] = ID[3];
    return message;
}

unsigned char * Modbus::internalTempMessage() {
    unsigned char *message = this->createMessage(CODE_23, SCODE_C1, 9);
    uint16_t crc = crcCalculator.computeCrc(message, 7);

    memcpy(&message[7], &crc, sizeof(crc));

    return message;
}

unsigned char * Modbus::referenceTempMessage() {
    unsigned char *message = this->createMessage(CODE_23, SCODE_C2, 9);
    uint16_t crc = crcCalculator.computeCrc(message, 7);

    memcpy(&message[7], &crc, sizeof(crc));

    return message;
}

unsigned char * Modbus::userInputMessage() {
    unsigned char *message = this->createMessage(CODE_23, SCODE_C3, 9);
    uint16_t crc = crcCalculator.computeCrc(message, 7);

    memcpy(&message[7], &crc, sizeof(crc));

    return message;
}


unsigned char * Modbus::setSystemStateMessage(unsigned char state) {
    unsigned char *message = this->createMessage(CODE_16, SCODE_D3, 10);

    message[7] = state;
    uint16_t crc = crcCalculator.computeCrc(message, 8);
    memcpy(&message[8], &crc, sizeof(crc));

    return message;
}


unsigned char * Modbus::setSystemStatusMessage(unsigned char state) {
    unsigned char *message = this->createMessage(CODE_16, SCODE_D5, 10);

    message[7] = state;
    uint16_t crc = crcCalculator.computeCrc(message, 8);
    memcpy(&message[8], &crc, sizeof(crc));

    return message;
}


unsigned char * Modbus::sendTimerMessage(int timer) {
    unsigned char *message = this->createMessage(CODE_16, SCODE_D6, 13);

    memcpy(&message[7], &timer, sizeof(timer));
    uint16_t crc = crcCalculator.computeCrc(message, 11);
    memcpy(&message[11], &crc, sizeof(crc));

    return message;
}

unsigned char * Modbus::sendIntSignalMessage(int signal) {
    unsigned char *message = this->createMessage(CODE_16, SCODE_D1, 13);

    memcpy(&message[7], &signal, sizeof(signal));
    uint16_t crc = crcCalculator.computeCrc(message, 11);
    memcpy(&message[11], &crc, sizeof(crc));

    return message;
}

// unsigned char * Modbus::sendFloatSignalMessage(float signal) {
//     unsigned char *message = this->createMessage(CODE_16, SCODE_D2, 13);

//     memcpy(&message[7], &signal, sizeof(signal));
//     uint16_t crc = crcCalculator.computeCrc(message, 11);
//     memcpy(&message[11], &crc, sizeof(crc));

//     return message;
// }

