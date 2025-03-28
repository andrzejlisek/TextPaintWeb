/* $Id: sixel.c,v 1.30 2024/12/05 00:43:14 tom Exp $ */

#include "vttest.h"
#include "ttymodes.h"
#include "esc.h"
#include "fakeio.h"
#define is_inter(c) ((c) >= 0x20 && ((c) <= 0x2f))
#define is_final(c) ((c) >= 0x30 && ((c) <= 0x7e))

#define L_CURL '{'
#define MAX_WIDTH 10

static char empty[1];

static const char *EraseCtl = "";
static const char *FontName = "";
static char *StartingCharPtr = empty;
static const char *TextCell = "";
static const char *WidthAttr = "";
static char *font_string = empty;

static int FontNumber;
static int MatrixHigh;
static int MatrixWide;
static int StartingCharNum;

/*
 * Lookup the given character 'chr' in the font-string and write a readable
 * display of the glyph
 */
static void
decode_header(void)
{
  int Pe, Pcms, Pw, Pt;
  char *s;

  switch (sscanf(font_string + 2,
                 "%d;%d;%d;%d;%d;%d",
                 &FontNumber, &StartingCharNum, &Pe, &Pcms, &Pw, &Pt)) {
  case 0:
    FontNumber = 0;
    /* FALLTHRU */
  case 1:
    StartingCharNum = 0;
    /* FALLTHRU */
  case 2:
    Pe = 0;
    /* FALLTHRU */
  case 3:
    Pcms = 0;
    /* FALLTHRU */
  case 4:
    Pw = 0;
    /* FALLTHRU */
  case 5:
    Pt = 0;
    /* FALLTHRU */
  case 6:
    break;
  }

  switch (Pcms) {
  case 1:
    MatrixWide = 0;
    MatrixHigh = 0;
    break;      /* illegal */
  case 2:
    MatrixWide = 5;
    MatrixHigh = 10;
    break;
  case 3:
    MatrixWide = 6;
    MatrixHigh = 10;
    break;
  case 0:
  case 4:
    MatrixWide = 7;
    MatrixHigh = 10;
    break;
  default:
    MatrixWide = Pcms;
    MatrixHigh = 10;
    break;      /* 5 through 10 */
  }

  switch (Pe) {
  case 0:
    EraseCtl = "this DRCS set";
    break;
  case 1:
    EraseCtl = "only reloaded chars";
    break;
  case 2:
    EraseCtl = "all chars in all DRCS sets";
    break;
  default:
    EraseCtl = "?";
    break;
  }

  switch (Pw) {
  case 0:
    /* FALLTHRU */
  case 1:
    WidthAttr = "80 cols, 24 lines";
    break;
  case 2:
    WidthAttr = "132 cols, 24 lines";
    break;
  case 11:
    WidthAttr = "80 cols, 36 lines";
    break;
  case 12:
    WidthAttr = "132 cols, 36 lines";
    break;
  case 21:
    WidthAttr = "80 cols, 24 lines";
    break;
  case 22:
    WidthAttr = "132 cols, 48 lines";
    break;
  default:
    WidthAttr = "?";
    break;
  }

  if (Pt == 2)
    TextCell = "Full Cell";
  else
    TextCell = "Text";
  for (s = font_string; *s; s++) {
    if (*s == L_CURL) {
      char *t;
      char tmp[BUF_SIZE];
      size_t use = 0;
      for (t = s + 1; *t; t++) {
        if (is_inter(*t)) {
          tmp[use++] = *t;
        }
        if (is_final(*t)) {
          char *tmp2;
          tmp[use++] = *t++;
          tmp[use] = '\0';
          if ((tmp2 = (char*)malloc(use + 1)) == NULL) {
            no_memory();
          } else {
            FontName = strcpy(tmp2, tmp);
            StartingCharPtr = t;
          }
          break;
        }
      }
      break;
    }
  }
}

static char *
find_char(int chr)
{
  char *s = StartingCharPtr;

  chr -= (' ' + StartingCharNum);
  if (chr < 0)
    return NULL;
  while (chr > 0) {
    do {
      if (*s == '\0')
        return NULL;
    } while (*s++ != ';');
    chr--;
  }
  return s;
}

static void
display_head(FILE *fp)
{
  fakeio::_fprintf(fp, "Font %d \"%s\", Matrix %dx%d (%s, %s)\n",
          FontNumber, FontName, MatrixWide, MatrixHigh, WidthAttr, TextCell);
  fakeio::_fprintf(fp, "Start %d, Erase %s\n",
          StartingCharNum, EraseCtl);
}

