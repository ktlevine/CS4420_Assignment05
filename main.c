// Assignment 05 - Kyle Levine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberOfProcesses = 0; // first line of file
int type = 0; // what algorithm
int clock = 0; // clock

// struct for each process
typedef struct {
    int pid;
    int arrival;
    int burst;
    int start;
    int end;
    int running;
    int waiting;
    int done;
    int shortest;
    int remaining;
    int round;
} Process;

// called this print file but more so it sets up the process array with the data from the file
void printFile(FILE *fp, Process processes[], int numberOfProcesses) {
    // read for the ammount of processes 
    for (int i = 0; i < numberOfProcesses; i++)
    {
        int pid;
        int arrival;
        int burst;

        fscanf(fp, "%d %d %d", &pid, &arrival, &burst);
        printf("PID: %d Arrival: %d Burst: %d\n", pid, arrival, burst);
        // lay out the starting values for each process
        processes[i].pid = pid;
        processes[i].arrival = arrival;
        processes[i].burst = burst;
        processes[i].start = 0;
        processes[i].end = 0;
        processes[i].waiting = 0;
        processes[i].done = 0;
        processes[i].shortest = 0;
        processes[i].running = 0;
        processes[i].remaining = burst;
        processes[i].round = 0;
    }
}

void sortByArrival(Process processes[], int numberOfProcesses)
{
    // bubble sort by arrival time
    for (int i = 0; i < numberOfProcesses - 1; i++) {
        for (int j = 0; j < numberOfProcesses - i - 1; j++) {
            if (processes[j].arrival > processes[j + 1].arrival) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // RR check
    int quantum = 0;
    if (argc >= 4) {
        quantum = atoi(argv[3]);
    }
    //printf("Quantum: %d\n", quantum);

    // check the command line input
    if (strcmp(argv[2], "FCFS") == 0) {
    type = 1; // FCFS
    }
    else if (strcmp(argv[2], "RR") == 0) {
    type = 2; // RR
    }
    else if (strcmp(argv[2], "SJF") == 0) {
    type = 3; // SJF
    }

    // open the file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening input file");
        return 1;
    }
    fscanf(fp, "%d", &numberOfProcesses);

    Process processes[numberOfProcesses];

    printf("The given processes are:\n");
    printFile(fp, processes, numberOfProcesses);

    sortByArrival(processes, numberOfProcesses);

    printf("\n");
    // run the simulation
    if (type == 1) { // FCFS
        printf("\033[1mYou have chosen the FCFS Algorithm\033[0m\n------------------------------------------------\n");
        printf("\033[3mDebugging info:\033[0m\n");
        clock = 0;
        for (int i = 0; i < numberOfProcesses; i++) {
            processes[i].start = clock; // start time is the current clock time 
            if (clock < processes[i].arrival) {
                processes[i].start = processes[i].arrival;
            }
            clock = processes[i].start; // update the clock to the start time

            clock += processes[i].burst; // increment the clock 
            processes[i].end = clock;
            processes[i].waiting = processes[i].start - processes[i].arrival;
            printf("Process %d: Start: %d End: %d Waiting: %d\n", processes[i].pid, processes[i].start, processes[i].end, processes[i].waiting);
            int running_time = processes[i].end - processes[i].start;
            processes[i].running = running_time;
        }
        // print all the results
        printf("------------------------------------------------\n");
        printf("\033[1mFinal FCFS Results:\033[0m\n");
        printf("%-5s %-12s %-10s %-8s %-12s %-12s\n", "PID", "Arrival", "Start", "End", "Running", "Waiting");
        for (int i = 0; i < numberOfProcesses; i++) {
            printf("%-5d %-12d %-10d %-8d %-12d %-12d\n", processes[i].pid, processes[i].arrival, processes[i].start, processes[i].end, processes[i].running, processes[i].waiting);
        }
        // print the avg waiting time (as a double)
        double avgWaitingTime = 0;
        for (int i = 0; i < numberOfProcesses; i++) {
            avgWaitingTime += processes[i].waiting;
        }
        avgWaitingTime /= numberOfProcesses;
        printf("Average Waiting Time: %.2f\n", avgWaitingTime);
            
    } else if (type == 2 && quantum > 0) { // RR
        printf("\033[1mYou have chosen the RR Algorithm\033[0m\n------------------------------------------------\n");
        printf("\033[3mDebugging info:\033[0m\n");
        printf("Quantum: %d\n", quantum);
        clock = 0;
        int completed = 0;
        while (completed < numberOfProcesses){ // make sure it loops till they are all done
            for (int i = 0; i < numberOfProcesses; i++) {
                if (processes[i].remaining > 0 && processes[i].arrival <= clock) {
                    if (processes[i].round == 0) {
                        processes[i].round++;
                        processes[i].start = clock; // update the start time only on first round
                    }

                    // record the start time of this quantum run
                    int anotherStart = clock;

                    if (processes[i].remaining > quantum) { // if time remaining is bigger than quantum, minus the full quantum amount from remaining time
                        processes[i].remaining -= quantum;
                        clock += quantum;
                    } else { // otherwise, the process should complete within the remaining time
                        clock += processes[i].remaining;
                        processes[i].remaining = 0;
                    }
                    processes[i].end = clock;
                    if (processes[i].remaining == 0) {
                        processes[i].end = clock;
                        // only calculate the waiting time once the process is done
                        processes[i].waiting = processes[i].end - processes[i].arrival - processes[i].burst;
                        completed++;
                    }
                    printf("Process %d: Start: %d End: %d Waiting: %d\n", processes[i].pid, anotherStart, processes[i].end, processes[i].waiting);
                    processes[i].running = processes[i].burst;
                }
            }
        }
        // print all the results
        printf("------------------------------------------------\n");
        printf("\033[1mFinal RR Results:\033[0m\n");
        printf("%-5s %-12s %-10s %-8s %-12s %-12s\n", "PID", "Arrival", "Start", "End", "Running", "Waiting");
        for (int i = 0; i < numberOfProcesses; i++) {
            printf("%-5d %-12d %-10d %-8d %-12d %-12d\n", processes[i].pid, processes[i].arrival, processes[i].start, processes[i].end, processes[i].running, processes[i].waiting);
        }
        // print the avg waiting time (as a double)
        double avgWaitingTime = 0;
        for (int i = 0; i < numberOfProcesses; i++) {
            avgWaitingTime += processes[i].waiting;
        }
        avgWaitingTime /= numberOfProcesses;
        printf("Average Waiting Time: %.2f\n", avgWaitingTime);

    } else if (type == 3) { // SJF
        printf("\033[1mYou have chosen the SJF Algorithm\033[0m\n------------------------------------------------\n");
        printf("\033[3mDebugging info:\033[0m\n");
        clock = 0;
        int completed = 0;
        int shortest = 0;
        int shortestIndex = -1;
        while (completed < numberOfProcesses){
            for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrival <= clock && !processes[i].done) { // check if the process is ready to be run
                if (processes[i].burst < shortest || shortest == 0) {
                    shortest = processes[i].burst;
                    // store the index of the shortest job
                    shortestIndex = i;
                }
            }
        }
        // if nothing is ready to be run, increment the clock and try again
        if (shortestIndex == -1) {
            clock++;
            continue;
        }
        // do the shortest process
        processes[shortestIndex].start = clock;
        processes[shortestIndex].end = processes[shortestIndex].start + processes[shortestIndex].burst;
        processes[shortestIndex].waiting = processes[shortestIndex].start - processes[shortestIndex].arrival;
        processes[shortestIndex].running = processes[shortestIndex].end - processes[shortestIndex].start;
        processes[shortestIndex].done = 1;
        clock = processes[shortestIndex].end;
        // print for debugging
        printf("Process %d: Start: %d End: %d Waiting: %d Running: %d\n", processes[shortestIndex].pid, processes[shortestIndex].start, processes[shortestIndex].end, processes[shortestIndex].waiting, processes[shortestIndex].running);
        completed++;
        shortest = 0;
        shortestIndex = -1;
        }

        // print all the results
        printf("------------------------------------------------\n");
        printf("\033[1mFinal SJF Results:\033[0m\n");
        printf("%-5s %-12s %-10s %-8s %-12s %-12s\n", "PID", "Arrival", "Start", "End", "Running", "Waiting");
        for (int i = 0; i < numberOfProcesses; i++) {
            printf("%-5d %-12d %-10d %-8d %-12d %-12d\n", processes[i].pid, processes[i].arrival, processes[i].start, processes[i].end, processes[i].running, processes[i].waiting); 
        }
        // print the avg waiting time (as a double)
        double avgWaitingTime = 0;
        for (int i = 0; i < numberOfProcesses; i++) {
            avgWaitingTime += processes[i].waiting;
        }
        avgWaitingTime /= numberOfProcesses;
        printf("Average Waiting Time: %.2f\n", avgWaitingTime);
    } else if (argc < 3 || quantum == 0) { // check to ensure the input wont crash the program
        printf("Invalid entry. Please enter in this format: <input_file> <algorithm> [quantum (only if needed)]\n");
    }

    // end program
    fclose(fp);
    return 0;
}