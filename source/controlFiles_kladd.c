#include <stdio.h>
#include <stdlib.h>
#include "controlFiles.h"
#include "variables.h"
#include <time.h>

int checkIfStop()
{
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
  {
    if ((hardware_read_floor_sensor(i)) && (FLOOR.current == -1))
    {
      hardware_command_floor_indicator_on(i);
      FLOOR.last=i;
      if (MASTER_MATRIX[DIRECTION.last][i] == 1 || MASTER_MATRIX[2][i])
      {
        FLOOR.current = i;
        return 1;
      }

      if (DIRECTION.last == HARDWARE_MOVEMENT_UP && checkOrdersAbove(i))
      {
        return 0;
      }

      if (DIRECTION.last == HARDWARE_MOVEMENT_DOWN && checkOrdersBelow(i))
      {
        return 0;
      }

      if (MASTER_MATRIX[!DIRECTION.last][i])
      {
        FLOOR.current = i;
        return 1;
      }
    }
  }
  return 0;
}

void arriveFloor()
{
  if(hasLeft!=1){
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  startTimer();
  hardware_command_door_open(1);
  deleteOrdersAndLightsOnCurrentFloor();
  for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
    hardware_command_order_light(FLOOR.current, i, 0);
  }
}
}

void leaveFloor(){
  if (timerCount() >= 3){
    if (FLOOR.current > -1){
      hardware_command_door_open(0);
      updateDirection(checkOrdersAbove(FLOOR.current), checkOrdersBelow(FLOOR.current));
    }
  switch (DIRECTION.current)
    {
    case HARDWARE_MOVEMENT_UP:
      hardware_command_movement(HARDWARE_MOVEMENT_UP);
      hardware_command_door_open(0);
      FLOOR.current = -1;
      hasLeft = 1;
      break;
    case HARDWARE_MOVEMENT_DOWN:
      hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
      hardware_command_door_open(0);
      FLOOR.current = -1;
      hasLeft = 1;
      break;
    case HARDWARE_MOVEMENT_STOP:
      hardware_command_movement(HARDWARE_MOVEMENT_STOP);
      break;
    }
  }
}

void setOrdersAndOrderLights(){
  for (int tempOrder = 0; tempOrder < HARDWARE_NUMBER_OF_ORDER_TYPES; tempOrder++){
    for (int tempFloor = 0; tempFloor < HARDWARE_NUMBER_OF_FLOORS; tempFloor++){
      if (!(tempOrder==0 && tempFloor==3) || !(tempOrder==1 && tempFloor==0)){ //Knappene finnes ikke
        if (hardware_read_order(tempFloor, tempOrder)){
          MASTER_MATRIX[tempOrder][tempFloor] = 1;
          hardware_command_order_light(tempFloor, tempOrder, 1);
        }
      }
    }
  }
}

void deleteOrdersAndLightsOnCurrentFloor(){
  if (FLOOR.current > -1){
    for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
      hardware_command_order_light(FLOOR.current, i, 0);
      MASTER_MATRIX[i][FLOOR.current] = 0;
    }
  }
}

void prioritizeAbove(int anyOrdersAbove, int anyOrdersBelow){
  if (anyOrdersAbove){DIRECTION.current = HARDWARE_MOVEMENT_UP;hasStopped=0;}
  else if (anyOrdersBelow){DIRECTION.current = HARDWARE_MOVEMENT_DOWN;hasStopped=0;}
  else{DIRECTION.current = HARDWARE_MOVEMENT_STOP;}
}

void prioritizeBelow(int anyOrdersAbove, int anyOrdersBelow){
  if (anyOrdersBelow){DIRECTION.current = HARDWARE_MOVEMENT_DOWN;hasStopped=0;}
  else if (anyOrdersAbove){DIRECTION.current = HARDWARE_MOVEMENT_UP;hasStopped=0;}
  else{DIRECTION.current = HARDWARE_MOVEMENT_STOP;}
}


void updateDirection(int anyOrdersAbove, int anyOrdersBelow)
{
  if (DIRECTION.current == HARDWARE_MOVEMENT_UP){
    prioritizeAbove(anyOrdersAbove, anyOrdersBelow);
  }
  else if (DIRECTION.current == HARDWARE_MOVEMENT_DOWN){
    prioritizeBelow(anyOrdersAbove, anyOrdersBelow);
  }
  else if (DIRECTION.current == HARDWARE_MOVEMENT_STOP){
    switch (DIRECTION.last){               //Prioriterer bestillinger i retning heisen kom fra før den stoppet i etasjen
    case HARDWARE_MOVEMENT_UP:
      prioritizeAbove(anyOrdersAbove, anyOrdersBelow);
      break;
    case HARDWARE_MOVEMENT_DOWN:
      prioritizeBelow(anyOrdersAbove, anyOrdersBelow);
      break;
    case HARDWARE_MOVEMENT_STOP:
      break;
    }
  }
}

void initializeElevator()
{
  int OnAFloor = 0;
  resetOrderLights();
  hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
  while (!OnAFloor){
      if (currentFloorIndicator()>-1){
        OnAFloor = 1;
        FLOOR.current=currentFloorIndicator();
        FLOOR.last = currentFloorIndicator();
        hardware_command_floor_indicator_on(FLOOR.current);
        startTimer();
      }
  }
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void stopFunction(){
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  DIRECTION.current = HARDWARE_MOVEMENT_STOP;
  hasStopped = 1;
  hasLeft = 0;

  resetMasterMatrix();
  resetOrderLights();

  if (currentFloorIndicator()>-1){
    hardware_command_door_open(1);
    FLOOR.current = currentFloorIndicator();
  }

  hardware_command_stop_light(1);

  while (hardware_read_stop_signal() || timerCount() < TIMER_WAIT_CONSTANT){
    if (hardware_read_stop_signal()){
      startTimer();
    }
  }

  hardware_command_stop_light(0);
  hardware_command_door_open(0);
}

int checkOrdersAbove(int tempFloor){
  for (int i = tempFloor + 1; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
      if (MASTER_MATRIX[j][i] == 1){
        return 1;
      }
    }
  }
  return 0;
}

int checkOrdersBelow(int tempFloor){
  for (int i = tempFloor - 1; i > -1; i--){
    for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
      if (MASTER_MATRIX[j][i] == 1){
        return 1;
      }
    }
  }
  return 0;
}

void hasStoppedFunction(){
  int floorBelow = FLOOR.last - DIRECTION.last;
  int floorAbove = floorBelow + 1;
  updateDirection(checkOrdersAbove(floorBelow), checkOrdersBelow(floorAbove));
}

void checkForOrdersOnCurrentFloor(){
  for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
    if (MASTER_MATRIX[i][FLOOR.current] == 1){
     arriveFloor();
    }
  }
}

int checkForObstruction(){
  return hardware_read_obstruction_signal() && FLOOR.current > -1 && (hardware_read_door_open() || hasStopped==1);
}

int currentFloorIndicator() {
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if(hardware_read_floor_sensor(i) == 1){
      return i;
    }
  }
    return -1;
}

void resetOrderLights() {
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
      hardware_command_order_light(i, j, 0);
    }
  }
}

void resetMasterMatrix(){
  for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
    for (int j = 0; j < HARDWARE_NUMBER_OF_FLOORS; j++){
      MASTER_MATRIX[i][j] = 0;
    }
  }
}
