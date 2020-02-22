#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "timer.h"
#include "controlFiles.h"
#include <stdbool.h>
#include "variables.h"

int FLOOR;
HardwareMovement DIRECTION;
int currentFloors[4] = {0,0,0,0};
int MASTER_MATRIX[3][4] = {{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}};
time_t TIMER;
int prevDirection=1;
int hasStopped = 0;



/*
static void clear_all_order_lights(){
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN
    };

    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}*/

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}


int main(){
	int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    initializeElevator();
    

    signal(SIGINT, sigint_handler);

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");
	
	
    while(1){
	
        if(hardware_read_obstruction_signal() && FLOOR>-1 && hardware_read_door_open()){
            StartTimer();
        }
        if(hardware_read_stop_signal()){
            stopFunction();
        }
        if(DIRECTION!=HARDWARE_MOVEMENT_STOP){
        prevDirection = DIRECTION-1; //Gir 0 for oppover og 1 for nedover
    	if(prevDirection<0){prevDirection+=1;}
		}

        if(prioritizeStop()){  //Sjekker om den ANKOMMER en etasje, og om MASTER_MATRIX har høye bit i etasjen
            ArriveFloor();//gjør alt som må gjøres i etasjen.
        }
        if(CheckIfLeave()){
            LeaveFloor();
        }

        setOrdersAndOrderLights();//Masse if-setninger, hvis en knapp blir trykket, settes riktig bit i matrisen til høy, og lyset slås på

        
    }
    return 0;
}
