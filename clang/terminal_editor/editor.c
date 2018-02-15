#include "editor.h"

void init(tedi *e, int argc, char *argv[])
{
  struct winsize w;
  enableRawMode();

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  e->nrow = w.ws_row;
  e->ncol = w.ws_col;

  e->rows = (trow *) malloc(e->nrow);
  e->ptrow = e->rows;

  startLoop(e); 
}

void startLoop(tedi *e)
{
  while(1) {
    refreshScreen(e);
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
    if (tcsetattr(STDOUT_FILENO, TCSAFLUSH, &raw) < 0) goto fatal;
    return 0;

fatal:
    return -1;
}

void disableRawMode() {
   /* Don't even check the return value as it's too late. */
  tcsetattr(STDOUT_FILENO, TCSAFLUSH, &orig_termios);
}

void listenKey()
{
  int nread;
  char key;

  while ((nread = read(STDIN_FILENO, &key, 1)) == 0);
  if (nread == -1) exit(-1);

  if (key == 'q') {
    disableRawMode();
    exit(0);
  }
}

void appendRow(tedi *e, char *str)
{
  e->ptrow->render = str;
  e->ptrow += 1;
}

void refreshScreen(tedi *e)
{
  int lineno = 0;
  trow *cur = e->rows;
  e->ptrow = e->rows; 

  appendRow(e, ANSI_HIDE_CURSOR ANSI_GO_HOME "\x1b[38;2;30;82;190m Hello \x1b[48;2;21;167;216m World" ANSI_RST_STYLE);

  while (lineno++ < e->nrow) {
    if (cur < e->ptrow) {
      printf("%s", cur->render);
      cur += 1;
      continue;
    }

    printf("\r\n%d %s", lineno, "!");
  }

  printf(ANSI_SHOW_CURSOR);

}




