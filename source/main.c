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
int hasLeft=0;
int checkForObstruction(){ return hardware_read_obstruction_signal() && FLOOR.current > -1 && (hardware_read_door_open() || hasStopped==1);}

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

    while (1)
    {
        if (checkIfStop()){
            arriveFloor();
        }

        if (checkForObstruction()){
            StartTimer();
            hardware_command_door_open(1);
        }
        if (hardware_read_stop_signal()){
            stopFunction();
        }
        leaveFloor();
        if (hasStopped){
            hasStoppedFunction();
        }
        checkForOrdersOnCurrentFloor();

        if (DIRECTION.current != HARDWARE_MOVEMENT_STOP){DIRECTION.last = DIRECTION.current;}
        if (!hardware_read_floor_sensor(FLOOR.last)){hasLeft=0;}

        setOrdersAndOrderLights();
    }
  return 0;
}
