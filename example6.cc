/*
    M|M|1, if front > 5 than M|M|2 for 15 time units
 */

#include <iostream>
#include "simlib.h"
using namespace std;

#define A 5     // interval of incoming transactions
#define B 8     // duration of transaction service
#define C 15     // duration M|M|2 service
#define STEP 10
#define NO_STEPS 20

Store Service("Service", 1);
Histogram Table("Example6 statistics", 0, STEP, NO_STEPS);

class ExtraService: public Process{
    void Behavior(){
        if (Service.Capacity() == 1){
            Service.SetCapacity(2);
        }
        Wait(C);
        Service.Clear();
    }
};

class Transaction: public Process{
    double Arrival;
    void Behavior(){
        Arrival = Time;
        if (Service.QueueLen() > 5){
            (new ExtraService)->Activate();
        }
        Enter(Service);
        Wait(Exponential(B));
        if (!Service.Empty()){
            Leave(Service);
        }
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
    Print("IMS Example 6\n");
    SetOutput("example6.out");
    Init(0, STEP*NO_STEPS*10);
    (new Generator)->Activate();
    Run();
    Table.Output();
    Service.Output();
}
