/*
    M|M|1, every 10th transaction generates failure, repair takes C minutes
 */

#include <iostream>
#include "simlib.h"
using namespace std;

#define C 20    // failure
#define A 5     // interval of incoming transactions
#define B 4     // duration of transaction service
#define STEP 10
#define NO_STEPS 20

Facility Service("M1 Service");
Histogram Table("Example2 statistics", 0, STEP, NO_STEPS);
int counter = 0;

class Transaction: public Process{
    double Arrival;
    void Behavior(){
        Arrival = Time;
        Seize(Service);
        Wait(Exponential(B));
        counter++;              // counter for failures
        if (counter == 10){
            Wait(C);            // wait until failure is repaired
            counter = 0;
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
    Print("IMS Example 2\n");
    SetOutput("example2.out");
    Init(0, STEP*NO_STEPS*10);
    (new Generator)->Activate();
    Run();
    Table.Output();
}
