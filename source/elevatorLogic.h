/**
 * @file
 * @brief Functions that decide what the elevetaor will do at a given time and state. */

#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "timer.h"
#include "variables.h"
#include "helpFunctions.h"

/**
 * @brief Resets lights, and drives down until the elevator reaches a floor sensor. Saves this floor as @p FLOOR.current and @p FLOOR.last*/
void initializeElevator();

/**
 * @brief When the elevator is moving and reaches a floor, the floor indicator lights up, @p FLOOR.last is set, and it decides whether or not to stop at this floor.
 * @return 0 if the elevator is to drive past the floor. 1 to stop.*/
int checkIfStop();

/**
 * @brief As long as the @p hasJustLeft flag is not set, this function stops the elevator, starts the timer, open the door, and deletes the order to the current floor.*/
void arriveFloor();

/**
 * @brief When the timer reaches 3 seconds and the elevator is not moving, the door is closed and new orders are looked for using @p updateDirection().*/
void leaveFloor();

/**
 * @brief Prioritizes what direction the elevator should go, given @p DIRECTION.current. If the elevator is not moving, @p DIRECTION.last is used to determine which orders to prioritize.
 * @param[in] anyOrdersAbove lets the function know whether the elevator has an order above or not.
 * @param[in] anyOrdersbelow lets the function know whether the elevator has an order below or not.*/
void updateDirection();

/**
 * @brief Polls all order buttons, and sets @p MASTER_MATRIX and the corresponding light if any buttons are pushed.*/
void setOrdersAndOrderLights();

/**
 * @brief Stops the elevator, sets @p hasStopped to 1 and flushes all orders. If the elevator is at a floor, the door is opened. Sets the stoplight.*/
void stopFunction();

/**
 * @brief If the elevator has stopped between two floors, this function determines these two floors and calls @p updateDirection() with the result as the two parameters.*/
void hasStoppedFunction();

/**
 * @brief If the elevator is ordered at its @p FLOOR.current, @p arriveFloor() is called.*/
void checkForOrdersOnCurrentFloor();

/** @brief Determines wheter the obstruction switch should have any effect.
 * @return 1 if the obstruction switch should affect the elevator, 0 if not.*/
int checkForObstruction();
