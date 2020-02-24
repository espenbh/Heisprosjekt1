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

int checkIfLeave()
{
  if (TimerCount() >= 3)
  {
    return 1;
  }
  return 0;
}

void arriveFloor()
{
  hardware_command_movement(HARDWARE_MOVEMENT_STOP); 
  StartTimer();
  hardware_command_door_open(1);
  for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
    hardware_command_order_light(FLOOR.current, i, 0);
  }
}

void leaveFloor(){
  if (FLOOR.current > -1){
    hardware_command_door_open(0);
    for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++)
    {
      if (MASTER_MATRIX[i][FLOOR.current] == 1)
      {
        StartTimer();
        hardware_command_door_open(1);
      }
    }
    deleteOrdersOnCurrentFloor();
    updateDirection(checkOrdersAbove(FLOOR.current), checkOrdersBelow(FLOOR.current));
  }
  switch (DIRECTION.current)
    {
    case HARDWARE_MOVEMENT_UP:
      hardware_command_movement(HARDWARE_MOVEMENT_UP);
      hardware_command_door_open(0);
      FLOOR.current = -1;
      break;
    case HARDWARE_MOVEMENT_DOWN:
      hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
      hardware_command_door_open(0);
      FLOOR.current = -1;
      break;
    case HARDWARE_MOVEMENT_STOP:
      hardware_command_movement(HARDWARE_MOVEMENT_STOP);
      break;
    }
  
}

void setOrdersAndOrderLights()
{
  for (int tempOrder = 0; tempOrder < HARDWARE_NUMBER_OF_ORDER_TYPES; tempOrder++){
    for (int tempFloor = 0; tempFloor < HARDWARE_NUMBER_OF_FLOORS; tempFloor++){
      if (!(tempOrder==0 && tempFloor==3) || !(tempOrder==1 && tempFloor==0)) //Knappene finnes ikke
      {
        if (hardware_read_order(tempFloor, tempOrder)){ 
          MASTER_MATRIX[tempOrder][tempFloor] = 1;
          hardware_command_order_light(tempFloor, tempOrder, 1);
        }
      }
    }
  }
}

void deleteOrdersOnCurrentFloor()
{
  if (FLOOR.current > -1)
  {
    for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++)
    {
      hardware_command_order_light(FLOOR.current, i, 0);
      MASTER_MATRIX[i][FLOOR.current] = 0;
    }
  }
}

void updateDirection(int anyOrdersAbove, int anyOrdersBelow)
{
  if (DIRECTION.current == HARDWARE_MOVEMENT_UP){
    if (anyOrdersAbove){
      DIRECTION.current = HARDWARE_MOVEMENT_UP;
    }
    else if (anyOrdersBelow){
      DIRECTION.current = HARDWARE_MOVEMENT_DOWN;
    }
    else{
      DIRECTION.current = HARDWARE_MOVEMENT_STOP;
    }
  }
  else if (DIRECTION.current == HARDWARE_MOVEMENT_DOWN){
    if (anyOrdersBelow){
      DIRECTION.current = HARDWARE_MOVEMENT_DOWN;
    }
    else if (anyOrdersAbove){
      DIRECTION.current = HARDWARE_MOVEMENT_UP;
    }
    else{
      DIRECTION.current = HARDWARE_MOVEMENT_STOP;
    }
  }
  else if (DIRECTION.current == HARDWARE_MOVEMENT_STOP){
    switch (DIRECTION.last){        //Prioriterer bestillinger i retning heisen kom fra før den stoppet i etasjen
    case HARDWARE_MOVEMENT_UP:
      if (anyOrdersAbove){
        DIRECTION.current = HARDWARE_MOVEMENT_UP;
        hasStopped = 0;
      }
      else if (anyOrdersBelow){
        DIRECTION.current = HARDWARE_MOVEMENT_DOWN;
        hasStopped = 0;
      }
      break;
    case HARDWARE_MOVEMENT_DOWN:
      if (anyOrdersBelow){
        DIRECTION.current = HARDWARE_MOVEMENT_DOWN;
        hasStopped = 0;
      }
      else if (anyOrdersAbove){
        DIRECTION.current = HARDWARE_MOVEMENT_UP;
        hasStopped = 0;
      }
      break;
    case HARDWARE_MOVEMENT_STOP:
      break;
    }
  }
  else{
    DIRECTION.current = HARDWARE_MOVEMENT_STOP;
  }
}

void initializeElevator()
{
  int OnAFloor = 0;
  hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
  DIRECTION.current = HARDWARE_MOVEMENT_DOWN;
  while (!OnAFloor)
  {
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
      if (hardware_read_floor_sensor(i))
      {
        OnAFloor = 1;
        FLOOR.current=i , FLOOR.last = i;
        hardware_command_floor_indicator_on(FLOOR.current);
        StartTimer();
      }
    }
  }
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
  {
    for (int j = 0; j < HARDWARE_NUMBER_OF_ORDER_TYPES; j++)
    {
      hardware_command_order_light(i, j, 0);
    }
  }
}

void stopFunction()
{
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  hasStopped = 1;
  for (int i = 0; i < HARDWARE_NUMBER_OF_ORDER_TYPES; i++){
    for (int j = 0; j < HARDWARE_NUMBER_OF_FLOORS; j++){
      hardware_command_order_light(j, i, 0);
      MASTER_MATRIX[i][j] = 0;
    }
  }
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if (hardware_read_floor_sensor(i)){
      hardware_command_door_open(1);
      FLOOR.current = i;
    }
  }
  hardware_command_stop_light(1);
  while (hardware_read_stop_signal() || TimerCount() < 3)
  {
    if (hardware_read_stop_signal())
    {
      StartTimer();
    }
  }
  DIRECTION.current = HARDWARE_MOVEMENT_STOP;
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
  int lowest = FLOOR.last - DIRECTION.last;
  int highest = lowest + 1;
  updateDirection(checkOrdersAbove(lowest), checkOrdersBelow(highest));
}
