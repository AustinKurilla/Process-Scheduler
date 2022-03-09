#ifndef LAB2_PROCESS_H
#define LAB2_PROCESS_H

#include <fstream>
#include <bits/stdc++.h> // Used for sorting vectors.
#include "estring.h" // Comes with vector, string, iostream classes. Funcs : split, print_str_vec

class Process {
    public:
        // Attributes
        int pid;
        int burst;
        int arrival; // First sorted
        int priority; // Tie on arrival, sort by priority
        int deadline;
        int io;
        int timeRemaining;
        int quanRemaining = -1;
        int age;
        int currQ;
        int slack;

        // Constructor
        Process(int new_pid, int new_burst, int new_arrival, int new_priority, int new_deadline, int new_io);

        // Methods
        int getAge(int ticks_elapsed);
        void printProcess();
};

// Functions
std::vector<Process> generateProcesses (std::string file_name, int max_line, double * total_burst, int io_enabled);
bool compareByArrival(const Process a, const Process b);
bool compareByPriority(const Process a, const Process b);
std::vector<Process> sortByArrival (std::vector<Process> process_vec);
std::vector<Process> sortByPriority (std::vector<Process> process_vec);
std::vector<Process> sortProcesses (std::vector<Process> process_vec);
//void checkProcesses(std::vector<std::deque<Process> > Q, int timeQuantum, int ageingInterval, int numQ, int currClockTick);

#endif