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

  if (argc > 1) readFile(e, argv[1]);

  startLoop(e); 
}

void startLoop(tedi *e)
{
  while(1) {
    refreshScreen(e);
    listenKey();
  }
}

void readFile(tedi *e, char *filename)
{
  FILE * file = fopen(filename, "r");

  if (file == NULL) {
    perror("Cannot open the file.\n");
    exit(-1);
  }

  /* Get file size */
  fseek(file, 0, SEEK_END); 
  long size = ftell(file);
  fseek(file, 0, SEEK_SET); 

  e->data = (char *) malloc(size);
  fread(e->data, 1, size, file);
  fclose(file);
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

void addTopbar(tedi *e)
{
  char *begin = ANSI_HIDE_CURSOR ANSI_GO_HOME "\x1b[38;2;30;82;190m Hello \x1b[48;2;21;167;216m World",
       *format = (char *) malloc(14), 
       *res = (char *) malloc(e->ncol * 3);
  int padding = e->ncol - strlen(begin) + 44;

  snprintf(format, 14, "%%s%%-%ds%%s", padding);
  snprintf(res, e->ncol * 3, format, begin, " ", ANSI_RST_STYLE); 

  free(format); 
  appendRow(e, res);
}

void addStatusbar(tedi *e)
{
  char *begin = "\x1b[38;2;30;82;190m status \x1b[48;2;21;167;216m bar",
       *format = (char *) malloc(14), 
       *res = (char *) malloc(e->ncol * 3);
  int padding = e->ncol - strlen(begin) + 35;

  snprintf(format, 14, "%%s%%-%ds%%s", padding);
  snprintf(res, e->ncol * 3, format, begin, " ", ANSI_RST_STYLE ANSI_SHOW_CURSOR); 

  free(format); 
  appendRow(e, res);
}

void addContent(tedi *e)
{
  long count,
       pos = 0, 
       prev_pos = 0, 
       fsize = e->data != NULL ? strlen(e->data) : 0;
  char *str;

  /* Prepare editor rows with data from file */
  while (pos < fsize) {
    if (e->data[pos] == '\n' || e->data[pos] == '\0') {
      count = pos - prev_pos + 1;
      str = (char *) malloc(count); 

      strncpy(str, e->data + prev_pos, count);
      str[count - 1] = '\0';
      appendRow(e, str);
      
      prev_pos = pos + 1;
    } 

    ++pos;
  }

  /* Calculate pointer to the last content row */
  trow *lptr = e->rows + e->nrow - 2;

  /* Fill empty files with ~ */
  while (e->ptrow < lptr) {
    appendRow(e, "~" ANSI_CLR_LINE); 
  }
}

void refreshScreen(tedi *e)
{
  trow *cur = e->rows;
  e->ptrow = e->rows; 

  addTopbar(e);
  addContent(e);
  addStatusbar(e);

  while (cur < e->ptrow) {
    printf("%s\r\n", cur->render);
    cur += 1;
  }
}




