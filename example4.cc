/*
    M|M|1, if service is taking longer than C, than maintenance for D
 */

#include <iostream>
#include "simlib.h"
using namespace std;

#define C 5    // max duration of service
#define D 10    // duration of maintenance
#define A 8     // interval of incoming transactions
#define B 4     // duration of transaction service
#define STEP 100
#define NO_STEPS 20

Facility Service("M1 Service");
Histogram Table("Example4 statistics", 0, STEP, NO_STEPS);

class Transaction: public Process{
    double Arrival, X;
    void Behavior(){
        Arrival = Time;
        Seize(Service);
        X = Exponential(B);
        Wait(X);
        if (X >= C){
            Wait(D);
        }
        Release(Service);
        Table(Time-Arrival);
    }
};

class Generator: public Event{
    void Behavior(){
        (new Transaction)->Activate();
        Activate(Time+Exponential(A));
    }
};

int main(){
    Print("IMS Example 4\n");
    SetOutput("example4.out");
    Init(0, STEP*NO_STEPS*10);
    (new Generator)->Activate();
    Run();
    Table.Output();
}
