#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hardware.h"
#ifndef structinit
#define structinit
typedef struct {
	int current;
	int last;
} Floor;
extern Floor FLOOR;

typedef struct {
	HardwareMovement current;
	HardwareMovement last;
} Direction;
#endif
extern Direction DIRECTION;
//extern int currentFloors[4];
extern int MASTER_MATRIX[3][4];
extern time_t TIMER;
//extern int prevDirection;
extern int hasStopped;
