#ifndef LCD_H
#define LCD_H

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

class Lcd {
    private:
    int fd;


    public:
    Lcd();
    void cursorLoc(int pos);
    void typeFloat(float number);
    void typeInt(int number);
    void typeChar(char character);
    void typeLine(const char *message);
    void typeTimer(int timer);
    void typeInternalTemp(float internalTemp);
    void typeReferenceTemp(float referenceTemp);
    void setHeaderScreen(char *message);
    void showHeatingProcess(float referenceTemp, float internalTemp);
    void showMenuState(float referenceTemp, int timer);
    void clearLcd();
    void setCursorPosition(int position);
    void sendByte(int bits, int mode);
    void lcd_toggle_enable(int bits);
};


#endif