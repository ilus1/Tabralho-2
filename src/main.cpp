#include <cstring>
#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>
#include <thread>

#include "../inc/Uart.h"
#include "../inc/Pid.h"


const int SEND_MSG_SIZE = 9;
const int HEATER = 4;
const int FAN = 5;

bool systemWorking = true;

void setStatus(double intensity) {
    if (intensity >= 0) softPwmWrite(HEATER, intensity);
    else {
        intensity > -40 ? intensity = 40: intensity *= -1;
        softPwmWrite(FAN, intensity);
        softPwmWrite(HEATER, 0);
    }
}

void pinSetup() {
    pinMode(FAN, OUTPUT);
    softPwmCreate(FAN, 0, 100);

    pinMode(HEATER, OUTPUT);
    softPwmCreate(HEATER, 0, 100);
}

void stop(int signal) {
    signal = signal;
    systemWorking = false;
}

void cleanState(Uart uart) {
    uart.setSystemState(0);
    uart.setSystemStatus(0);
    uart.sendTimerSignal(0);
}

void temperatureControl(Uart uart, Pid pid) {
    float referenceTemp;
    float internalTemp;
    double intensity;

    while(1) {
        referenceTemp = uart.getReferenceTemp();
        internalTemp = uart.getInternalTemp();

        pid.set_reference_temp(referenceTemp);
        intensity = pid.pid_control(internalTemp);
        printf("Referencia %f\tInterna: %f\tSinal de controle: %lf\n", referenceTemp, internalTemp, intensity);
        setStatus(intensity);
        uart.sendControlSignal((int) intensity);
        sleep(1);
    }
}

void watchUserInputs(int *userInput, Uart uart, bool *isSystemRunning) {
    while(systemWorking) {
        if (!*isSystemRunning) *userInput = uart.getUserInput();
        else usleep(50000);
    }
    std::exit(0);
}


int main(void) {
    Uart uart;
    Pid pid;
    bool isSystemON = false, isSystemRunning = false;
    int userInput = 0;
    signal(SIGINT, stop);

    uart.setup();
    pid.setup(50.0, 0.2, 300.0);
    if (wiringPiSetup() != -1) pinSetup();

    std::thread readUserInputs(watchUserInputs, &userInput, uart, &isSystemRunning);
    
    while(systemWorking) {
        switch(userInput) {
            case 1:
                isSystemON = true;
                uart.setSystemState(1);
                break;
            case 2:
                isSystemON = false;
                uart.setSystemState(0);
                isSystemRunning = false;
                uart.setSystemStatus(0);
                break;
            case 3: 
                if(isSystemON) {
                    isSystemRunning = true;
                    uart.setSystemStatus(1);
                    //temperatureControl(uart, pid);
                }
                break;
            case 4:
                isSystemRunning = false;
                break;
            default:
                usleep(50000);
        }
    }
    printf("Shutting down...\n");

    return 0;
}