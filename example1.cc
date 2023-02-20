/*
    Customers come to the repair shop in data intervals exponential probability distribution with mean value 20 minutes.
    There are two repairmen in the workshop: one processing and normal orders and the other advanced orders. Every third order is advanced.
    Normal orders take exponential probability distribution with center 15min to finish.
    Advanced orders take exponential probability distribution with center 15min to finish
    The customer is waits for the order, and then he leaves the system.
 */


#include <iostream>
#include "simlib.h"
using namespace std;

#define WAIT_BASIC 15
#define WAIT_ADVANCED 45
#define CUSTOMER_ENTER 20
#define STEP 30
#define NO_STEPS 10


Facility Basic("Basic Customers");
Facility Advanced("Advanced Customers");
Histogram Table("Statistics", 0, STEP, NO_STEPS);
int counter = 0, countBasic = 0, countAdvanced = 0;

class Customer1: public Process{
    double Arrival;
    void Behavior(){
        Arrival = Time;
        counter++;
        if (counter == 3){
            countAdvanced++;
            counter = 0;
            Seize(Advanced);
            Wait(Exponential(WAIT_ADVANCED));
            Release(Advanced);
        }
        else{
            countBasic++;
            Seize(Basic);
            Wait(Exponential(WAIT_BASIC));
            Release(Basic);
        }
        Table(Time-Arrival);
    }
};

class Customer2: public Process{
    double Arrival;
    void Behavior(){
        Arrival = Time;
        double x = Uniform(0, 100);
        if (x <= 33){
            countAdvanced++;
            Seize(Advanced);
            Wait(Exponential(WAIT_ADVANCED));
            Release(Advanced);
        }
        else{
            countBasic++;
            Seize(Basic);
            Wait(Exponential(WAIT_BASIC));
            Release(Basic);
        }
        Table(Time-Arrival);
    }
};

class Generator: public Event{
    void Behavior(){
        (new Customer1)->Activate();
        Activate(Time+Exponential(CUSTOMER_ENTER));
    }
};

int main(){
    Print("IMS Example 1\n");
    SetOutput("example1.out");
    Init(0, STEP*NO_STEPS*10);
    (new Generator)->Activate();
    Run();
    Table.Output();
    ::printf("basic: %d, advanced %d\n",countBasic,countAdvanced);
}