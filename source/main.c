#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "timer.h"
#include "controlFiles.h"
#include <stdbool.h>
#include "variables.h"

Direction DIRECTION;
Floor FLOOR;
int MASTER_MATRIX[3][4] = {{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}};
time_t TIMER;
int hasStopped = 0;

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
	signal(SIGINT, sigint_handler);

  initializeElevator();

  while(1){
    if(hardware_read_obstruction_signal() && FLOOR.current>-1 && hardware_read_door_open()){
      StartTimer();
    }
    if(hardware_read_stop_signal()){
    	stopFunction();
    }
    if(DIRECTION.current!=HARDWARE_MOVEMENT_STOP){
        DIRECTION.last = DIRECTION.current; //Gir 0 for oppover og 1 for nedover
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
