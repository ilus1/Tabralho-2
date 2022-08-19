#ifndef PID_H_
#define PID_H_

class Pid {

    private:
    const int sinal_de_controle_MAX;
    const int sinal_de_controle_MIN;
    const int T;

    double saida_medida, sinal_de_controle;
    double referencia;
    double Kp;
    double Ki;
    double Kd;
    double erro_total, erro_anterior;

    public:

    Pid();
    void pid_configura_constantes(double Kp_, double Ki_, double Kd_);
    void pid_atualiza_referencia(float referencia_);
    double pid_controle(double saida_medida);
};

#endif /* PID_H_ */
