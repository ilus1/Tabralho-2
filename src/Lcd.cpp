#include "../inc/Lcd.h"

#define ENABLE  0b00000100

const int I2C_ADDR = 0x27;

const int CHARACTER = 1;
const int COMMAND = 0;

const int LINE1_START = 0x80;
const int LINE2_START = 0xC0;

const int INTERNAL_TEMP_POSITION = 0xC4;

const int BACKLIGHT_ON = 0x08;



Lcd::Lcd() {
    fd = wiringPiI2CSetup(I2C_ADDR);
    sendByte(0x33, COMMAND);
    sendByte(0x32, COMMAND);
    sendByte(0x06, COMMAND);
    sendByte(0x0C, COMMAND);
    sendByte(0x28, COMMAND);
    sendByte(0x01, COMMAND);
    delayMicroseconds(500);
}

void Lcd::typeFloat(float number) {
    char buffer[20];
    sprintf(buffer, "%2.2f", number);
    typeLine(buffer);
}

void Lcd::typeInt(int number) {
    char buffer[20];
    sprintf(buffer, "%d", number);
    typeLine(buffer);
}

void Lcd::typeChar(char character)   {
  sendByte(character, CHARACTER);
}

void Lcd::typeLine(const char *message) {
    while (*message) sendByte(*(message++), CHARACTER);
}

void Lcd::typeTimer(int timer) {
    setCursorPosition(LINE1_START);
    typeInt(timer / 60);
    typeChar(':');
    typeInt(timer % 60);
}

void Lcd::typeInternalTemp(float internalTemp) {
    typeLine("TI: ");
    typeFloat(internalTemp);
}

void Lcd::typeReferenceTemp(float referenceTemp) {
    typeLine("TR: ");
    typeFloat(referenceTemp);
}

void Lcd::setHeaderScreen(char *message){
    setCursorPosition(LINE1_START);
    typeLine(message);
}

void Lcd::showHeatingProcess(float referenceTemp, float internalTemp) {
    setCursorPosition(LINE2_START);
    typeReferenceTemp(referenceTemp);
    typeInternalTemp(internalTemp);
}

void Lcd::clearLcd(void)   {
  sendByte(0x01, COMMAND);
  sendByte(0x02, COMMAND);
}

void Lcd::setCursorPosition(int position)   {
  sendByte(position, CHARACTER);
}

void Lcd::sendByte(int bits, int mode)   {
  int bits_high;
  int bits_low;
  bits_high = mode | (bits & 0xF0) | BACKLIGHT_ON ;
  bits_low = mode | ((bits << 4) & 0xF0) | BACKLIGHT_ON ;

  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void Lcd::lcd_toggle_enable(int bits)   {
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}
