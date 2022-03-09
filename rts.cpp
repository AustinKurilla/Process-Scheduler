#include <stdio.h>
#include <stdlib.h>

// Getting and handling file libraries
#include <fstream>

// Vector array & string handling

#include "process.h"
#include "queue.h"
#include "estring.h"

void realTimeScheduler(const char* enviornment, std::vector<Process> process_vec){
    int curr_tick = 0;
    Queue q_vec;
    std::vector<Process> incomplete;

    while(!process_vec.empty() && !q_vec.proc_list.empty()){
        //check for new arrivals
        //Calculate slack of new arrivals (deadline - curr tick - timeRemaining)
        //Sort q_vec based on lowest slack first
        //Check if current process has passed its deadline, if yes and environment = soft add to incomplete vector if environment = hard kill the scheduler
        //Check if current process has finished executing (timeRemaining = 0)
        //Decrement timeRemaining of curr process
        //Check if there is a process with lower slack than the current one, if yes interrupt and replace curr process
        //Decrement slack of all processes in q_vec
        curr_tick++;
    }
}