#include <stdio.h>
#include <stdlib.h>
#include "variables.h"
#include "hardware.h"

void resetOrderLights();
int currentFloorIndicator();
int checkOrdersAbove(int tempFloor);
int checkOrdersBelow(int tempFloor);
void deleteOrdersAndLightsOnCurrentFloor();
void prioritizeAbove(int anyOrdersAbove, int anyOrdersBelow);
void prioritizeBelow(int anyOrdersAbove, int anyOrdersBelow);
void resetMasterMatrix();
