#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include <stdbool.h>
#include "timer.h"
#include "variables.h"

int CheckIfStop(int currentFloors[]);
int prioritizeStop();
int CheckIfLeave();
void ArriveFloor();
void LeaveFloor();
void setOrdersAndOrderLights();
void UpdateMasterMatrixAndDirection();
void initializeElevator();
void stopFunction();
int checkOrdersAbove();
int checkOrdersBelow();
