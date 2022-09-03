#include <cstring>
#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>
#include <stdlib.h>

#include "../inc/Uart.h"
#include "../inc/Pid.h"
#include "../inc/bme280.h"
#include "../inc/linux_userspace.h"

const int HEATER = 4;
const int FAN = 5;

bool systemWorking = true;
struct bme280_dev bme;
struct identifier id;

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
    uart.receive();
    uart.setSystemState(0);
    uart.setSystemStatus(0);
    uart.sendTimerSignal(0, true);
}

void heatUp(Uart uart) {
    float referenceTemp = uart.getReferenceTemp();
    float internalTemp = uart.getInternalTemp();

    setStatus(100.0);
    printf("Referencia: %f\tInterna: %f\tConta: %f\n", referenceTemp, internalTemp, referenceTemp - referenceTemp / 20);
    while(systemWorking && internalTemp <= referenceTemp - referenceTemp / 5) {
        referenceTemp = uart.getReferenceTemp();
        internalTemp = uart.getInternalTemp();
        uart.sendControlSignal(100);
        sleep(1);
    }

}

void coolDown(Uart uart) {
    double externalTemp = 25;//= bmeGetTemp(&bme);
    float internalTemp = uart.getInternalTemp();

    setStatus(-100.0);
    while(systemWorking && internalTemp > externalTemp) {
        // externalTemp = bmeGetTemp(&bme);
        internalTemp = uart.getInternalTemp();
        uart.sendControlSignal(-100);
        sleep(1);
    }
    uart.setSystemStatus(0);
}


void temperatureControl(Uart uart, Pid pid, bool isSystemRunning, int *timer) {
    float referenceTemp;
    float internalTemp;
    double intensity;

    while(systemWorking && isSystemRunning && *timer > 0) {
        referenceTemp = uart.getReferenceTemp();
        internalTemp = uart.getInternalTemp();

        pid.set_reference_temp(referenceTemp);
        intensity = pid.pid_control(internalTemp);
        printf("Referencia %f\tInterna: %f\tSinal de controle: %lf\tTimer: %d\n", referenceTemp, internalTemp, intensity, *timer);
        setStatus(intensity);
        uart.sendControlSignal((int) intensity);
        if(*timer % 60 == 0) uart.sendTimerSignal(*timer/60, true);
        sleep(1);
        *timer -= 1;
    }
    uart.sendTimerSignal(0, true);
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
    int timer = 0;
    signal(SIGINT, stop);

    uart.setup();
    pid.setup(50.0, 0.2, 400.0);
	// bme280Init("/dev/i2c-1", &bme, &id);
    if (wiringPiSetup() != -1) pinSetup();
    cleanSystemState(uart);

    while(systemWorking) {
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
                isSystemRunning = true;
                uart.setSystemStatus(1);
                heatUp(uart);
                temperatureControl(uart, pid, isSystemRunning, &timer);
                coolDown(uart);
                break;
            case 4:
                printf("Case 4\n");
                isSystemRunning = false;
                uart.setSystemStatus(0);
                break;
            case 5:
                printf("Case 5\n");
                timer += 60;
                uart.sendTimerSignal(timer/60, true);
                break;
            case 6:
                printf("Case 6\n");
                if (timer <= 60) timer = 0;
                else timer -= 60;
                uart.sendTimerSignal(timer/60, false);
                break;
            default:
                usleep(100000);
        }
    }

    cleanSystemState(uart);
    uart.stop();
    printf("Shutting down...\n");

    return 0;
}