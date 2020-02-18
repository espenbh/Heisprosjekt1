#include <stdio.h>

int CheckArriveFloor(Array currentFloors) {
  for(i=0;i<HARDWARE_NUMBER_OF_FLOORS;i++){
    if(hardware_read_floor_sensor(i) > currentFloors[i]){
      currentFloors[i]=1
      return 1;
    }
    elseif(hardware_read_floor_sensor(i)<currentFloors[i])
  }
}
