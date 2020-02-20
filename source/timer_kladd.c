
#include <time.h>
#include <stdio.h>

volatile time_t TIMER;

volatile void StartTimer() {
  TIMER=clock();
}
volatile int TimerCount(){
  return ((clock()-TIMER)/CLOCKS_PER_SEC);
}

volatile void Stoptimer() {
  TIMER=0;
}

// int main(){
//   StartTimer();
//   printf("Klokka starter, og er %d\n", TIMER);
//   int a=0;
//   while(TimerCount()<3){
//     a=1;
//   }
//   printf("Timeren har gått i tre sekunder, og er %d", TIMER+TimerCount());
//   return 0;
// }
