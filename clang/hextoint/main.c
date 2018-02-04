#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int htoi(char *hex) 
{
  size_t len = strlen(hex);
  int res = 0, rank, m;
  char c;

  if (len <= 2) {
    perror("Wrong hex value, should be started with 0x or 0X");
    return 0;
  } 
  
  for (int i = 2; i < len; i++) {
    c = hex[i];
    rank = len - 2 - i; 

    if (c >= '0' && c <= '9') {
      m = c - '0'; 
    } else if (c >= 'a' && c <= 'f') {
      m = c - 'a' + 10; 
    } else if (c >= 'A' && c <= 'F') {
      m = c - 'A' + 10;
    } else if (c == '\n') continue;

    res += m * pow(16, rank);
  } 

  return res;
}

int main() 
{
  char c, *buffer;
  size_t bufsize = 32;
  int res;

  buffer = (char *)malloc(bufsize * sizeof(char));
  
  if (buffer == NULL) {
    perror("Unable to allocate memory\n");
    exit(1);
  }

  while (getline(&buffer, &bufsize, stdin) > 0) {
    res = htoi(buffer); 

    printf("%d\n", res);
  }
}

