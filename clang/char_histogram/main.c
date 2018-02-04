#include <stdio.h>

int main() 
{
  char c;
  int i, j, stat[256];

  for (i = 0; i < 256; i++) {
    stat[i] = 0; 
  }

  while ((c = getchar()) != EOF) {
    ++stat[c];      
  }

  printf("Char staticstics:\n");
  
  for (i = 0; i < 256; i++) {
    if (stat[i]) {
      printf("%c ", i); 
      for (j = 0; j < stat[i]; j++) putchar('#');
      putchar('\n');
    } 
  }
}
