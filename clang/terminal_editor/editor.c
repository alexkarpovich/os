#include "editor.h"

void init(tedi *e, int argc, char *argv[])
{
  struct winsize w;
  enableRawMode();

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  e->nrow = w.ws_row;
  e->ncol = w.ws_col;

  startLoop(); 
}

void startLoop()
{
  while(1) {
    refreshScreen();
    listenKey();
  }
}


int enableRawMode() {
    struct termios raw;

    if (!isatty(STDIN_FILENO)) goto fatal;
    if (tcgetattr(STDOUT_FILENO, &orig_termios) == -1) goto fatal;

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
    if (tcsetattr(STDOUT_FILENO, TCSAFLUSH,&raw) < 0) goto fatal;
    return 0;

fatal:
    return -1;
}

void disableRawMode() {
   /* Don't even check the return value as it's too late. */
  tcsetattr(STDOUT_FILENO, TCSAFLUSH,&orig_termios);
}

void listenKey()
{
  int nread;
  char key;

  while ((nread = read(STDIN_FILENO, &key, 1)) == 0);
  if (nread == -1) exit(-1);

  if (key == 'q') {
    disableRawMode(STDIN_FILENO);
    exit(0);
  }
}

void refreshScreen()
{
  
}




