#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "timer.h"
#include "variables.h"

void initializeElevator();
int checkIfStop();
void arriveFloor();
void leaveFloor();
void updateDirection();
void setOrdersAndOrderLights();
void stopFunction();
void hasStoppedFunction();
void checkForOrdersOnCurrentFloor();
int checkForObstruction();
