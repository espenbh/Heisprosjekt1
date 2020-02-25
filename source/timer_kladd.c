
#include <time.h>
#include "timer.h"
#include <stdio.h>

time_t TIMER;

void startTimer() {
  TIMER=time(NULL);
}
int timerCount(){
  return (time(NULL)-TIMER);
}

