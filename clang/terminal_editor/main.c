#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


struct econfig {
  int rowno;
  int colno;
  char *data;
} econf;

static struct termios orig_termios;

int enableRawMode(int fd) {
    struct termios raw;

    if (!isatty(STDIN_FILENO)) goto fatal;
    if (tcgetattr(fd,&orig_termios) == -1) goto fatal;

    raw = orig_termios;  /* modify the original mode */
    /* input modes: no break, no CR to NL, no parity check, no strip char,
     * no start/stop output control. */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /* output modes - disable post processing */
    raw.c_oflag &= ~(OPOST);
    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);
    /* local modes - choing off, canonical off, no extended functions,
     * no signal chars (^Z,^C) */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    /* control chars - set return condition: min number of bytes and timer. */
    raw.c_cc[VMIN] = 0; /* Return each byte, or zero for timeout. */
    raw.c_cc[VTIME] = 1; /* 100 ms timeout (unit is tens of second). */

    /* put terminal in raw mode after flushing */
    if (tcsetattr(fd,TCSAFLUSH,&raw) < 0) goto fatal;
    return 0;

fatal:
    return -1;
}

void disableRawMode(int fd) {
   /* Don't even check the return value as it's too late. */
  tcsetattr(fd,TCSAFLUSH,&orig_termios);
}

void refreshScreen()
{
  /* Hide terminal cursor */
  printf("\x1b[?25l");
  printf("\x1b[H");
  char *topbar = "\x1b[48;5;26m[ \x1b[32;3mHello my dear\x1b[0m";

  strcpy(econf.data, topbar) ;
  
  fflush(stdin); /* force it to go out */
  printf("\x1b[?25h"); /* Show cursor. */
  write(STDOUT_FILENO, econf.data, econf.rowno * econf.colno);
}

void listenKey()
{
  int nread;
  char key;

  while ((nread = read(STDIN_FILENO, &key, 1)) == 0);
  if (nread == -1) exit(-1);

  if (key == 'q') exit(0);
}

int main() {
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  econf.rowno = w.ws_row;
  econf.colno = w.ws_col;
  enableRawMode(STDIN_FILENO);

  int scr_size = w.ws_row * w.ws_col * 2;

  econf.data = (char *) malloc(scr_size);

  while (1) {
    refreshScreen();
    listenKey();
  }

  disableRawMode(STDIN_FILENO);

  return 1;
}
