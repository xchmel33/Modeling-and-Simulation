/*
    M|M|1, failure happens every exp(X) for C
 */

#include <iostream>
#include "simlib.h"
using namespace std;

#define C 10     // duration of repair
#define X 50    // interval of incoming failures
#define A 5     // interval of incoming transactions
#define B 1     // duration of transaction service
#define STEP 100
#define NO_STEPS 20

Facility Service("M1 Service");
Histogram Table("Example5 statistics", 0, STEP, NO_STEPS);

class Transaction: public Process{
    double Arrival;
    void Behavior(){
        Arrival = Time;
        Seize(Service);
        Wait(Exponential(B));
        Release(Service);
        Table(Time-Arrival);
    }
};

class Failure: public Process{
    void Behavior(){
        Seize(Service, 2);
        Wait(C);
        Release(Service);
    }
};

class Generator: public Event{
    void Behavior(){
        (new Transaction)->Activate();
        Activate(Time+Exponential(A));
    }
};
class GeneratorF: public Event{
    void Behavior(){
        Activate(Time+Exponential(X));
        (new Failure)->Activate();
    }
};

int main(){
    Print("IMS Example 5\n");
    SetOutput("example5.out");
    Init(0, STEP*NO_STEPS*10);
    (new Generator)->Activate();
    (new GeneratorF)->Activate();
    Run();
    Table.Output();
    Service.Output();
}
