#ifndef EDITOR_H
#define EDITOR_H
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


struct row {
  int lineno;
  char *render;
};

typedef struct row trow;

struct editor {
  int nrow;
  int ncol;
  char *data;
  trow *rows;    
};

typedef struct editor tedi;
static struct termios orig_termios;

void init(tedi *e, int argc, char *argv[]);
int enableRawMode();
void disableRawMode();
void refreshScreen();
void listenKey();
void startLoop();

#endif
