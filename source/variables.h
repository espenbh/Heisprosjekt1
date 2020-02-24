#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hardware.h"

typedef struct {
	int current;
	int last;
} Floor;
extern Floor FLOOR;

typedef struct {
	HardwareMovement current;
	HardwareMovement last;
} Direction;
extern Direction DIRECTION;
//extern int currentFloors[4];
extern int MASTER_MATRIX[3][4];
extern time_t TIMER;
//extern int prevDirection;
extern int hasStopped;
