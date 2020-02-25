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
void deleteOrdersAndLightsOnCurrentFloor();
void initializeElevator();
void stopFunction();
int checkOrdersAbove();
int checkOrdersBelow();
void hasStoppedFunction();
void updateDirection();
void checkForOrdersOnCurrentFloor();
int checkForObstruction();
void prioritizeAbove(int anyOrdersAbove, int anyOrdersBelow);
void prioritizeBelow(int anyOrdersAbove, int anyOrdersBelow);
int currentFloorIndicator();
void resetOrderLights();
void resetMasterMatrix();
