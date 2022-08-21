#include "../inc/Pid.h"

Pid::Pid () {
    this->sinal_de_controle_MAX = 100;
    this->sinal_de_controle_MIN = -100;
    this->T = 1;
    this->erro_total = 0.0;
    this->erro_anterior = 0.0;
    this->referencia = 0.0;
}

void Pid::pid_configura_constantes(double Kp_, double Ki_, double Kd_){
    this->Kp = Kp_;
    this->Ki = Ki_;
    this->Kd = Kd_;
}

void Pid::pid_atualiza_referencia(float referencia_){
    this->referencia = (double) referencia_;
}

double Pid::pid_controle(double saida_medida){

    double erro = this->referencia - saida_medida;

    this->erro_total += erro;

    if (this->erro_total >= this->sinal_de_controle_MAX)
    {
        this->erro_total = this->sinal_de_controle_MAX;
    }
    else if (this->erro_total <= this->sinal_de_controle_MIN)
    {
        this->erro_total = this->sinal_de_controle_MIN;
    }

    double delta_error = erro - this->erro_anterior;

    this->sinal_de_controle = this->Kp*erro +
        (this->Ki * this->T)*this->erro_total +
        (this->Kd/this->T)*delta_error;

    if (this->sinal_de_controle >= this->sinal_de_controle_MAX)
    {
        this->sinal_de_controle = this->sinal_de_controle_MAX;
    }
    else if (this->sinal_de_controle <= this->sinal_de_controle_MIN)
    {
        this->sinal_de_controle = this->sinal_de_controle_MIN;
    }

    this->erro_anterior = erro;

    return this->sinal_de_controle;
}
