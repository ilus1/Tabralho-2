#include <cstring>
#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>
#include <thread>
#include <stdlib.h>

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

void cleanSystemState(Uart uart) {
    uart.setSystemState(0);
    uart.setSystemStatus(0);
    uart.sendTimerSignal(0);
}

void temperatureControl(Uart uart, Pid pid, bool isSystemRunning) {
    float referenceTemp;
    float internalTemp;
    double intensity;

    while(systemWorking && isSystemRunning) {
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
        else usleep(100000);
    }
    std::exit(EXIT_SUCCESS);
}


int main(void) {
    Uart uart;
    Pid pid;
    bool isSystemON = false, isSystemRunning = false;
    int timer = 0;//, userInput = 0;
    signal(SIGINT, stop);

    uart.setup();
    pid.setup(50.0, 0.2, 300.0);
    if (wiringPiSetup() != -1) pinSetup();
    cleanSystemState(uart);

    while(systemWorking) {
        printf("Switch\n");
        switch(uart.getUserInput()) {
            case 1:
                printf("Case 1\n");
                isSystemON = true;
                uart.setSystemState(1);
                break;
            case 2:
                printf("Case 2\n");
                isSystemON = false;
                uart.setSystemState(0);
                isSystemRunning = false;
                uart.setSystemStatus(0);
                break;
            case 3: 
                printf("Case 3\n");
                if(isSystemON) {
                    isSystemRunning = true;
                    uart.setSystemStatus(1);
                    temperatureControl(uart, pid, isSystemRunning);
                }
                break;
            case 4:
                printf("Case 4\n");
                if(isSystemON && isSystemRunning) {
                    isSystemRunning = false;
                    uart.setSystemStatus(0);
                }
                break;
            case 5:
                if(isSystemON && !isSystemRunning) {
                    printf("Case 5\n");
                    timer += 60;
                    uart.sendTimerSignal(timer/60);
                }
                break;
            case 6:
                if(isSystemON && !isSystemRunning) {
                    printf("Case 6\n");
                    if (timer <= 60) timer = 0;
                    else timer -= 60;
                    uart.sendTimerSignal(timer/60);
                }
                break;
            default:
                usleep(30000);
        }
    }

    cleanSystemState(uart);
    uart.stop();
    printf("Shutting down...\n");

    return 0;
}