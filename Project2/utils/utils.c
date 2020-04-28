#include <stdio.h>
#include "utils.h"


clock_t begin;
int elapsedTime;


void beginTimer() {
  clock_t begin = clock();
  
}

int duration(){

  clock_t end = clock();
  elapsedTime = ((int) (end - begin)) / CLOCKS_PER_SEC;

  int elapsedTime;
}