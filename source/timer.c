#include <time.h>
#include <stdio.h>

void StopAtFloor(){
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  time_t start=clock();
  hardware_command_door_open(1);
  wait=1;
  while(wait){
    if(clock()>start+3*CLOCKS_PER_SEC){
      wait=0;
    }
  }
  hardware_command_movement(HARDWARE_MOVEMENT_UP);
  hardware_command_door_open(0);
}
