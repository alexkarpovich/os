#include <stdio.h>
#define BLK_NO 5

int main(int argc, char *argv[]) 
{
  int line_number = 1;
  char c;
  FILE *file;
  int stats[BLK_NO]; 
  for (int i = 0; i < BLK_NO; i++) stats[i] = 0;
  
  if (argc < 2) {
    printf("Expecting path to *.c file\n");
    return -1;
  }

  file = fopen(argv[1], "r");

  if (file) {
    while ((c = getc(file)) != EOF) {
      switch (c) {
        case '{': 
          ++stats[0]; 
          break;
        case '}': --stats[0]; break;
        case '(': ++stats[1]; break;
        case ')': --stats[1]; break;
        case '[': ++stats[2]; break;
        case ']': --stats[2]; break;
        case '\'': 
          if (stats[3]) --stats[3];
          else ++stats[3];
          break;
        case '"': 
          if (stats[4]) --stats[4];
          else ++stats[4];
          break;
      }
    } 

    fclose(file);
  }

  for (int i = 0; i < BLK_NO; i++) {
    if (stats[i] > 0) {
      printf("%d hasn't been closed\n", i); 
    } else if (stats[i] < 0) {
      printf("%d hasn't been opened\n", i); 
    } 
  }
  
  return 0;
}
