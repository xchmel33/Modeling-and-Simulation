#include <iostream>
#include "simlib.h"

class MyFacility {
    Queue queue; // procesy se radi dle jejich priority
    Process currentProcess = None;
    int capacity;

    MyFacility(int cap) {
        capacity = cap;
    }

    void Seize(Process process) {
        if (this->currentProcess == None) {
            this->currentProcess = process;
        } else if (this->queue.Len() < capacity) {
            process.Passivate();
            this->queue.Queue(process);
        } else {
            SIMLIB_Error("The Facility's queue is full");
        }
    }

    void Release(Process process) {
        if (this->currentProcess != process){
            SIMLIB_Error("The Facility cannot be released");
        }
        if (this->queue.Len() > 0){
            this->currentProcess = this->queue.Dequeue();
            this->currentProcess.Activate();
        }
        else{
            this->currentProcess = None;
        }
    }
};
