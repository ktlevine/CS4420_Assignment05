// Assignment 05 - Kyle Levine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberOfProcesses = 0; // first line of file
int type = 0; // what algorithm
int clock = 0; // fake clock

// struct for each process
typedef struct {
    int pid;
    int arrival;
    int burst;
    int start;
    int end;
    int waiting;
} Process;

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
    Process processes[numberOfProcesses]; // array for the processes
    // sort by arrival time before doing anything
    sortByArrival(processes, numberOfProcesses);


    // run the simulation
    if (type == 1) { // FCFS
        printf("\033[1mYou have chosen the FCFS Algorithm\033[0m\n------------------------------------------------\n");
        printf("The given processes are:\n");
        printFile(fp, processes, numberOfProcesses);
        printf("------------------------------------------------\n");
        printf("\033[3mDebugging info:\033[0m\n");
        clock = 0;
        for (int i = 0; i < numberOfProcesses; i++) {
            processes[i].start = clock; // start time is the current clock time 
            if (clock < processes[i].arrival) {
                processes[i].start = processes[i].arrival;
            }
            clock = processes[i].start; // update the clock to the start time

            clock += processes[i].burst; // increment the fake clock 
            processes[i].end = clock;
            processes[i].waiting = processes[i].start - processes[i].arrival;
            printf("Process %d: Start: %d End: %d Waiting: %d\n", processes[i].pid, processes[i].start, processes[i].end, processes[i].waiting);
        }
            
    } else if (type == 2) { // RR
        printf("\033[1mYou have chosen the RR Algorithm\033[0m\n------------------------------------------------\n");
        printf("The given processes are:\n");
        printFile(fp, processes, numberOfProcesses);
        printf("------------------------------------------------\n");
        printf("\033[3mDebugging info:\033[0m\n");

    } else if (type == 3) { // SJF
        printf("\033[1mYou have chosen the SJF Algorithm\033[0m\n------------------------------------------------\n");
        printf("The given processes are:\n");
        printFile(fp, processes, numberOfProcesses);
        printf("------------------------------------------------\n");
        printf("\033[3mDebugging info:\033[0m\n");
    }

    // end program
    fclose(fp);
    return 0;
}