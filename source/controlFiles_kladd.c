#include <stdio.h>
#include <stdlib.h>
#include "controlFiles.h"
#include "variables.h"
#include <time.h>


/*int CheckIfStop(int currentFloors[]) {
  for(int i=0;i<HARDWARE_NUMBER_OF_FLOORS;i++){
    if((hardware_read_floor_sensor(i)) && (FLOOR==-1) && (MASTER_MATRIX[0][i]+MASTER_MATRIX[1][i]+MASTER_MATRIX[2][i])){
      currentFloors[i]=1;
      FLOOR=i;

	for (int i=0; i<4; i++){
		printf("\n");
		for (int j=0; j<3; j++){
			printf("%d",MASTER_MATRIX[j][i]);
	}}
      return 1;
    }
    else if(hardware_read_floor_sensor(i) < currentFloors[i]) {
      currentFloors[i]=0;
      printf("heihei");
      FLOOR=-1;

      return 0;
    }
  }
return 0;
}*/

int prioritizeStop(){
for(int i=0;i<HARDWARE_NUMBER_OF_FLOORS;i++){
    if((hardware_read_floor_sensor(i)) && (FLOOR.current==-1)){
    	hardware_command_floor_indicator_on(i);
    	//printf("i er: %d",i);
    	if(MASTER_MATRIX[DIRECTION.last][i]==1 || MASTER_MATRIX[2][i]){
    		FLOOR.current=i;
    		return 1;
    	}

    	if(DIRECTION.last==0 && checkOrdersAbove(i)){
    		//printf("ordersabove");
    		return 0;
    	}

    	if(DIRECTION.last==1 && checkOrdersBelow(i)){
    		//printf("ordersbelow");
    		return 0;
    	}

    	if(MASTER_MATRIX[!DIRECTION.last][i]){
    		FLOOR.current=i;
    		return 1;
    	}
}}
return 0;
}

int CheckIfLeave() {
  if(TimerCount()>3){
    Stoptimer();
    return 1;
  }
  return 0;
}

void ArriveFloor() {
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);//Stopper bevegelsen
  StartTimer(); //starter pauseklokka
  hardware_command_door_open(1); //setter døråpnelyset til 1

  for(int i=0;i<3;i++){
      hardware_command_order_light(FLOOR.current, i, 0);

  }
  //UpdateMasterMatrixAndDirection(); //sletter alle elementene på etasjen heisen står i, skrur av disse lysene, og sjekker matrisen for retning heisen skal gå i
}

void LeaveFloor() {
	hardware_command_door_open(0);
	for(int i=0;i<3;i++){
	if(MASTER_MATRIX[i][FLOOR.current]==1){
	StartTimer();
	hardware_command_door_open(1);
	}}
  UpdateMasterMatrixAndDirection();
  switch(DIRECTION.current){
    case HARDWARE_MOVEMENT_UP:
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    hardware_command_door_open(0);
    FLOOR.current=-1;
	break;
    case HARDWARE_MOVEMENT_DOWN:
    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    hardware_command_door_open(0);
    FLOOR.current=-1;
	break;
    case HARDWARE_MOVEMENT_STOP: hardware_command_movement(HARDWARE_MOVEMENT_STOP);
	break;
  }
}

void setOrdersAndOrderLights() {
  if(hardware_read_order(0, HARDWARE_ORDER_UP)){
    MASTER_MATRIX[0][0]=1;
    hardware_command_order_light(0, HARDWARE_ORDER_UP, 1);
  }
  if(hardware_read_order(1, HARDWARE_ORDER_UP)){
    MASTER_MATRIX[0][1]=1;
    hardware_command_order_light(1, HARDWARE_ORDER_UP, 1);
  }
  if(hardware_read_order(2, HARDWARE_ORDER_UP)){
    MASTER_MATRIX[0][2]=1;
    hardware_command_order_light(2, HARDWARE_ORDER_UP, 1);
  }
  if(hardware_read_order(1, HARDWARE_ORDER_DOWN)){
    MASTER_MATRIX[1][1]=1;
    hardware_command_order_light(1, HARDWARE_ORDER_DOWN, 1);
  }
  if(hardware_read_order(2, HARDWARE_ORDER_DOWN)){
    MASTER_MATRIX[1][2]=1;
    hardware_command_order_light(2, HARDWARE_ORDER_DOWN, 1);
  }
  if(hardware_read_order(3, HARDWARE_ORDER_DOWN)){
    MASTER_MATRIX[1][3]=1;
    hardware_command_order_light(3, HARDWARE_ORDER_DOWN, 1);
  }
  if(hardware_read_order(0, HARDWARE_ORDER_INSIDE)){
    MASTER_MATRIX[2][0]=1;
    hardware_command_order_light(0, HARDWARE_ORDER_INSIDE, 1);
  }
  if(hardware_read_order(1, HARDWARE_ORDER_INSIDE)){
    MASTER_MATRIX[2][1]=1;
    hardware_command_order_light(1, HARDWARE_ORDER_INSIDE, 1);
  }
  if(hardware_read_order(2, HARDWARE_ORDER_INSIDE)){
    MASTER_MATRIX[2][2]=1;
    hardware_command_order_light(2, HARDWARE_ORDER_INSIDE, 1);
  }
  if(hardware_read_order(3, HARDWARE_ORDER_INSIDE)){
    MASTER_MATRIX[2][3]=1;
    hardware_command_order_light(3, HARDWARE_ORDER_INSIDE, 1);
  }
/*for (int i=0; i<4; i++){
	printf("\n");
	for (int j=0; j<3; j++){
		printf("%d",MASTER_MATRIX[j][i]);
}}printf("\n");*/
}

