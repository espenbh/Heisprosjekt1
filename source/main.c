/**
 * @file
 * @brief The main file of the application
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "timer.h"
#include "elevatorLogic.h"
#include "variables.h"

Direction DIRECTION = {1, 1};
Floor FLOOR = {0, 0};
int MASTER_MATRIX[3][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
time_t TIMER;
int hasStopped = 0;
int hasJustLeft = 0;

static void sigint_handler(int sig){
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

    signal(SIGINT, sigint_handler);

    initializeElevator();

    while (1)
    {
        if (checkIfStop()){arriveFloor();}

        leaveFloor();

        setOrdersAndOrderLights();

        if (hardware_read_stop_signal()){stopFunction();}

        if (hasStopped){hasStoppedFunction();}

        if (checkForObstruction()){
            startTimer();
            hardware_command_door_open(1);
        }

        printf("Floor last: %d\n",FLOOR.last);
        printf("Floor current: %d\n",FLOOR.current);
        printf("DIrection last: %d\n",DIRECTION.last);
        printf("DIrection current: %d\n",DIRECTION.current);
        printf("%d\n",hasJustLeft);

        checkForOrdersOnCurrentFloor();

        if (DIRECTION.current != HARDWARE_MOVEMENT_STOP&& currentFloorIndicator()>-1){DIRECTION.last = DIRECTION.current;}
        if (!hardware_read_floor_sensor(FLOOR.last)){hasJustLeft=0;}

    }
  return 0;
}
