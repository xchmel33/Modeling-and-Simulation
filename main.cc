#include <iostream>
#include "simlib.h"
using namespace std;

#define NO_WEEKS 1
#define NO_OPERATORS 5
#define NO_FORKLIFTS 2
#define NO_SUPPLY 30

/* Globals */
Facility Forklift[NO_FORKLIFTS];
Facility Operator[NO_OPERATORS];
Facility Supervisor("Inspector");
Facility FifoBuffer("Conveyor belt");
Store Warehouse("Inventory", 10500);
Histogram TableIN("Inbound Table", 0, NO_WEEKS*600, 20); // name, start, step, number of steps
Histogram TableOUT("Outbound Table", 0, NO_WEEKS*3600, 20); // name, start, step, number of steps
bool inventoryInitialized = false;
int break_count, workDay_count;


/* Procedures */
int GetFreeEmployee(const string& type){
    if (type == "Forklift"){
        for (int i = 0; i < NO_FORKLIFTS; ++i) {
            if (!Forklift[i].Busy()){
                return i;
            }
        }
    }
    else if (type == "Operator"){
        for (int i = 0; i < NO_OPERATORS; ++i) {
            if (!Operator[i].Busy()){
                return i;
            }
        }
    }
    return 0; // Unknown type of Employee
}

/* Processes */
// Inbound Warehouse process used for handling incoming stock(boxes)
class Inbound: public Process {
    double Arr;
    void Behavior() {
        int id = 0;
        Arr = Time;

        // initial warehouse inventory
        if (Warehouse.Empty() && !inventoryInitialized){
            Enter(Warehouse, 1600);
            inventoryInitialized = true;
        }

        // Scanning of boxes
        id = GetFreeEmployee("Operator");
        Seize(Forklift[id], 1);
        Wait(Uniform(3,5));
        Release(Forklift[id]);

        // Unloading truck & put-away
        id = GetFreeEmployee("Forklift");
        Seize(Operator[id], 1);
        Wait(Uniform(70, 170));
        Release(Operator[id]);

        // Sorting items inside box
        id = GetFreeEmployee("Operator");
        Seize(Operator[id], 1);
        for (int i = 0; i < 25; ++i) {
            Wait(Uniform(3, 5));
            Enter(Warehouse, 1);
        }
        Release(Operator[id]);

        TableIN(Time-Arr);
    }
};
// Outbound Warehouse process used for handling customer orders
class Outbound: public Process{
    double Arrival;
    void Behavior(){
        int ItemsCount, id = 0;
        Arrival = Time;
        ItemsCount = 1 + (rand() % 4); // generate random number of items in order

        // Wait for warehouse inventory to fill up
        while ((int)Warehouse.Capacity() < ItemsCount){
            Print("Warehouse is empty and orders may not be fulfilled in time!\n");
            Wait(10);
        }

        // Picking items for customer order
        id = GetFreeEmployee("Operator");
        Seize(Operator[id]);
        Wait(Uniform(70, 170));
        Release(Operator[id]);
        Leave(Warehouse, ItemsCount); // remove items from warehouse inventory


        // Checking items
        for (int i = 0; i < ItemsCount; ++i) {
            Seize(Supervisor);
            Seize(FifoBuffer);
            Wait(Uniform(2,5));
            Release(FifoBuffer);
            Release(Supervisor);
        }

        // Packing items
        id = GetFreeEmployee("Operator");
        Seize(Operator[id]);
        Wait(Uniform(70, 160));
        Release(Operator[id]);

        // Loading truck
        id = GetFreeEmployee("Forklift");
        Seize(Forklift[id]);
        Wait(Uniform(30, 90));
        Release(Forklift[id]);

        TableOUT(Time-Arrival);
    }
};
// Process used for handling breaks during and between shifts
class Break: public Process{
public: Break(int wt): Process(){wait_time=wt;};
    int wait_time;
    void Behavior(){
        //printf("%f Break for %d minutes\n",(float)Time/3600, (int)wait_time/60);
        for (int i = 0; i < NO_OPERATORS; ++i) {
            Seize(Operator[i], 2);
        }
        for (int i = 0; i < NO_FORKLIFTS; ++i) {
            Seize(Forklift[i], 2);
        }
        Seize(Supervisor, 1);
        //printf("Siezed all\n");
        Wait((double)wait_time);
        Release(Supervisor);
        for (int i = 0; i < NO_FORKLIFTS; ++i) {
            Release(Forklift[i]);
        }
        for (int i = 0; i < NO_OPERATORS; ++i) {
            Release(Operator[i]);
        }
    }
};

/* Events */
// Inbound transaction(box of items) generator
class GeneratorIN: public Event {
    void Behavior() {
        for (int i = 0; i < NO_SUPPLY; ++i) { // generate 30 transactions(boxes)
            (new Inbound)->Activate();
        }
        Activate(Time+168*3600);
    }
};
// Outbound transaction(customer order) generator
class GeneratorOUT: public Event {
    void Behavior() {
        (new Outbound)->Activate();
        Activate(Time+Uniform(1536, 1920));
    }
};
// Generating 3 types of breaks during and between shifts
class ShiftBreaks: public Event {
    void Behavior() {
        break_count++;

        // weekend breaks
        if (workDay_count == 5){
            workDay_count = 0;
            (new Break(48*3600))->Activate();
            Activate(Time+48*3600+150*60);
        }

        // changing shifts
        else if (break_count == 3){
            Activate(Time+150*60);
        }

        // night breaks
        else if (break_count == 6){
            workDay_count++;
            break_count = 0;
            (new Break(8*3600))->Activate();
            Activate(Time+8*3600+150*60);
        }

        // standard breaks
        else{
            (new Break(900))->Activate();
            Activate(Time+165*60);
        }


    }
};

int main() {
    Print("Warehouse - SIMLIB/C++ operations\n");
    SetOutput("warehouse.out");
    break_count = workDay_count = 0;

    // experiment initialization for X weeks of time(seconds)
    Init(0,NO_WEEKS*168*3600);

    // initialize generators
    (new GeneratorIN)->Activate();
    (new GeneratorOUT)->Activate();
    (new ShiftBreaks)->Activate(Time+150*60);

    // start simulation
    Run();

    // Save output
    TableIN.Output();
    TableOUT.Output();
    Warehouse.Output();

    return 0;
}
