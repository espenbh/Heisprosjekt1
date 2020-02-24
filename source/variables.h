/**
* @file
* @brief Define global variables. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hardware.h"

/**
* @brief Define a struct to keep track of current and previous floor.
* @param current Holds the current floor, -1 if moving.
* @param last Holds the last floor the elevator were at.
*/
typedef struct {
	int current;
	int last;
} Floor;
extern Floor FLOOR;

/**
* @brief Define a struct to keep track of current and previous direction.
* @param current Holds the current direction.
* @param last Holds the last direction the elevator had when it was moving.
*/
typedef struct {
	HardwareMovement current;
	HardwareMovement last;
} Direction;
extern Direction DIRECTION;

/**
* @brief A matrix that keeps track of orders. */
extern int MASTER_MATRIX[3][4];
extern time_t TIMER;

/**
* @breif A flag thats high when the last order
* the elevator received was stop.*/
extern int hasStopped;
