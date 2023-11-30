# 1. The Assignment

This assignment aims to implement a system that simulates the control of a digital Air Fryer.

The work involves the development of software that performs the complete control of the AirFryer, including:
* Turning the equipment on/off;
* Controlling the temperature;
* Timing and different food modes.

Specifically, the oven temperature is controlled by two actuating elements:

* A 15 Watts power resistor used to increase temperature;
* A fan that pulls external air (room temperature) to reduce the system temperature.

The user's commands to set the desired temperature will be controlled in three ways:

* Through buttons on the control panel (Via UART);
* Through keyboard input in the terminal;
* Following the pre-defined time and temperatures for each type of food.

Input Buttons

* On/Off  
* Start/Cancel  
* Temperature +/- (Every 5 ˚C)  
* Time +/- (Minutes)



## 2. Code Execution

1) Access the desired Raspberry Pi board:

```sh 
ssh "username"@"board_ip"


```sh 
ssh "nome_de_usuario"@"ip_da_placa"
```

2) After access to the board is granted, clone the project repository:

```sh 
git@github.com:ilus1/Tabralho-2.git
```

3) The program can be run directly with the command:

```sh 
make run
```


# 3. Tests

## Pre-heating
![Pré-Aquecimento](/imgs/Pre-Aquecimento.png)

## Temperature control with timer
![Funcionamento](/imgs/Funcionamento.png)

## Cooling
![Resfriamento](/imgs/Resfriamento.png)

# 4. Notes

The reading of user inputs (through the dashboard buttons) is not always identified, so it is necessary to wait for the dashboard to update and, if necessary, press the button again.
