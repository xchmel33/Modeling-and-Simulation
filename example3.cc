/*
    M|M|1, if there are more than C transactions waiting in line for service,
    than generating transactions will be in intervals exp(A) otherwise exp(B)
 */

#include <iostream>
#include "simlib.h"
using namespace std;

#define C 10     // max transactions waiting in line for service
#define B 8     // first interval for generating transactions
#define A 4     // second interval for generating transactions
#define X 5     // duration of transaction service
#define STEP 100
#define NO_STEPS 20

Facility Service("M1 Service");
Histogram Table("Example3 statistics", 0, STEP, NO_STEPS);

class Transaction: public Process{
    double Arrival;
    void Behavior(){
        Arrival = Time;
        Seize(Service);
        Wait(Exponential(X));
        Release(Service);
        Table(Time-Arrival);
    }
};

class Generator: public Event{
    void Behavior(){
        (new Transaction)->Activate();
        if (Service.QueueLen() >= C)
            Activate(Time+Exponential(A));
        else
            Activate(Time+Exponential(B));
    }
};

int main(){
    Print("IMS Example 3\n");
    SetOutput("example3.out");
    Init(0, STEP*NO_STEPS*10);
    (new Generator)->Activate();
    Run();
    Table.Output();
}
