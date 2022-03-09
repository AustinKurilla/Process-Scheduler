#include "queue.h"

//Takes in an int indicating the number of queues and returns a vector of queues
std::vector<Queue> generateQueues(int numQueues, int time_quantum) {
    std::cout << "Generating queues...\n";
    std::vector<Queue> queues;

    int time_q = 4;
    
    for (int i = 0; i < numQueues; i++) {
        Queue curr_queue;
        curr_queue.time_quantum = time_quantum * std::pow(2, i);
        std::list<Process> curr_proc_vec;
        curr_queue.proc_list = curr_proc_vec;
        
        queues.push_back(curr_queue);
    }
    std::cout << "Generation complete\n";
    return queues;
}

void smallPrint(Queue queue) {
    std::list<Process> :: iterator it;
    std::list<Process> curr_list = queue.proc_list;
    Process temp = Process(-1,-1,-1,-1,-1,-1);

    if (queue.proc_list.empty()) {
        std::cout << "Queue size : 0 \n";
    } else {
        long qSize = queue.proc_list.size();
        std::cout << "Queue size : " << qSize << "\n";
    }

    int curr_printed = 0;
    /*
    while (!queue.proc_list.empty()) {
        std::cout << "Process Slot " << curr_printed << " : ";
        curr_printed++;
        Process curr = queue.proc_list.front();
        curr.printProcess();
        queue.proc_list.pop_front();
    }
    */
    if (!queue.proc_list.empty()) {
        std::cout << "Process Slot " << curr_printed << " : ";
        curr_printed++;
        Process curr = queue.proc_list.front();
        curr.printProcess();
    }
}

void printQueue(Queue queue) {
    //below prints out the whole queue in order of priority and arrival
    std::list<Process> :: iterator it;
    std::list<Process> curr_list = queue.proc_list;
    Process temp = Process(-1,-1,-1,-1,-1,-1);
   // long qSize = curr_list.size();
    for(it = curr_list.begin(); it != curr_list.end(); it++) {
        temp = *it;
        temp.printProcess();
    }
}

std::list<Process> handleRRqueue(Queue* queue, Queue* io, int * last_quantum_q, int curr_queue, double * total_time, int curr_tick) {
    // This is where we'll handle all the RR queue processing.
    std::list<Process> to_demote;

    // Set this process as the active process
    *last_quantum_q = curr_queue;

    if (!queue->proc_list.empty()) {

        Process curr = queue->proc_list.front();
        queue->proc_list.pop_front(); // We'll add it back later if it's not finished or demoted.

        // Set the time quantum if it's a fresh process.
        if (curr.quanRemaining == -1) {
            curr.quanRemaining = queue->time_quantum;
        }

        if (curr.timeRemaining == 1 && curr.io > 0) {
            // Transfer to I/O here.
            *last_quantum_q = -1;
            curr.quanRemaining = -1;
            io->proc_list.push_back(curr);
        } else {
            if (curr.timeRemaining > 0) {
                // If process still has time remaining...
                if (curr.quanRemaining > 0) {
                    curr.timeRemaining--;
                    curr.quanRemaining--;
                    queue->proc_list.push_front(curr);
                } else {
                    // We can demote it here.
                    *last_quantum_q = -1;
                    curr.quanRemaining = -1;
                    to_demote.push_back(curr);
                }
            } else {
                // Process is complete. 
                *last_quantum_q = -1;
                *total_time += ((double) curr_tick - (double) curr.arrival);
            }
        }
    }

    return to_demote;
}

void ageFCFSqueue(Queue* queue, std::vector<Process>* addToTop, int ageInterval) {
    // Changed because it was throwing segfaults / mem issues with sizes of 1 and other wacky conditions.
    std::list<Process> old_age_list = queue->proc_list;
    std::list<Process> new_age_list;

    while(!old_age_list.empty()) {
        Process curr_proc = old_age_list.front();
        old_age_list.pop_front();
        
        if (curr_proc.age >= ageInterval) {
            // Send it back to the top queue and reset the age
            curr_proc.age = 0;
            addToTop->push_back(curr_proc);
        } else {
            // Age it and keep it in the FCFS queue.
            curr_proc.age++;
            new_age_list.push_back(curr_proc);
        }
    }

    queue->proc_list = new_age_list;
}

//handleFCFSqueue will run once per tick in the main tick loop
void handleFCFSqueue(Queue* queue, Queue* io) {
    std::list<Process> fcfsList = queue->proc_list;
    std::list<Process> ioList = io->proc_list;

    //if we are currently in a FCFS queue (maybe this could be checked in main, im not sure how performance will differ)
    if (!fcfsList.empty()) {
        //if one tick before burst ends, remove and add it I/O queue
        if (fcfsList.front().timeRemaining == 1){
            if (fcfsList.front().io > 0) {
                ioList.push_back(fcfsList.front());
            }
            fcfsList.pop_front();
        }
        //otherwise decrement it
        else{
            fcfsList.front().timeRemaining--;
        }
    }

    queue->proc_list = fcfsList;
    io->proc_list = ioList;

}

void handleIOqueue(Queue* io, std::vector<Process>* addToTop){
    // Changed because it was throwing segfaults / mem issues with sizes of 1 and other wacky conditions.
    std::list<Process> temp_io_list = io->proc_list;
    std::list<Process> new_io_list;
    
    while (!temp_io_list.empty()) {
        Process temp_proc = temp_io_list.front();
        temp_io_list.pop_front();
        
        if (temp_proc.io > 0) {
            temp_proc.io--;
            new_io_list.push_back(temp_proc); // Rescue service
        } else {
            addToTop->push_back(temp_proc); // Risin to the top - service
        }
    }

    io->proc_list = new_io_list;
}




