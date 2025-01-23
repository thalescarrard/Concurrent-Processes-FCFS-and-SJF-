/*
 * COP4610 - ASSIGNMENT 2
 * Members:
 *     - Anthony L Carvalho 6365160
 *     - Thales Moraes 6332398
 * 
 * Description:
 * This assignment implements two scheduling algorithms: First-Come, First-Served 
 * (FCFS) and Shortest Job First (SJF). The program reads a set of processes 
 * with arrival times and burst times from an input text file, simulates the 
 * scheduling of these processes, and calculates the waiting time and turnaround time
 * for each process.
 *
 * In the FCFS algorithm, processes are scheduled in the order of their arrival times.
 * The SJF algorithm selects the process with the shortest burst time for execution 
 * from the set of processes that have arrived. Both algorithms calculate and display
 * the average waiting time and the average turnaround time after the scheduling.
 *
 * The program reads an input file, simulates the chosen scheduling algorithm, and
 * outputs the order in which processes were executed, along with the calculated 
 * average waiting and turnaround times.
 *
 * Usage:
 * The program takes two command-line arguments:
 *     1. The name of the input file containing process information.
 *     2. The scheduling algorithm to use ("FCFS" or "SJF").
 * Example:
 *     ./scheduler input.txt FCFS
 * 
 * Input File Format:
 * The input file should contain the number of processes on the first line,
 * followed by the arrival time and burst time for each process on subsequent lines.
 * Example:
 *     3
 *     0 5
 *     2 3
 *     4 1
 * In this example:
 *     - Process 1 arrives at time 0 and has a burst time of 5 units.
 *     - Process 2 arrives at time 2 and has a burst time of 3 units.
 *     - Process 3 arrives at time 4 and has a burst time of 1 unit.
 *
 * Output:
 * The program outputs the order in which processes are executed, 
 * the average waiting time, and the average turnaround time.
 * 
 * Compilation:
 *     gcc -o scheduler main.c scheduling_algorithms.c
 *
 * Important Notes:
 * - The SJF algorithm does not preempt processes once they are running.
 * - Both algorithms assume that all processes are available at time 0,
 *   meaning no process has an arrival time after the simulation has started.
 */


#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int processID;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    int isCompleted;
} Process;

// Function to read input from file
void readFile(const char* filename, int* numProcesses, Process processes[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        exit(1);
    }

    fscanf(file, "%d", numProcesses);
    for (int i = 0; i < *numProcesses; i++) {
        processes[i].processID = i + 1;
        fscanf(file, "%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
        processes[i].isCompleted = 0;
    }
    fclose(file);
}

// Function to calculate FCFS scheduling
void fcfs(Process processes[], int numProcesses) {
    int currentTime = 0;

    for (int i = 0; i < numProcesses; i++) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        currentTime += processes[i].burstTime;
    }
}

// Function to calculate SJF scheduling
void sjf(Process processes[], int numProcesses) {
    int currentTime = 0, completed = 0;
    while (completed != numProcesses) {
        int shortestJob = -1;
        for (int i = 0; i < numProcesses; i++) {
            if (!processes[i].isCompleted && processes[i].arrivalTime <= currentTime) {
                if (shortestJob == -1 || processes[i].burstTime < processes[shortestJob].burstTime) {
                    shortestJob = i;
                }
            }
        }

		//  SJF algorithm logic, ensuring processes with the shortest burst time are prioritized
        if (shortestJob == -1) {
            currentTime++;
        } else {
            processes[shortestJob].waitingTime = currentTime - processes[shortestJob].arrivalTime;
            processes[shortestJob].turnaroundTime = processes[shortestJob].waitingTime + processes[shortestJob].burstTime;
            currentTime += processes[shortestJob].burstTime;
            processes[shortestJob].isCompleted = 1;
            completed++;
        }
    }
}

// Function to calculate average waiting time
double calculateAverageWaitingTime(Process processes[], int numProcesses) {
    double totalWaitTime = 0;
    for (int i = 0; i < numProcesses; i++) {
        totalWaitTime += processes[i].waitingTime;
    }
    return totalWaitTime / numProcesses;
}

// Function to calculate average turnaround time
double calculateAverageTurnaroundTime(Process processes[], int numProcesses) {
    double totalTurnaroundTime = 0;
    for (int i = 0; i < numProcesses; i++) {
        totalTurnaroundTime += processes[i].turnaroundTime;
    }
    return totalTurnaroundTime / numProcesses;
}

// Function to display results
void displayResults(Process processes[], int numProcesses) {
    printf("Order of Execution: ");
    for (int i = 0; i < numProcesses; i++) {
        printf("P%d", processes[i].processID);
        if (i < numProcesses - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
    printf("Average Waiting Time: %.2f\n", calculateAverageWaitingTime(processes, numProcesses));
    printf("Average Turnaround Time: %.2f\n", calculateAverageTurnaroundTime(processes, numProcesses));
}

//  Handled result display logic
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <FCFS/SJF>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *algorithm = argv[2];
    
    int numProcesses;
    Process processes[100];  // Assuming max 100 processes

    // Read the input from the file
    readFile(filename, &numProcesses, processes);

    // Choose the scheduling algorithm
    if (strcmp(algorithm, "FCFS") == 0) {
        fcfs(processes, numProcesses);
    } else if (strcmp(algorithm, "SJF") == 0) {
        sjf(processes, numProcesses);
    } else {
        printf("Unknown scheduling algorithm: %s\n", algorithm);
        return 1;
    }

    // Display the results
    displayResults(processes, numProcesses);

    return 0;
}
// Added main function logic, ensuring correct command-line argument handling and program flow