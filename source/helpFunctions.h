/**
* @file
* @brief Small functions with intuitive behavior that are used in @c elevatorLogic.c
*  */

#include <stdio.h>
#include <stdlib.h>
#include "variables.h"
#include "hardware.h"


/**
* @brief Turns of all the order lights in all the floor.*/
void resetOrderLights();

/**
* @brief Polls the floor indicator,
* and investigate whether any floor indicator is active.
* @return the floor number of the floor indicator that is active.
* -1 if no indicator is active.*/
int currentFloorIndicator();

/**
* @brief Checks for orders above @p tempFloor.
* @param tempFloor the floor number that are used when searching for orders.
* @return 0 if no orders above @tempFloor. 1 if any orders above @tempFloor. */
int checkOrdersAbove(int tempFloor);

/**
* @brief Checks for orders below @p tempFloor.
* @param tempFloor the floor number that are used when searching for orders.
* @return 0 if no orders below @tempFloor. 1 if any orders below @tempFloor. */
int checkOrdersBelow(int tempFloor);

/**
* @brief Turns of all the order lights,
* and sets every bit in @c MASTER_MATRIX to 0.*/
void deleteOrdersAndLightsOnCurrentFloor();

/**
* @brief Switch on @p DIRECTION.current, drives the elevator*/
void drive();

/**
* @brief Sets direction to the elevator based on a case where upwards motion is prefered.
* @param anyOrdersAbove lets the function know whether the elevator has an order above or not.
* @param anyOrdersbelow lets the function know whether the elevator has an order below or not.
* @c hasStopped this flag gets updated based on the direction the function sets.*/
void prioritizeAbove(int anyOrdersAbove, int anyOrdersBelow);

/**
* @brief Sets direction to the elevator based on a case where downwards motion is prefered.
* @param anyOrdersAbove lets the function know whether the elevator has an order above or not.
* @param anyOrdersbelow lets the function know whether the elevator has an order below or not.
* @c hasStopped this flag gets updated based on the direction the function sets.*/
void prioritizeBelow(int anyOrdersAbove, int anyOrdersBelow);

/**
* @brief Sets every bit in @c MASTER_MATRIX to 0.*/
void resetMasterMatrix();
