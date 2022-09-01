#include <cstring>
#include <wiringPi.h>
#include <softPwm.h>

#include "../inc/Uart.h"
#include "../inc/Pid.h"


const int SEND_MSG_SIZE = 9;
const int HEATER = 4;
const int FAN = 5;

void setStatus(double activationValue) {
    if (activationValue >= 0) softPwmWrite(HEATER, activationValue);
    else {
        activationValue > -40 ? activationValue = 40: activationValue *= -1;
        softPwmWrite(FAN, activationValue);
        softPwmWrite(HEATER, 0);
    }
}

void pinSetup() {
    pinMode(FAN, OUTPUT); 
    softPwmCreate(FAN, 0, 100);

    pinMode(HEATER, OUTPUT);
    softPwmCreate(HEATER, 0, 100);
}


int main(void) {
    Uart uart;
    Pid pid = Pid();
    float referenceTemp;
    float internalTemp;
    double activationValue;

    uart.setup();
    pid.setup(50.0, 0.2, 300.0);
    if (wiringPiSetup() != -1) pinSetup();

    while(1) {
        referenceTemp = uart.getReferenceTemp();
        internalTemp = uart.getInternalTemp();

        pid.set_reference_temp(referenceTemp);
        activationValue = pid.pid_control(internalTemp);
        printf("Referencia %f\tInterna: %f\tSinal de controle: %lf\n", referenceTemp, internalTemp, activationValue);
        setStatus(activationValue);
        uart.sendControlSignal((int) activationValue);
        sleep(1);
    }

    return 0;
}