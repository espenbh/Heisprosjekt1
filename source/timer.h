/**
* @file
* @brief Functions to keep track of how long certain operations has last. */

#include <stdio.h>
#include <time.h>
#include <stdbool.h>

/**
* @brief Starts the timer from scratch.
* @param[out] TIMER Global timer variable. */
void StartTimer();

/**
* @brief Returns the time since the last call to @f StartTimer().
* @param[in] TIMER Global timer variable.
* @return Returns the number of seconds since the last call to @f StartTimer(). */
int TimerCount();
