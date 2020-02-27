/**
 * @file
 * @brief Implementation file for helpFunctions
 */

#include "helpFunctions.h"

void resetOrderLights() {
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++){
      hardware_command_order_light(i, j, 0);
    }
  }
}

int currentFloorIndicator() {
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if(hardware_read_floor_sensor(i) == 1){
      return i;
    }
  }
    return -1;
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

void deleteOrdersAndLightsOnCurrentFloor(){
  if (FLOOR.current > -1){
    for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
      hardware_command_order_light(FLOOR.current, i, 0);
      MASTER_MATRIX[i][FLOOR.current] = 0;
    }
  }
}


void drive(){
  switch (DIRECTION.current)
    {
    case HARDWARE_MOVEMENT_UP:
      hardware_command_movement(HARDWARE_MOVEMENT_UP);
      hardware_command_door_open(0);
      FLOOR.current = -1;
      hasJustLeft = 1;

      break;
    case HARDWARE_MOVEMENT_DOWN:
      hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
      hardware_command_door_open(0);
      FLOOR.current = -1;
      hasJustLeft = 1;

      break;
    case HARDWARE_MOVEMENT_STOP:
      hardware_command_movement(HARDWARE_MOVEMENT_STOP);
      hardware_command_door_open(0);
      break;
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

void resetMasterMatrix(){
  for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
    for (int j = 0; j < HARDWARE_NUMBER_OF_FLOORS; j++){
      MASTER_MATRIX[i][j] = 0;
    }
  }
}
