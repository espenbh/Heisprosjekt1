#include <stdio.h>
#include <stdlib.h>
#include "controlFiles.h"

int CheckArriveFloor(int currentFloors[]) {
  for(int i=0;i<HARDWARE_NUMBER_OF_FLOORS;i++){
    if(hardware_read_floor_sensor(i) > currentFloors[i]){
      currentFloors[i]=1;
      return 1;
    }
    else if(hardware_read_floor_sensor(i) < currentFloors[i]) {
      currentFloors[i]=0;
      return 0;
    }
  }
return 0;
}
