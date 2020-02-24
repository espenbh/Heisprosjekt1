#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "timer.h"
#include "variables.h"

int checkIfStop();
int checkIfLeave();
void arriveFloor();
void leaveFloor();
void setOrdersAndOrderLights();
void deleteOrdersOnCurrentFloor();
void initializeElevator();
void stopFunction();
int checkOrdersAbove();
int checkOrdersBelow();
void hasStoppedFunction();
void updateDirection();
