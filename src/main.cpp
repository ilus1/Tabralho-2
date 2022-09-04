#include <cstring>
#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>
#include <stdlib.h>

#include "../inc/Uart.h"
#include "../inc/Pid.h"
#include "../inc/bme280.h"
#include "../inc/AmbientTempSensor.h"

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
    pinMode(HEATER, OUTPUT);

    softPwmCreate(FAN, 0, 100);
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
    uart.sendTimerSignal(0);
}

void heatUp(Uart uart, Pid pid) {
    float referenceTemp = uart.getReferenceTemp();
    float internalTemp = uart.getInternalTemp();
    double intensity;

    setStatus(100.0);
    while(systemWorking && internalTemp <= referenceTemp - referenceTemp / 20) {
        referenceTemp = uart.getReferenceTemp();
        internalTemp = uart.getInternalTemp();

        pid.set_reference_temp(referenceTemp);
        intensity = pid.pid_control(internalTemp);
        uart.sendControlSignal((int) intensity);
        sleep(1);
    }
}

void coolDown(Uart uart, AmbientTempSensor ambientSensor) {
    double ambientTemp = ambientSensor.getAmbientTemp(&bme);
    float internalTemp = uart.getInternalTemp();
    printf("temp ambiente: %lf\n", ambientTemp);
    setStatus(-100.0);
    while(systemWorking && internalTemp > ambientTemp) {
        ambientTemp = ambientSensor.getAmbientTemp(&bme);
        internalTemp = uart.getInternalTemp();
        uart.sendControlSignal(-100);
        sleep(1);
    }
    uart.setSystemStatus(0);
}


void temperatureControl(Uart uart, Pid pid, int *timer) {
    float referenceTemp;
    float internalTemp;
    double intensity;

    while(systemWorking && *timer > 0) {
        referenceTemp = uart.getReferenceTemp();
        internalTemp = uart.getInternalTemp();

        pid.set_reference_temp(referenceTemp);
        intensity = pid.pid_control(internalTemp);
        printf("Referencia %f\tInterna: %f\tSinal de controle: %lf\tTimer: %d\n", referenceTemp, internalTemp, intensity, *timer);
        setStatus(intensity);
        uart.sendControlSignal((int) intensity);
        if(*timer % 60 == 0) uart.sendTimerSignal(*timer/60);
        sleep(1);
        *timer -= 1;
    }
    uart.sendTimerSignal(0);
}


int main(void) {
    Uart uart;
    Pid pid;
    AmbientTempSensor ambientSensor = AmbientTempSensor("/dev/i2c-1", &bme, &id);
    int timer = 0;
    bool isSystemOn = false, isSystemRunning = false;
    signal(SIGINT, stop);

    uart.setup();
    pid.setup(50.0, 0.2, 400.0);
    if (wiringPiSetup() != -1) pinSetup();
    cleanSystemState(uart);
  
    while(systemWorking) {
        switch(uart.getUserInput()) {
            case 1:
                if (!isSystemOn) {
                    printf("Case 1\n");
                    isSystemOn = true;
                    uart.setSystemState(1);
                }
                break;
            case 2:
                if (isSystemOn) {
                    printf("Case 2\n");
                    isSystemOn = false;
                    uart.setSystemState(0);
                    uart.setSystemStatus(0);
                }
                break;
            case 3:
                if (!isSystemRunning) {
                    printf("Case 3\n");
                    isSystemRunning = true;
                    uart.setSystemStatus(1);
                    heatUp(uart, pid);
                    temperatureControl(uart, pid, &timer);
                    coolDown(uart, ambientSensor);
                }
                break;
            case 4:
                if(isSystemRunning) {
                    printf("Case 4\n");
                    isSystemRunning = false;
                    uart.setSystemStatus(0);
                }
                break;
            case 5:
                if(isSystemOn) {
                    printf("Case 5\n");
                    timer += 60;
                    uart.sendTimerSignal(timer/60);
                }
                break;
            case 6:
                if(isSystemOn) {
                    printf("Case 6\n");
                    if (timer <= 60) timer = 0;
                    else timer -= 60;
                    uart.sendTimerSignal(timer/60);
                }
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