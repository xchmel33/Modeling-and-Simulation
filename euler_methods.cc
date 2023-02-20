#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double state[2] = {1.0,2.0}, step = 0.01, time=0, end=20;


void error(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

// pointer to event description
typedef void (*ev_t)(void);

struct ev_notice {              // calendar item
    double atime;               // activation time
    int priority;               // for sorting of the same time events
    ev_t evptr;                 // event function pointer
};

// dump record (for debugging)
void cal_dump_evn(struct ev_notice e);

// limit of calendar size
#define CAL_MAX 100
struct ev_notice calendar_data[CAL_MAX];
unsigned calendar_size = 0;

// initialize calendar
void cal_init(void){
    calendar_size = 0;
}

// return first entry
struct ev_notice cal_get_first(void){
    if (calendar_size == 0)
        error("empty calendar");
    return calendar_data[0];
}

// remove first entry
void cal_pop(void){
    if (calendar_size == 0)
        error("empty calendar");
    memmove(calendar_data, calendar_data + 1,
            sizeof(struct ev_notice) * (calendar_size - 1));
    calendar_size -= 1;
}

// add record
void cal_insert(ev_t ptr, int prio, double t){
    unsigned oldsize = calendar_size;
    if (oldsize + 1 > (sizeof(calendar_data) / sizeof(calendar_data[0])))
        error("calendar overflow");
    // search (sort order: time,priority,FIFO)
    int i = 0;
    while (i < oldsize && calendar_data[i].atime < t)
        ++i;
    while (i < oldsize && calendar_data[i].atime == t
           && calendar_data[i].priority >= prio)
        ++i;
    // create space for new record
    if (i < oldsize) {
        memmove(calendar_data + i + 1, calendar_data + i,
                sizeof(struct ev_notice) * (oldsize - i));
    }
    calendar_size = oldsize + 1;
    calendar_data[i].atime = t;
    calendar_data[i].priority = prio;
    calendar_data[i].evptr = ptr;

#ifdef DEBUG
    printf("# Scheduling calendar[%d/%u] ", i, calendar_size);
    cal_dump_evn(calendar_data[i]);
    printf("\n");
#endif
}

// END event simplifies end condition testing, etc.
void END(void)
{
}

// dump record (for debugging)
void cal_dump_evn(struct ev_notice e){
    printf("(");
    printf("t=%g,", e.atime);
    printf("p=%d,", e.priority);
    if (e.evptr == END)
        printf("e=END");
    else
        printf("e=%p", e.evptr);
    printf(")");
}

// dump all records (for debugging)
void cal_dump(void){
#ifdef DEBUG
    printf("# calendar {\n");
    for (int i = 0; i < calendar_size; ++i) {
        printf("#  %2d: ", i);
        cal_dump_evn(calendar_data[i]);
        printf("\n");
    }
    printf("# }\n");
#endif
}

void Dynamic(double *in){
    in[0] = state[1];
    in[1] = state[0];
}

void EulerStep(double h) {
    double in[2] = {};
    Dynamic(in);
    for (int i = 0; i < 2; ++i) {
        state[i] += h * in[i];
    }
    time += h;
}

void Event(){
    printf("state = %f, %f, \t time= %f\n", state[0], state[1], time);
    cal_insert(Event, 0, time+exp(3));
}

//int main(){
//    cal_init();
//    cal_insert(END, -1, end);
//    cal_insert(Event, 0, time+exp(3));
//    struct ev_notice e;
//    while ((e = cal_get_first()).evptr != END){
//        cal_pop();
//        if (e.atime < end){        // simulation during event
//            while(time <= e.atime){
//                EulerStep(time, state, step);
//            }
//            EulerStep(time, state, e.atime-time);
//            time = e.atime;
//        } else {                    // finish simulation
//            while(time <= end){
//                EulerStep(time, state, step);
//            }
//            EulerStep(time, state, end-time);
//            time = end;
//            e.evptr();
//        }
//    }
//}

int main(){
    // initialization of model+simulator
    time = 0;
    double step = 0.001;          // try 1.0/16

    cal_init();

    cal_insert(END, -100, 1e2);   // set simulation end time
    cal_insert(Event, 1, 5);   // Priority: will be first event2 executed

    cal_dump();

    // start of simulation run
    struct ev_notice e;
    while ((e = cal_get_first()).evptr != END) {
        cal_pop();
        // continuous simulation to next event activation time
        while (time < e.atime) {
            if (time + step * 1.01 > e.atime) {
                // printf("### Time:%g  \treduced_step=%.16g\n", time, e.atime - time);
                EulerStep(e.atime - time);
            } else
                EulerStep(step);
        }
        //time = e.atime;         // no need for this
        e.evptr();              // event
    }
    time = e.atime;             // END time
    // end of simulation run

    cal_dump();
    return 0;
}