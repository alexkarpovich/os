#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 256

int main() {
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  int scr_size = w.ws_row * w.ws_col;

  char *scr = (char *) malloc(scr_size);

  for (int i = 0; i < w.ws_row; i++) {
    for (int j = 0; j < w.ws_col; j++) {
      scr[i * w.ws_col + j] = '#';
    }
  }

  fflush(stdout); /* force it to go out */
  write(1, scr, scr_size);
  printf("\e[31;3mHello\e[0m");

  return 1;
}
