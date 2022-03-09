#include "process.h"

// New process
Process::Process(int new_pid, int new_burst, int new_arrival, int new_priority, int new_deadline, int new_io) {
    pid = new_pid;
    burst = new_burst;
    arrival = new_arrival;
    priority = new_priority;
    deadline = new_deadline;
    io = new_io;
    timeRemaining = new_burst;
    age = 0;
    currQ = 0;
    slack = 0;
}
    
// Used to get the current age of a Process.
int Process::getAge(int ticks_elapsed) {
    return (ticks_elapsed - arrival);
}

void Process::printProcess() {
    std::cout   << "PID : [" 
                << pid
                << "] BUR : ["
                << burst
                << "] ARR : ["
                << arrival
                << "] PRI : ["
                << priority               
                << "] DLNE : ["
                << deadline
                << "] IO : ["
                << io
                << "] TIME_R : ["
                << timeRemaining
                << "] AGE : ["
                << age
                << "] CURR_Q : ["
                << currQ
                << "]\n";
}

/*
 Processes a text file into a Process vector, storing Gantt chart information into a custom C++ class.
 Parameters :
 - string: File name + directory string.
 - int: Max length per new line.
 Returns :
 - vector<Process>: Array of custom Process objects (each line from input file is a new 'Process' object in the vector)
 */
std::vector<Process> generateProcesses(std::string file_name, int max_line, double * total_burst, int io_enabled) {
    std::ifstream infile;
    infile.open(file_name);
    char* templine = (char*) calloc(50, sizeof(char));
    infile.getline(templine, 50);
    std::vector<std::string> new_vec;
    std::vector<Process> process_vec;
    
    if (!infile.is_open()){
        std::cout << "Couldn't open file" << "\n";
    }
    else{
        // Processes provided file line by line, creating a vector of strings separated by indents.
        std::cout << "Proccessing input" << "\n";
        while (!infile.eof()) {
            // Defines each loop by file's new line, assumes each line will be under specified 'max_line' characters.
            infile.getline(templine, 50);
            std::string tempStr (templine);
            // Split defined in enhancedstrings.cpp, gives a vector of strings separated by '\t' (indentation)
            new_vec = split(tempStr, '\t');
            int count;
            try {
                
                int pid = std::stoi(new_vec.at(0), nullptr, 10);
                count = pid;
                int burst = std::stoi(new_vec.at(1), nullptr, 10);
                *total_burst += (u_int) burst;
                int arr = std::stoi(new_vec.at(2), nullptr, 10);
                int prio = std::stoi(new_vec.at(3), nullptr, 10);
                int deadline = std::stoi(new_vec.at(4), nullptr, 10);
                int io = 0;
                if (io_enabled) io = std::stoi(new_vec.at(5), nullptr, 10);
                
                if(burst > io && burst >= 0 && arr >= 0 && prio >= 0 && deadline >= 0 && io >= 0){
                    Process curr_proc = Process(pid, burst, arr, prio, deadline, io);
                    process_vec.push_back(curr_proc);
                }
                
            } catch (std::exception &err) {
                std::cout << count <<" Caught exception attempting to convert string to integer.\n";
            }
        }
    }
    std::cout << "Processing complete" << "\n";
    return process_vec;
}

bool compareByArrival(const Process a, const Process b)
{
    return a.arrival < b.arrival;
}
bool compareByPriority(const Process a, const Process b)
{
    return a.priority < b.priority;
}

std::vector<Process> sortByArrival (std::vector<Process> process_vec){
    sort(process_vec.begin(),process_vec.end(),compareByArrival);
    return process_vec;
}
std::vector<Process> sortByPriority (std::vector<Process> process_vec){
    sort(process_vec.begin(),process_vec.end(),compareByPriority);
    return process_vec;
}

std::vector<Process> sortProcesses(std::vector<Process> process_vec){
    std::cout << "Sorting..." << "\n";
    process_vec = sortByArrival(process_vec);
    std::vector<Process> newProcess_vec;
    std::vector<Process> sameTick;
    int groupArrivalT;
    for(int i = 0; i < process_vec.size(); i++){
        groupArrivalT = process_vec.front().arrival;
        if(process_vec.at(i).arrival == groupArrivalT){ // check for each process with same arrival
            sameTick.push_back(process_vec.at(i));
        }
        else{ //once not same arrival, sort all the ones of same arrival by prior, add to new vec, erase all of same tick from orig vec
            sameTick = sortByPriority(sameTick);
            newProcess_vec.insert(newProcess_vec.end(),sameTick.begin(), sameTick.end());//adds sorted processes(arrival&prior) to new pvec
            process_vec.erase(process_vec.begin(), process_vec.begin()+i);
            sameTick.clear();
            i=-1;
        }
    }
    newProcess_vec.insert(newProcess_vec.end(), sameTick.begin(), sameTick.end());
    std::cout << "Sorting complete" << "\n";
    return newProcess_vec;
}

//Checks if processes need to be promoted, demoted, or interuptted for IO. Runs once per tick
/*
void checkProcesses(std::vector<std::deque<Process> > Q, int timeQuantum, int ageingInterval, int numQ, int currClockTick) {
    int activeQ = 0;
    // Time quantum is demotion time, doubled every time
    
    //This loop find the first Q with processes in it
    while(Q[activeQ].empty() == true){
        activeQ++;
        //If activeQ > numQ there are currently no processes in any queues
        if(activeQ > numQ){
            return;
        }
    }
    //Iterate though bottom Q and check if any process has aged past the ageing interval, if it has promote it
    for(int i = 0; i < Q[numQ].size(); i++){
        if(Q[numQ][i].age > ageingInterval){
            //TODO Promote Process
        }
        else{
            Q[numQ][i].age++;
        }
    }
    
    //The active process is the process at the front of the first non empty Q
    Process activeProcess = Q[activeQ].front();
    
    //Check if the procecss has been active for longer than the time quantum if it has demote it
    if(currClockTick - activeProcess.arrival > timeQuantum){
        //TODO Demote Process
    }
}
*/