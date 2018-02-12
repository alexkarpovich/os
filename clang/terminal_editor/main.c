#ifdef __KERNEL__
#include <linux/ioctl.h>
#else
#include <sys/ioctl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define ANSI_HIDE_CURSOR "\x1b[?25l"
#define ANSI_GO_HOME "\x1b[H"
#define ANSI_SHOW_CURSOR "\x1b[?25h"
#define ANSI_RST_STYLE "\x1b[0m"


struct econfig {
  int rowno;
  int colno;
  char *data;
} econf;

struct econtent {
  int pos;
  char *data;
} econt;

static int BUF_SIZE;
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

void appendBuffer(char *str)
{
  snprintf(econt.data + econt.pos, BUF_SIZE, "%s%s", econt.data, str);
}

char *prepareTopbar() 
{
  char *beg = "\x1b[48;5;26m[ \x1b[32;5mundefined\x1b[0m\x1b[48;5;26m ]", 
       *end = ANSI_RST_STYLE;
  int count = econf.colno - sizeof(beg) - 5;
  char * topbar = (char *) malloc(econf.colno),
       * format = (char *) malloc(10);
  
  snprintf(format, 10, "%%s%%-%ds%%s", count);
  snprintf(topbar, 300, format, beg, " ", end);

  free(format);

  return topbar;
   
}

void refreshScreen()
{
  econt.pos = 0;
  /* Hide cursor and move it to top left corner */
  char *specchars = ANSI_HIDE_CURSOR ANSI_GO_HOME;
  char *show_cursor = ANSI_SHOW_CURSOR;
  char *topbar = prepareTopbar();

  int actual_size = snprintf(econt.data, BUF_SIZE, "%s%s%s", specchars, topbar, show_cursor);
  free(topbar);
  
  fflush(stdin); /* force it to go out */
  write(STDOUT_FILENO, econt.data, actual_size);
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

int main() {
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  econf.rowno = w.ws_row;
  econf.colno = w.ws_col;
  enableRawMode(STDIN_FILENO);

  BUF_SIZE = w.ws_row * w.ws_col * 2;

  econt.data = (char *) malloc(BUF_SIZE);

  while (1) {
    refreshScreen();
    listenKey();
  }


  return 1;
}
