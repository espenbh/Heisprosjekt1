#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
<<<<<<< HEAD
#include "timer.h"
=======
#include "controlFiles.h"
>>>>>>> 7484f5374129ca3a806b190e79e62910f026e0a3


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
}

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}


int main(){
    initializeElevator()
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    int currentFloors[] = {0,0,0,0};
    global bool STOP; //Sjekker om heisen er i stop-modus
    global int MASTER_MATRIX[][]={{0, 0, 0}{0, 0, 0}{0, 0, 0}}
    global int FLOOR;
    GLOBAL HardwareMovement DIRECTION;
    GLOBAL time_t TIMER;
    while(1){
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            break;
        }

        if(CheckIfStop(int currentFloors){  //Sjekker om den ANKOMMER en etasje, og om MASTER_MATRIX har høye bit i etasjen
            ArriveFloor();//gjør alt som må gjøres i etasjen.
        }
        if(CheckIfLeave()){
            LeaveFloor();
        }

        setOrdersAndLights()//Masse if-setninger, hvis en knapp blir trykket, settes riktig bit i matrisen til høy, og lyset slås på

        if(hardware_read_obstruction_signal()){
            hardware_command_stop_light(1);
            clear_all_order_lights();
        }
        else{
            hardware_command_stop_light(0);
        }
    }
    return 0;
}
