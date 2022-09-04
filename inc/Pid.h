#ifndef PID_H_
#define PID_H_

class Pid {

    private:
    int control_signal_MAX;
    int control_signal_MIN;
    int T;
    double measured_output, control_signal;
    double reference_temp;
    double Kp;
    double Ki;
    double Kd;
    double total_error, previous_error;

    public:
    Pid();
    void setup(double Kp_, double Ki_, double Kd_);
    void set_reference_temp(float temp);
    double pid_control(double measured_output);
};

#endif /* PID_H_ */
