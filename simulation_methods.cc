#include <iostream>
#include <cstring>
using namespace std;

#define N 3

void dynamic(double t, double *in, double *y){
    in[0] = y[1];
    in[1] = y[2];
    in[2] = 7*y[1]-2;
}

void rk_step(double t, double step, double *y){
    double k1[N] = {}, k2[N] = {}, start[N] = {}, yin[N] = {};

    for (int i = 0; i < N; i++){
        start[i] = y[i];
    }
    dynamic(t,yin, y);

    for (int i = 0; i < N; i++) {
        k1[i] = yin[i];
        y[i] = start[i]+k1[i]*(3.0/4.0)*step;
    }
    dynamic(t+(3.0/4.0)*step, yin, y);

    for (int i = 0; i < N; i++) {
        k2[i] = yin[i];
        y[i] = start[i] + (k1[i]*(1.0/3.0)+k2[i]*(2.0/3.0))*step;
    }
}

void euler_step(double t, double step, double *y){
    double yin[N] = {};

    dynamic(t, yin, y);
    for (int i = 0; i < N; i++) {
        y[i] += step*yin[i];
    }
}
void print_step(double t, double *y){
    if ((int)(t*100)%10 != 0) return;
    cout << "Time: " << t << " \nY: ";
    for (int i = 0; i < N; ++i) {
        cout << y[i] << " ";
    }
    cout << endl << endl;
}

void rk_main(){
    const double end = 1.00, step = 0.01;
    double y[] = {1,2,3}, t=0;
    while (t < end){
        if ((t+step)*1.01 > end){
            rk_step(t, end-t, y);
            t = end;
        } else {
            rk_step(t, step, y);
            t += step;
        }
        print_step(t, y);
    }
}

void state_event(){
    cout << "State Event occurred!" << endl;
}

void euler_main(){}

void combined_main() {
    const double end = 1.00, base_step = 0.01, main_step = base_step/2;
    double y[] = {4,2,3}, t=0, y_backup[N] = {}, step = base_step, t_backup;
    int total_steps = 0;

    while (t < end){
        t_backup = t; // backup time and y
        for (int i = 0; i < N; i++) {
            y_backup[i] = y[i];
        }

        bool state = y[1] > y[0];
        if ((t+step)*1.01 > end){ // break simulation
            rk_step(t, end-t, y);
            t = end;
        } else { // continue simulation
            rk_step(t, step, y);
            t += step;
        }
        bool newState = y[1] > y[0];

        if (newState != state){ // state event occurred
            if (step <= main_step){
                state_event();
                step = base_step;
            } else { // interpolation
                step = step/2;
                t = t_backup;
                for (int i = 0; i < N; i++) {
                    y[i] = y_backup[i];
                }
            }
        }
        print_step(t, y);
        total_steps++;
    }
    cout << "Total steps: " << total_steps << endl;
}

int main(int argc, char* argv[]){
    if (strcmp(argv[0],"RK")==0)
        rk_main();
    else if (strcmp(argv[0],"Euler")==0)
        euler_main();
//    else if (strcmp(argv[0],"Combined")==0)
//        combined_main();
    else
        combined_main();
    return 0;

}
