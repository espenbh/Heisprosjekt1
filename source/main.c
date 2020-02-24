#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "timer.h"
#include "controlFiles.h"
#include "variables.h"

Direction DIRECTION = {2, 1};
Floor FLOOR = {0, 0};
int MASTER_MATRIX[3][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
time_t TIMER;
int hasStopped = 0;

static void sigint_handler(int sig)
{
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int main()
{
    int error = hardware_init();
    if (error != 0)
    {
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    initializeElevator();

    signal(SIGINT, sigint_handler);

    printf("kjør");

    while (1)
    {

        if (hardware_read_obstruction_signal() && FLOOR.current > -1 && hardware_read_door_open()){
            StartTimer();
        }
        if (hardware_read_stop_signal()){
            stopFunction();
        }
        if (DIRECTION.current != HARDWARE_MOVEMENT_STOP){
            DIRECTION.last = DIRECTION.current;
        }
        if (checkIfStop()){
            arriveFloor(); 
        }
        if (checkIfLeave()){
            leaveFloor();
        }
        if (hasStopped){
            hasStoppedFunction();
        }
        printf("%d",TimerCount());
        setOrdersAndOrderLights();
    }
    return 0;
}
