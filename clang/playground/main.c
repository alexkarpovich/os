#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() 
{
  char *x = (char *) malloc(100);
  char *c = x;
  char *y = "";
  char *z = "ending";
  clock_t begin = clock();

  c += snprintf(c, 100, "%s%s", c, y);
  c += snprintf(c, 100, "%s%s", c, z);

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("%s\n", x);
}

