#include "spacecraft.h"
#include <stdio.h>
int main(){
  dd a = {25,0};
  dd g = {3, 0};
  dd res_d;
  dd b[3];
  dd c[3] = {{7,0}, {9,0}, {11,0}};
  int res = test_function_1(b, c, g);
  for(int i = 0; i < 3; i++){
    printf("res %i: %f       %f\n", i, b[i].x, b[i].y);
  }

  return 0;
}