void UpdateMasterMatrixAndDirection(){
	if(FLOOR.current>-1 || hasStopped==1){
	  for(int i=0;i<3;i++){
	  	hardware_command_order_light(FLOOR.current, i, 0);
		MASTER_MATRIX[i][FLOOR.current]=0;
	  }
  bool anyOrdersBelow=checkOrdersBelow(FLOOR.current), anyOrdersAbove=checkOrdersAbove(FLOOR.current);
  //printf("Above:%d\n",anyOrdersAbove);
  //printf("Below:%d\n",anyOrdersBelow);
  //printf("%d\n",FLOOR);
  if(DIRECTION.current==HARDWARE_MOVEMENT_UP){
    if(anyOrdersAbove){DIRECTION.current=HARDWARE_MOVEMENT_UP;}
    else if(anyOrdersBelow){DIRECTION.current=HARDWARE_MOVEMENT_DOWN;}
    else{DIRECTION.current=HARDWARE_MOVEMENT_STOP;}
  }
  else if(DIRECTION.current==HARDWARE_MOVEMENT_DOWN){
    if(anyOrdersBelow){DIRECTION.current=HARDWARE_MOVEMENT_DOWN;}
    else if(anyOrdersAbove){DIRECTION.current=HARDWARE_MOVEMENT_UP;}
    else {DIRECTION.current=HARDWARE_MOVEMENT_STOP;}
  }
  else if(DIRECTION.current==HARDWARE_MOVEMENT_STOP){
    switch(DIRECTION.last){
    case 0:
    	if(anyOrdersAbove){DIRECTION.current=HARDWARE_MOVEMENT_UP;}
    	else if(anyOrdersBelow){DIRECTION.current=HARDWARE_MOVEMENT_DOWN;}
    case 1:
    	if(anyOrdersBelow){DIRECTION.current=HARDWARE_MOVEMENT_DOWN;}
    	else if(anyOrdersAbove){DIRECTION.current=HARDWARE_MOVEMENT_UP;}
    else{DIRECTION.current=HARDWARE_MOVEMENT_STOP;}}
  }
}
hasStopped=0;}

void initializeElevator() {
  bool OnAFloor = 0;
  hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
  DIRECTION.current=HARDWARE_MOVEMENT_DOWN;
  while(!OnAFloor){
    for(int i=0;i<HARDWARE_NUMBER_OF_FLOORS;i++){
      if(hardware_read_floor_sensor(i)){
        OnAFloor=1;
        FLOOR.current=i;
        hardware_command_floor_indicator_on(FLOOR.current);
        StartTimer();
      }
    }
  }
	hardware_command_movement(HARDWARE_MOVEMENT_STOP);
	for (int i=0; i<4; i++){
		for (int j=0; j<3; j++){
    hardware_command_order_light(i, j, 0);}}
}

void stopFunction(){
  hardware_command_movement(HARDWARE_MOVEMENT_STOP);
  hasStopped=1;
  for(int i=0;i<3;i++){
    for(int j=0;j<HARDWARE_NUMBER_OF_FLOORS;j++){
      hardware_command_order_light(j, i, 0);
      MASTER_MATRIX[i][j]=0;
    }
  }
  if (FLOOR.current!=-1) {
    hardware_command_door_open(1);
  }
  hardware_command_stop_light(1);
  while(hardware_read_stop_signal() || TimerCount()<3){
      if (hardware_read_stop_signal()){
          StartTimer();
      }
  }
  DIRECTION.current=HARDWARE_MOVEMENT_STOP;
  hardware_command_stop_light(0);
}

int checkOrdersAbove(int tempFloor){
	for(int i=tempFloor+1;i<HARDWARE_NUMBER_OF_FLOORS;i++){
		for(int j=0;j<3;j++){
		  if(MASTER_MATRIX[j][i]==1){
		    return 1;
		  }
		}
	  }
	return 0;
}

int checkOrdersBelow(int tempFloor){
	for(int i=tempFloor-1;i>-1;i--){
		for(int j=0;j<3;j++){
		  if(MASTER_MATRIX[j][i]==1){
		    return 1;
		  }
		}
	  }
	return 0;
}
