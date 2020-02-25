#include "controlFiles.h"

void initializeElevator(){
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


int checkIfStop(){
  tempFloor=currentFloorIndicator();
  if (tempFloor>-1 && FLOOR.current == -1){
    hardware_command_floor_indicator_on(tempFloor);
    FLOOR.last=tempFloor;

    if (MASTER_MATRIX[DIRECTION.last][tempFloor] == 1 || MASTER_MATRIX[2][tempFloor]){
      FLOOR.current = tempFloor;
      return 1;
    }
    else if (DIRECTION.last == HARDWARE_MOVEMENT_UP && checkOrdersAbove(tempFloor)){return 0;}
    else if (DIRECTION.last == HARDWARE_MOVEMENT_DOWN && checkOrdersBelow(tempFloor)){return 0;}
    else if (MASTER_MATRIX[!DIRECTION.last][tempFloor]){
      FLOOR.current = tempFloor;
      return 1;
    }
  }
  return 0;
}

void arriveFloor(){
  if(!hasLeft){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    startTimer();
    hardware_command_door_open(1);
    deleteOrdersAndLightsOnCurrentFloor();
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
      hardware_command_door_open(0); //trenger vi disse
      FLOOR.current = -1;
      hasLeft = 1;
      break;
    case HARDWARE_MOVEMENT_STOP:
      hardware_command_movement(HARDWARE_MOVEMENT_STOP);
      break;
    }
  }
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
