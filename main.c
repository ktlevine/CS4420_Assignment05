// Assignment 05 - Kyle Levine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberOfProcesses = 0; // first line of file
int type = 0; // what algorithm
int clock = 0; // fake clock

void printFile(FILE *fp) {

    fscanf(fp, "%d", &numberOfProcesses);

    for (int i = 0; i < numberOfProcesses; i++)
    {
        int pid;
        int arrival;
        int burst;

        fscanf(fp, "%d %d %d", &pid, &arrival, &burst);
        printf("PID: %d Arrival: %d Burst: %d\n", pid, arrival, burst);
        
    }
    fclose(fp);
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


    // run the simulation
    while (clock < 500) {
        
        if (type == 1) {
            printf("You have chosen the FCFS Algorithm\n");
            printf("The given processes are:\n");
            printFile(fp);
            
        } else if (type == 2) {
            printf("You have chosen the RR Algorithm\n");
            printf("The given processes are:\n");
            printFile(fp);

        } else if (type == 3) {
            printf("You have chosen the SJF Algorithm\n");
            printf("The given processes are:\n");
            printFile(fp);
        }

  
    clock = 600;

    }


    return 0;
}