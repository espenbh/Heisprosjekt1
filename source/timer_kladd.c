
#include <time.h>
#include "timer.h"
#include <stdio.h>

time_t TIMER;

void StartTimer() {
  TIMER=time(NULL);
}
int TimerCount(){
  return (time(NULL)-TIMER);
}

/*
int main(){
  printf("Tiden er: %d", time(NULL));
  while(TimerCount()<3){

  }
  printf("Tiden er nå 3 sek. senere; %d", time(NULL));
  return 0;
}*/
