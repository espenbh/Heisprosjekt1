#include <stdio.h>
#include <stdlib.h>
#include "controlFiles.h"


int CheckIfStop(int currentFloors[]) {
  for(int i=0;i<HARDWARE_NUMBER_OF_FLOORS;i++){
    if(hardware_read_floor_sensor(i) > currentFloors[i] && (MASTER_MATRIX[:][i] & 0b111)){
      currentFloors[i]=1;
      FLOOR=i
      return 1;
    }
    else if(hardware_read_floor_sensor(i) < currentFloors[i]) {
      currentFloors[i]=0;
      FLOOR=-1
      return 0;
    }
  }
return 0;
}

int CheckIfLeave() {
  if(TimerCount()>3){
    StopTimer()
    return 1;
  }
  return 0;
}

void ArriveFloor() {
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);//Stopper bevegelsen
  StartTimer() //starter pauseklokka
  hardware_command_door_open(1); //setter døråpnelyset til 1
  hardware_command_floor_indicator_on(FLOOR);
  UpdateMasterMatrixAndDirection() //sletter alle elementene på etasjen heisen står i, skrur av disse lysene, og sjekker matrisen for retning heisen skal gå i
}

void LeaveFloor() {
  UpdateMasterMatrixAndDirection();
  switch(DIRECTION){
    case HARDWARE_MOVEMENT_UP:
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    hardware_command_door_open(0);
    case HARDWARE_MOVEMENT_DOWN:
    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    hardware_command_door_open(0);
    case HARDWARE_MOVEMENT_STOP: hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  }
}

void setOrdersAndOrderLights() {
  if(hardware_read_order(0, HARDWARE_ORDER_UP)){
    MASTER_MATRIX[0][0]=1
    hardware_command_order_light(0, HARDWARE_ORDER_UP, 1)
  }
  if(hardware_read_order(1, HARDWARE_ORDER_UP)){
    MASTER_MATRIX[0][1]=1
    hardware_command_order_light(1, HARDWARE_ORDER_UP, 1)
  }
  if(hardware_read_order(2, HARDWARE_ORDER_UP)){
    MASTER_MATRIX[0][2]=1
    hardware_command_order_light(2, HARDWARE_ORDER_UP, 1)
  }
  if(hardware_read_order(1, HARDWARE_ORDER_DOWN)){
    MASTER_MATRIX[1][1]=1
    hardware_command_order_light(1, HARDWARE_ORDER_DOWN, 1)
  }
  if(hardware_read_order(2, HARDWARE_ORDER_DOWN)){
    MASTER_MATRIX[1][2]=1
    hardware_command_order_light(2, HARDWARE_ORDER_DOWN, 1)
  }
  if(hardware_read_order(3, HARDWARE_ORDER_DOWN)){
    MASTER_MATRIX[1][3]=1
    hardware_command_order_light(3, HARDWARE_ORDER_DOWN, 1)
  }
  if(hardware_read_order(0, HARDWARE_ORDER_INSIDE){
    MASTER_MATRIX[2][0]=1
    hardware_command_order_light(0, HARDWARE_ORDER_INSIDE, 1)
  }
  if(hardware_read_order(1, HARDWARE_ORDER_INSIDE){
    MASTER_MATRIX[2][1]=1
    hardware_command_order_light(1, HARDWARE_ORDER_INSIDE, 1)
  }
  if(hardware_read_order(2, HARDWARE_ORDER_INSIDE){
    MASTER_MATRIX[2][2]=1
    hardware_command_order_light(2, HARDWARE_ORDER_INSIDE, 1)
  }
  if(hardware_read_order(3, HARDWARE_ORDER_INSIDE){
    MASTER_MATRIX[2][3]=1
    hardware_command_order_light(3, HARDWARE_ORDER_INSIDE, 1)
  }
}

void UpdateMasterMatrixAndDirection(){
  for(int i=0;i<3;i++){
    hardware_command_order_light(FLOOR, i, 0);
    MASTER_MATRIX[i][FLOOR]=0;
  }
  bool anyOrdersBelow=0, anyOrdersAbove=0;
  for(int i=FLOOR+1;i<HARDWARE_NUMBER_OF_FLOORS;i++){
    for(int j=0;j<3;j++){
      if(MASTER_MATRIX[j][i]==1){
        anyOrdersAbove=1
      }
    }
  }
  for(int i=FLOOR-1;i>-1;i--){
    for(int j=0;j<3;j++){
      if(MASTER_MATRIX[j][i]==1){
        anyOrdersBelow=1
      }
    }
  }
  if(DIRECTION==HARDWARE_MOVEMENT_UP){
    if(anyOrdersAbove){DIRECTION=HARDWARE_MOVEMENT_UP;}
    else if(anyOrdersBelow){DIRECTION=HARDWARE_MOVEMENT_DOWN;}
    else{DIRECTION=HARDWARE_MOVEMENT_STOP;}
  }
  else if(DIRECTION==HARDWARE_MOVEMENT_DOWN){
    if(anyOrdersBelow){DIRECTION=HARDWARE_MOVEMENT_DOWN;}
    else if(anyOrdersAbove){DIRECTION=HARDWARE_MOVEMENT_UP;}
    else {DIRECTION=HARDWARE_MOVEMENT_STOP;}
  }
  else if(DIRECTION==HARDWARE_MOVEMENT_STOP){
    if(anyOrdersAbove){DIRECTION=HARDWARE_MOVEMENT_UP;}
    else if(anyOrdersBelow){DIRECTION=HARDWARE_MOVEMENT_DOWN;}
    else{DIRECTION=HARDWARE_MOVEMENT_STOP;}
  }
}

void initializeElevator() {
  bool OnAFloor = 0;
  hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
  DIRECTION=HARDWARE_MOVEMENT_DOWN
  while(!OnAFloor){
    for(int i=0;i<HARDWARE_NUMBER_OF_FLOORS;i++){
      if(hardware_read_floor_sensor(i)){
        OnAFloor=1;
        FLOOR=i;
        StartTimer();
      }
    }
  }
}
     

