#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() 
{
  char *x = (char *) malloc(100);
  char *y = "hello world";
  char *z = "ending";
  clock_t begin = clock();

  snprintf(x, 100, "%s%s", y, z);
  snprintf(x + 20, 100, "%s%s", x, x);

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("%s %f\n", x, time_spent);
}

