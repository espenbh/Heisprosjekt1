/**
* @file
* @brief Functions to keep track of how long certain operations has last. */

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#define TIMER_WAIT_CONSTANT 3

/**
* @brief Starts the timer from scratch.
* @param[out] TIMER Global timer variable. */
void startTimer();

/**
* @brief Returns the time since the last call to @f StartTimer().
* @param[in] TIMER Global timer variable.
* @return Returns the number of seconds since the last call to @f StartTimer(). */
int timerCount();

