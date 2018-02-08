#include <stdio.h>
#include <stdlib.h>

int main() 
{
  char c;

  while ((c = getc(stdin)) != EOF) {
    printf("%c\n", c);
    if (c == '1') {
      ungetc('~', stdin);
    } 
  }
}

