#ifndef LAB2_QUEUE_H
#define LAB2_QUEUE_H

#include "process.h"
#include <list>
#include <iterator>
#include <math.h>
// Queue structure
struct Queue {
    std::list<Process> proc_list;
    int time_quantum;
    bool round_robin;
    Process* next;
};

// Functions
std::vector<Queue> generateQueues(int numQueues, int time_quantum);
void smallPrint(Queue queue);
void printQueue(std::list<Process> q_vec);
std::list<Process> handleRRqueue(Queue* queue, Queue* io, int* last_quantum_q, int curr_queue, double * total_time, int curr_tick);
void handleFCFSqueue(Queue* queue, Queue* io);
void ageFCFSqueue(Queue* queue, std::vector<Process>* addToTop, int ageInterval);
void handleIOqueue(Queue* io, std::vector<Process>* addToTop);
#endif  // LAB2_QUEUE_H

/*
std::list<Process> handleRRqueue(Queue* queue, std::vector<Process>* addToTop, Queue* io);
void handleFCFSqueue(Queue* queue, std::vector<Process>* addToTop, Queue* io);
*/