static int
display_char(FILE *fp, int chr)
{
  char *s;

  s = find_char(chr);
  if (s != NULL) {
    int bit = 0;
    int high = 0;
    int n;
    char bits[6][MAX_WIDTH];

    fakeio::_fprintf(fp, "Glyph '%c'\n", chr);
    do {
      if (*s >= '?' && *s <= '~') {
        for (n = 0; n < 6; n++)
          bits[n][bit] = (char) (((*s - '?') & 1 << n) ? 'O' : '.');
        bit++;
      } else if ((*s == ';' || *s == '/') && bit) {
        for (n = 0; (n < 6) && (high++ < MatrixHigh); n++) {
          bits[n][bit] = '\0';
          fakeio::_fprintf(fp, "%s\n", bits[n]);
        }
        bit = 0;
      }
    } while (*s++ != ';');
    return TRUE;
  }
  return FALSE;
}

static int
tst_DECDLD(MENU_ARGS)
{
  char *s;

  vt_move(1, 1);
  printxx("Working...\n");
  for (s = font_string; *s; s++) {
    fakeio::_putchar(*s);
    if (*s == '\n') {
      fakeio::_fflush(stdout);
      padding(20);
    }
  }
  fakeio::_fflush(stdout);
  padding(20);
  printxx("...done ");

  tprintf("%c*%s", ESC, FontName);  /* designate G2 as the DRCS font */
  padding(4);
  fakeio::_fflush(stdout);

  return MENU_HOLD;
}

static int
tst_display(MENU_ARGS)
{
  int d, c = -1;

  vt_move(1, 1);
  display_head(stdout);
  println("");
  println("Press any key to display its soft-character.  Repeat a key to quit.");

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  pause_replay();
  do {
    d = c;
    c = get_char();
    if (c < 0)
      break;
    vt_move(6, 1);
    vt_clear(0);
    if (display_char(stdout, c)) {
      println("");
      cprintf("Render: %cN%c", ESC, c);   /* use SS2 to invoke G2 into GL */
    }
  } while (c != d);
  resume_replay();

  restore_ttymodes();
  return MENU_NOHOLD;
}

/*
 * Remove all characters in all DRCS sets (the " @" is a dummy name)
 */
static int
tst_cleanup(MENU_ARGS)
{
  do_dcs("1;1;2%c @", L_CURL);
  padding(20);
  return MENU_NOHOLD;
}

/*
 * Read a soft-character definition string from a file.  Strip off garbage
 * at the beginning (to accommodate the "font2xx" output format).
 */
void
setup_softchars(const char *filename)
{
  FILE *fp;
  int c;
  size_t len = 1024;
  size_t use = 0;
  char *buffer;
  char *s;
  const char *first = NULL;
  char *last = NULL;
  int save_8bits = input_8bits;
  input_8bits = FALSE;  /* use the 7-bit input-parsing */

  /* read the file into memory */
  if ((fp = fakeio::_fopen(filename, "r")) == NULL) {
    failed(filename);
  }
  if ((buffer = (char*)malloc(len)) == NULL)
    no_memory();
  while ((c = fakeio::_fgetc(fp)) != EOF) {
    if (use + 1 >= len) {
      char *check;
      if ((check = (char*)realloc(buffer, len *= 2)) == NULL) {
        no_memory();
      } else {
        buffer = check;
      }
    }
    buffer[use++] = (char) c;
  }
  buffer[use] = '\0';
  fakeio::_fclose(fp);

  /* find the DCS that begins the control string */
  /* and the ST that ends the control string */
  for (s = buffer; *s; s++) {
    if (first == NULL) {
      if (skip_dcs(s) != NULL)
        first = s;
    } else {
      if (!strncmp(s, st_input(), (size_t) 2)) {
        last = s + 2;
        *last = '\0';
        break;
      }
    }
  }
  input_8bits = save_8bits;

  if (first == NULL || last == NULL) {
    fakeio::_fprintf(stderr, "Not a vtXXX font description: %s\n", filename);
    fakeio::_exit(EXIT_FAILURE);
  }
  for (s = buffer; (*s++ = *first++) != '\0';) ;
  if (LOG_ENABLED && first != NULL)
    fakeio::_fprintf(log_fp, NOTE_STR "font string %s\n", buffer);

  font_string = buffer;

  decode_header();
}

int
tst_softchars(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Download the soft characters (DECDLD)",             tst_DECDLD },
      { "Examine the soft characters",                       tst_display },
      { "Clear the soft characters",                         tst_cleanup },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  vt_move(1, 1);
  if (font_string == NULL || *font_string == 0) {
    printxx("You did not specify a font-file with the -f option\n");
    return MENU_HOLD;
  }
  do {
    vt_clear(2);
    __(title(0), printxx("Soft Character Sets"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}
