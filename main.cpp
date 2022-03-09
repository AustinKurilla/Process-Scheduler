#include <stdio.h>
#include <stdlib.h>

// Getting and handling file libraries
#include <fstream>

// Vector array & string handling

#include "process.h"
#include "queue.h"
#include "estring.h"

// Debugging timer stuff
#include <chrono>
#include <thread>

using namespace std;


bool checkQueues(vector<Process> * addToTop, Queue * io, vector<Process> * process_vec, vector<Queue> * q_vec, int queue_size) {

    //std::cout << "GUNGA\n";
    for (int i = 0; i < queue_size; i++) {
        if (!q_vec->at(i).proc_list.empty()) {
            return false;
        }
    }
    if (!addToTop->empty() || !io->proc_list.empty() || !process_vec->empty()) return false;

    std::cout << "\033[2J\033[1;1H"; // Clears the console
    smallPrint(*io);
    for (int i = 0; i < queue_size; i++) {
        std::cout << "Queue " << i << " : \n";
        //if (i != 0) {
        smallPrint(q_vec->at(i));
        //}
    }

    return true;
}

void runMFQS(vector<Process> process_vec, vector<Queue> q_vec, int queue_size, double total_burst, int age_interval) {
    vector<Process> addToTop;
    Queue io;
    
    int curr_tick = 0;
    long processVecSize = process_vec.size();
    int last_quantum_q = -1;
    int processTracker = 0;
    double total_time = 0;
    

    // Start process simulation
    while (!checkQueues(&addToTop, &io, &process_vec, &q_vec, queue_size)) {
        // Start by adding the next in line processes to the top queue.
        if(processTracker < processVecSize){
            while(process_vec.at(processTracker).arrival == curr_tick){
                addToTop.push_back(process_vec.at(processTracker));
                processTracker++;
                if(processTracker == processVecSize){
                    process_vec.clear();
                    break;
                }
            }
        }

        addToTop = sortByPriority(addToTop); // sort by priority then add to top queue (queue 0)
        int curr_top = addToTop.size();
        for(int i = 0; i < curr_top; i++){
            q_vec.at(0).proc_list.push_back(addToTop.at(i));
        }
        addToTop.clear(); //clear container each iteration after added
        
        if (curr_tick % 30000 == 0) {
            std::cout << "\033[2J\033[1;1H"; // Clears the console
            std::cout << "I/O Queue " << curr_tick << " TOPSIZE : " << curr_top << " \n";
            smallPrint(io);
            for (int i = 0; i < queue_size; i++) {
                std::cout << "Queue " << i << " : \n";
                //if (i != 0) {
                smallPrint(q_vec.at(i));
                //}
            }
        }


        // Start work on first non-empty queue... (unless there's a quantum queue with time left over.)
        int curr_queue = 0;
        if (last_quantum_q == -1) {
            while(curr_queue < queue_size && q_vec.at(curr_queue).proc_list.empty()) {
                curr_queue++;
            }
        } else {
            curr_queue = last_quantum_q;
        }

        if (!q_vec.at(queue_size-1).proc_list.empty()) {
            ageFCFSqueue(&q_vec.at(queue_size-1), &addToTop, age_interval);
        }

        handleIOqueue(&io, &addToTop);

        if (curr_queue < queue_size) {
            if (curr_queue < queue_size - 1) { // Handle RR
                // Will list of queues that need to be demoted.
                std::list<Process> demotions = handleRRqueue(&(q_vec.at(curr_queue)), &io, &last_quantum_q, curr_queue, &total_time, curr_tick);
                if (!demotions.empty()) {
                    q_vec.at(curr_queue + 1).proc_list.push_back(demotions.front());
                    demotions.pop_front(); // Doesn't return front, just removes it so need to send data separately.
                }
            } else { // Handle FCFS
                handleFCFSqueue(&(q_vec.at(curr_queue)), &io);
                // Turnaround time : 
                // (Time it finished - tick it arrived) / total proc
            }
        }

        curr_tick++;

        // For debugging purposes, adding a delay here to help visualize queue structures.
        //std::chrono::milliseconds timespan(200);
        //std::this_thread::sleep_for(timespan);
    }

    // Get our final statistics
    double result = (double) (total_time / processTracker);
    double result_waiting = (double) ((total_time - total_burst) / processTracker);
    std::cout << "Turnaround time : " << result << "\n";
    std::cout << "Waiting time : " << result_waiting << "\n";
    //std::cout << "Curr Tick : " << curr_tick << "\n";
    //std::cout << "Total Time : " << total_time << "\n";
    //std::cout << "Processes : " << processTracker << "\n";

}

int main(int argc, char **argv) {
    // Will want to pass input file name as parameter.
    vector<Process> process_vec;
    //user input vars below
    int numQueues;
    int ageInterval;
    int timeQuantum;
    int io_enabled;
    string tmp;
    Process temp = Process (-1,-1,-1,-1,-1,-1);

    string file_name;
    std::cout << "Input file name : ";
    std::cin >> file_name;

    std::cout << "Input num_queues : ";
    std::cin >> numQueues;

    std::cout << "Input aging interval : ";
    std::cin >> ageInterval;

    std::cout << "Input time quantum : ";
    std::cin >> timeQuantum;

    std::cout << "Enable I/O? (0 = FALSE | 1 = TRUE)";
    std::cin >> io_enabled;
    
    if (argc > 1) {
        printf("Invalid number of arguments passed to process, expected < 2. (%d)\n", argc);
    } else {
        double total_burst = 0;
        process_vec = generateProcesses(file_name, 50, &total_burst, io_enabled);
        process_vec = sortByArrival(process_vec);
        int queue_size = 5;
        vector<Queue> q_vec = generateQueues(numQueues, timeQuantum);
        // Waiting time = turnaround - burst
        runMFQS(process_vec, q_vec, numQueues, total_burst, ageInterval);
    }
}



