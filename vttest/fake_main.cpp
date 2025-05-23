/* $Id: main.c,v 1.155 2024/12/05 00:37:39 tom Exp $ */

/*
                               VTTEST.C

         Written November 1983 - July 1984 by Per Lindberg,
         Stockholm University Computer Center (QZ), Sweden.

                  THE MAD PROGRAMMER STRIKES AGAIN!

  Copyright (c) 1984, Per Lindberg
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:
  1.  Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
  2.  Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
  3.  Neither the name of Per Lindberg nor the names of contributors may
      be used to endorse or promote products derived from this software
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "vttest.h"
#include "ttymodes.h"
#include "esc.h"
#include "fakeio.h"
#ifdef LOCALE
#include <locale.h>
#endif

#ifdef HAVE_LANGINFO_CODESET
#include <langinfo.h>
#endif

/* *INDENT-EQLS* */
FILE *log_fp    = NULL;
int allows_utf8 = FALSE;
int assume_utf8 = FALSE;
int brkrd       = FALSE;
int debug_level = 0;
int decac_bg    = -1;
int decac_fg    = -1;
int input_8bits = FALSE;
int log_disabled = FALSE;
int max_cols    = 132;
int max_lines   = 24;
int min_cols    = 80;
int output_8bits = FALSE;
int parse_7bits = FALSE;
int quick_reply = FALSE;
int reading     = FALSE;
int slow_motion = FALSE;
int tty_speed   = DEFAULT_SPEED;  /* nominal speed, for padding */
int use_decac   = FALSE;
int use_padding = FALSE;

//jmp_buf intrenv;

static char *program;
static char empty[1];
static char *current_menu = empty;

GCC_NORETURN void
failed(const char *msg)
{
#ifdef HAVE_STRERROR
  const char *why = strerror(errno);
  fakeio::_fprintf(stderr, "%s: %s: %s\n", program, msg, why ? why : "?");
#elif defined(HAVE_SYS_NERR) && defined(HAVE_SYS_ERRLIST)
  const char *why = (errno > 0 && errno < sys_nerr) ? sys_errlist[errno] : "?";
  fakeio::_fprintf(stderr, "%s: %s: %s\n", program, msg, why ? why : "?");
#else
  fakeio::_printf("%s", msg); //perror(msg);
#endif
  fakeio::_exit(EXIT_FAILURE);
}

GCC_NORETURN void
no_memory(void)
{
  failed("no memory");
}

static void
usage(void)
{
  static const char *msg[] =
  {
    "Usage: vttest [options] [24x80.132]"
    ,""
    ,"Options:"
    ,"  -V     print the program version, and exit"
    ,"  -8     use 8-bit controls"
    ,"  -d     debug (repeat for more detail)"
    ,"  -c cmdfile  read commands from file"
    ,"  -f fontfile load DRCS data from file"
    ,"  -l logfile  log test results to vttest.log"
    ,"  -p     use padding"
    ,"  -q     filter replies to show only the most recent"
    ,"  -s     add time delay for scrolling"
    ,"  -u     allow some tests to use UTF-8"
  };
  size_t n;

  for (n = 0; n < TABLESIZE(msg); ++n)
    fakeio::_fprintf(stderr, "%s\n", msg[n]);

  fakeio::_exit(EXIT_FAILURE);
}

static int
version(void)
{
  printxx("VT100 test program, version %d.%d", RELEASE, PATCHLEVEL);
#ifdef PATCH_DATE
  if (PATCH_DATE)
    printxx(" (%d)", PATCH_DATE);
#endif
  return 1;     /* used for indenting */
}

int
fake_main(int argc, char *argv[])
{
  /* *INDENT-OFF* */
  static MENU mainmenu[] = {
      { "Exit",                                              NULL },
      { "Test of cursor movements",                          tst_movements },
      { "Test of screen features",                           tst_screen },
      { "Test of character sets",                            tst_characters },
      { "Test of double-sized characters",                   tst_doublesize },
      { "Test of keyboard",                                  tst_keyboard },
      { "Test of terminal reports",                          tst_reports },
      { "Test of VT52 mode",                                 tst_vt52 },
      { "Test of VT102 features (Insert/Delete Char/Line)",  tst_insdel },
      { "Test of known bugs",                                tst_bugs },
      { "Test of reset and self-test",                       tst_rst },
      { "Test non-VT100 (e.g., VT220, XTERM) terminals",     tst_nonvt100 },
      { "Modify test-parameters",                            tst_setup },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */
  char *opt_command = NULL;
  char *opt_softchr = NULL;
  char *opt_logging = NULL;

  program = strrchr(argv[0], '/');
  if (program != NULL)
    ++program;
  else
    program = argv[0];

#define OPT_ARG(value) \
    do { \
      if (!*++opt) { \
        if (--argc < 1) \
          usage(); \
        value = *++argv; \
      } \
      opt = "?"; \
    } while (0)

  while (argc-- > 1) {
    const char *opt = *++argv;
    if (*opt == '-') {
      while (*++opt != '\0') {
        switch (*opt) {
        case 'V':
          version();
          fakeio::_putchar('\n');
          fakeio::_exit(EXIT_SUCCESS);
        case 'c':
          OPT_ARG(opt_command);
          break;
        case 'd':
          debug_level++;
          break;
        case 'f':
          OPT_ARG(opt_softchr);
          break;
        case 'l':
          OPT_ARG(opt_logging);
          break;
        case 'p':
          use_padding = TRUE;
          break;
        case 'q':
          quick_reply = TRUE;
          break;
        case 's':
          slow_motion = TRUE;
          break;
        case 'u':
          allows_utf8 = TRUE;
          break;
        case '8':
          output_8bits = TRUE;
          break;
        default:
          usage();
        }
      }
    } else {
      /*
       * Allow user to specify geometry of terminal to accommodate quasi-VT100
       * terminals such as Linux console and xterm.
       */
      char *p = argv[0];
      char *q;
      int values[3], n;

      for (n = 0; n < 3; n++) {
        int m;

        if (!*p)
          break;
        if ((m = (int) strtol(p, &q, 10)) > 0) {
          values[n] = m;
          p = q;
          if (*p)
            p++;
        } else {
          break;
        }
      }
      switch (n) {
      case 3:
        max_cols = values[2];
        /* FALLTHRU */
      case 2:
        min_cols = values[1];
        /* FALLTHRU */
      case 1:
        max_lines = values[0];
        break;
      }
      if ((max_cols < min_cols) || (n == 0)) {
        usage();
      }
    }
  }

  /* do this first, to capture results from other options */
  if (opt_logging)
    enable_logging(opt_logging);
  log_disabled = !log_disabled;
  if (opt_command)
    setup_replay(opt_command);

  if (opt_softchr)
    setup_softchars(opt_softchr);

#ifdef UNIX
  initterminal(setjmp(intrenv));
  signal(SIGINT, onbrk);
  signal(SIGTERM, onterm);
  reading = FALSE;
#else
  initterminal(0);
#endif
  do {
    vt_clear(2);
    __(title(0), version());

    title(1);
    if (max_lines != 24
        || min_cols != 80
        || max_cols != 132)
      printxx("Screen size %dx%d (%d max) ", max_lines, min_cols, max_cols);
    if (tty_speed != DEFAULT_SPEED)
      printxx("Line speed %dbd ", tty_speed);
    if (use_padding)
      printxx(" (padded)");

    __(title(2), println("Choose test type:"));
  } while (menu(mainmenu));
  bye();
  return EXIT_SUCCESS;
}

int
tst_movements(MENU_ARGS)
{
  /* Test of:
     CUF (Cursor Forward)
     CUB (Cursor Backward)
     CUD (Cursor Down)      IND (Index)  NEL (Next Line)
     CUU (Cursor Up)        RI  (Reverse Index)
     CUP (Cursor Position)  HVP (Horizontal and Vertical Position)
     ED  (Erase in Display)
     EL  (Erase in Line)
     DECALN (Screen Alignment Display)
     DECAWM (Autowrap)
     <CR> <BS>
     Cursor control characters inside CSI sequences
   */

  int i, row, col, pass, width;
  const char *ctext = "This is a correct sentence";

  set_tty_crmod(TRUE);  /* want to disable tab/space conversion */

  for (pass = 0; pass <= 1; pass++) {
    int hlfxtra;
    int inner_l, inner_r;

    if (pass == 0) {
      deccolm(FALSE);
      width = min_cols;
    } else {
      deccolm(TRUE);
      width = max_cols;
    }

    /* Compute left/right columns for a 60-column box centered in 'width' */
    inner_l = (width - 60) / 2;
    inner_r = 61 + inner_l;
    hlfxtra = (width - 80) / 2;

    if (LOG_ENABLED)
      fakeio::_fprintf(log_fp, NOTE_STR "tst_movements box(%d cols)\n",
              pass ? max_cols : min_cols);

    decaln();
    cup(9, inner_l);
    ed(1);
    cup(18, 60 + hlfxtra);
    ed(0);
    el(1);
    cup(9, inner_r);
    el(0);
    /* 132: 36..97 */
    /*  80: 10..71 */
    for (row = 10; row <= 16; row++) {
      cup(row, inner_l);
      el(1);
      cup(row, inner_r);
      el(0);
    }
    cup(17, 30);
    el(2);
    for (col = 1; col <= width; col++) {
      hvp(max_lines, col);
      tprintf("*");
      hvp(1, col);
      tprintf("*");
    }
    cup(2, 2);
    for (row = 2; row <= max_lines - 1; row++) {
      tprintf("+");
      cub(1);
      ind();
    }
    cup(max_lines - 1, width - 1);
    for (row = max_lines - 1; row >= 2; row--) {
      tprintf("+");
      cub(1);
      ri();
    }
    cup(2, 1);
    for (row = 2; row <= max_lines - 1; row++) {
      tprintf("*");
      cup(row, width);
      tprintf("*");
      cub(10);
      if (row < 10)
        nel();
      else
        tprintf("\n");
    }
    cup(2, 10);
    cub(42 + hlfxtra);
    cuf(2);
    for (col = 3; col <= width - 2; col++) {
      tprintf("+");
      cuf(0);
      cub(2);
      cuf(1);
    }
    cup(max_lines - 1, inner_r - 1);
    cuf(42 + hlfxtra);
    cub(2);
    for (col = width - 2; col >= 3; col--) {
      tprintf("+");
      cub(1);
      cuf(1);
      cub(0);
      tprintf("%c", 8);
    }
    cup(1, 1);
    cuu(10);
    cuu(1);
    cuu(0);
    cup(max_lines, width);
    cud(10);
    cud(1);
    cud(0);

    cup(10, 2 + inner_l);
    for (row = 10; row <= 15; row++) {
      for (col = 2 + inner_l; col <= inner_r - 2; col++)
        tprintf(" ");
      cud(1);
      cub(58);
    }
    cuu(5);
    cuf(1);
    printxx("The screen should be cleared,  and have an unbroken bor-");
    cup(12, inner_l + 3);
    printxx("der of *'s and +'s around the edge,   and exactly in the");
    cup(13, inner_l + 3);
    printxx("middle  there should be a frame of E's around this  text");
    cup(14, inner_l + 3);
    printxx("with  one (1) free position around it.    ");
    holdit();
  }
  deccolm(FALSE);

  /* DECAWM demo */
  for (pass = 0; pass <= 1; pass++) {
    static char on_left[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static char on_right[] = "abcdefghijklmnopqrstuvwxyz";
    int height = sizeof(on_left) - 1;
    int region = max_lines - 6;

    if (LOG_ENABLED)
      fakeio::_fprintf(log_fp, NOTE_STR "tst_movements wrap(%d cols)\n",
              pass ? max_cols : min_cols);

    /* note: DECCOLM clears the screen */
    if (pass == 0) {
      deccolm(FALSE);
      width = min_cols;
    } else {
      deccolm(TRUE);
      width = max_cols;
    }

    println("Test of autowrap, mixing control and print characters.");
    println("The left/right margins should have letters in order:");

    decstbm(3, region + 3);
    decom(TRUE);  /* this also homes the cursor */
    for (i = 0; i < height; ++i) {
      switch (i % 4) {
      case 0:
        /* draw characters as-is, for reference */
        __(cup(region + 1, 1), tprintf("%c", on_left[i]));
        __(cup(region + 1, width), tprintf("%c", on_right[i]));
        tprintf("\n");
        break;
      case 1:
        /* simple wrapping */
        __(cup(region, width), tprintf("%c%c", on_right[i - 1], on_left[i]));
        /* backspace at right margin */
        __(cup(region + 1, width), tprintf("%c%c %c",
                                           on_left[i], BS, on_right[i]));
        tprintf("\n");
        break;
      case 2:
        /* tab to right margin */
        __(cup(region + 1, width), tprintf("%c%c%c%c%c%c",
                                           on_left[i], BS, BS,
                                           TAB, TAB, on_right[i]));
        __(cup(region + 1, 2), tprintf("%c%c\n", BS, on_left[i]));
        break;
      default:
        /* newline at right margin */
        __(cup(region + 1, width), tprintf("\n"));
        __(cup(region, 1), tprintf("%c", on_left[i]));
        __(cup(region, width), tprintf("%c", on_right[i]));
        break;
      }
    }
    decom(FALSE);
    decstbm(0, 0);
    cup(max_lines - 2, 1);
    holdit();
  }
  deccolm(FALSE);   /* 80 cols */

  if (LOG_ENABLED)
    fakeio::_fprintf(log_fp,
            NOTE_STR "tst_movements cursor-controls in ESC sequences\n");

  vt_clear(2);
  vt_move(1, 1);
  println("Test of cursor-control characters inside ESC sequences.");
  println("Below should be four identical lines:");
  println("");
  println("A B C D E F G H I");
  for (i = 1; i < 10; i++) {
    tprintf("%c", '@' + i);
    do_csi("2%cC", BS);   /* Two forward, one backspace */
  }
  println("");
  /* Now put CR in CUF sequence. */
  tprintf("A ");
  for (i = 2; i < 10; i++) {
    cprintf("%s%c%dC", csi_output(), CR, 2 * i - 2);
    tprintf("%c", '@' + i);
  }
  println("");
  /* Now put VT in CUU sequence. */
  rm("20");
  for (i = 1; i < 10; i++) {
    tprintf("%c ", '@' + i);
    do_csi("1\013A");
  }
  println("");
  println("");
  holdit();

  if (LOG_ENABLED)
    fakeio::_fprintf(log_fp,
            NOTE_STR "tst_movements leading zeros in ESC sequences\n");

  vt_clear(2);
  vt_move(1, 1);
  println("Test of leading zeros in ESC sequences.");
  printxx("Two lines below you should see the sentence \"%s\".", ctext);
  for (col = 1; *ctext; col++) {
    cprintf("%s00000000004;00000000%dH", csi_output(), col);
    tprintf("%c", *ctext++);
  }
  cup(20, 1);

  restore_ttymodes();
  return MENU_HOLD;
}

/* Scrolling test (used also in color-testing) */
void
do_scrolling(void)
{
  int soft, first, last, down, i, pass;

  ed(2);
  decom(TRUE);  /* Origin mode (relative) */
  for (soft = -1; soft <= 0; soft++) {
    if (soft)
      decsclm(TRUE);
    else
      decsclm(FALSE);
    for (pass = 0; pass < 2; ++pass) {
      if (pass == 0) {
        first = max_lines / 2;
        last = first + 1;
      } else {
        first = 1;
        last = max_lines;
      }
      decstbm(first, last);
      ed(2);
      for (down = 0; down >= -1; down--) {
        if (down)
          cuu(max_lines);
        else
          cud(max_lines);
        for (i = 1; i <= max_lines + 5; i++) {
          printxx("%s scroll %s region [%d..%d] size %d Line %d\n",
                  soft ? "Soft" : "Jump",
                  down ? "down" : "up",
                  first, last, last - first + 1, i);
          if (down) {
            ri();
            ri();
          } else if (soft)
            extra_padding(10);
        }
      }
      holdit();
    }
  }
}

int
tst_screen(MENU_ARGS)
{
  /* Test of:
     - DECSTBM (Set Top and Bottom Margins)
     - TBC     (Tabulation Clear)
     - HTS     (Horizontal Tabulation Set)
     - SM RM   (Set/Reset mode): - 80/132 chars
     .                           - Origin: Relative/absolute
     .                           - Scroll: Smooth/jump
     .                           - Wraparound
     - SGR     (Select Graphic Rendition)
     - SM RM   (Set/Reset Mode) - Inverse
     - DECSC   (Save Cursor)
     - DECRC   (Restore Cursor)
   */

  int i, j, cset, row, col, background;

  static const char *tststr = "*qx`";
  static const char *attr[5] =
  {
    ";0", ";1", ";4", ";5", ";7"
  };

  set_tty_crmod(TRUE);  /* want to disable tab/space conversion */

  cup(1, 1);
  decawm(TRUE); /* DECAWM: Wrap Around ON */
  for (col = 1; col <= min_cols * 2; col++)
    tprintf("*");
  decawm(FALSE);  /* DECAWM: Wrap Around OFF */
  cup(3, 1);
  for (col = 1; col <= min_cols * 2; col++)
    tprintf("*");
  decawm(TRUE); /* DECAWM: Wrap Around ON */
  cup(5, 1);
  println("This should be three identical lines of *'s completely filling");
  println("the top of the screen without any empty lines between.");
  println("(Test of WRAP AROUND mode setting.)");
  holdit();

  ed(2);
  tbc(3);
  cup(1, 1);
  for (col = 1; col <= min_cols - 2; col += 3) {
    cuf(3);
    hts();
  }
  cup(1, 4);
  for (col = 4; col <= min_cols - 2; col += 6) {
    tbc(0);
    cuf(6);
  }
  cup(1, 7);
  tbc(1);
  tbc(2);       /* no-op */
  cup(1, 1);
  for (col = 1; col <= min_cols - 2; col += 6)
    tprintf("%c*", TAB);
  cup(2, 2);
  for (col = 2; col <= min_cols - 2; col += 6)
    tprintf("     *");
  cup(4, 1);
  println("Test of TAB setting/resetting. These two lines");
  printxx("should look the same. ");
  holdit();
  for (background = 0; background <= 1; background++) {
    if (background)
      decscnm(FALSE);
    else
      decscnm(TRUE);
    deccolm(TRUE);  /* 132 cols */
    ed(2);      /* VT100 clears screen on SM3/RM3, but not obviously, so... */
    cup(1, 1);
    tbc(3);
    for (col = 1; col <= max_cols; col += TABWIDTH) {
      cuf(TABWIDTH);
      hts();
    }
    cup(1, 1);
    for (col = 1; col <= max_cols; col += 10)
      tprintf("%.*s", (max_cols > col) ? (max_cols - col) : 10, "1234567890");
    for (row = 3; row <= 20; row++) {
      cup(row, row);
      tprintf("This is %d column mode, %s background.", max_cols,
              background ? "dark" : "light");
    }
    holdit();
    deccolm(FALSE);   /* 80 cols */
    ed(2);      /* VT100 clears screen on SM3/RM3, but not obviously, so... */
    cup(1, 1);
    for (col = 1; col <= min_cols; col += 10)
      tprintf("%.*s", (min_cols > col) ? (min_cols - col) : 10, "1234567890");
    for (row = 3; row <= 20; row++) {
      cup(row, row);
      tprintf("This is %d column mode, %s background.", min_cols,
              background ? "dark" : "light");
    }
    holdit();
  }
  do_scrolling();
  ed(2);
  decstbm(max_lines - 1, max_lines);
  printxx(
           "\nOrigin mode test. This line should be at the bottom of the screen.");
  cup(1, 1);
  tprintf("%s",
          "This line should be the one above the bottom of the screen. ");
  holdit();
  ed(2);
  decom(FALSE); /* Origin mode (absolute) */
  cup(max_lines, 1);
  tprintf(
           "Origin mode test. This line should be at the bottom of the screen.");
  cup(1, 1);
  tprintf("%s", "This line should be at the top of the screen. ");
  holdit();
  decstbm(1, max_lines);

  ed(2);
  /* *INDENT-OFF* */
  cup( 1,20); tprintf("Graphic rendition test pattern:");
  cup( 4, 1); sgr("0");         tprintf("vanilla");
  cup( 4,40); sgr("0;1");       tprintf("bold");
  cup( 6, 6); sgr(";4");        tprintf("underline");
  cup( 6,45);sgr(";1");sgr("4");tprintf("bold underline");
  cup( 8, 1); sgr("0;5");       tprintf("blink");
  cup( 8,40); sgr("0;5;1");     tprintf("bold blink");
  cup(10, 6); sgr("0;4;5");     tprintf("underline blink");
  cup(10,45); sgr("0;1;4;5");   tprintf("bold underline blink");
  cup(12, 1); sgr("1;4;5;0;7"); tprintf("negative");
  cup(12,40); sgr("0;1;7");     tprintf("bold negative");
  cup(14, 6); sgr("0;4;7");     tprintf("underline negative");
  cup(14,45); sgr("0;1;4;7");   tprintf("bold underline negative");
  cup(16, 1); sgr("1;4;;5;7");  tprintf("blink negative");
  cup(16,40); sgr("0;1;5;7");   tprintf("bold blink negative");
  cup(18, 6); sgr("0;4;5;7");   tprintf("underline blink negative");
  cup(18,45); sgr("0;1;4;5;7"); tprintf("bold underline blink negative");
  /* *INDENT-ON* */

  sgr("");

  decscnm(FALSE);   /* Inverse video off */
  cup(max_lines - 1, 1);
  el(0);
  tprintf("Dark background. ");
  holdit();

  decscnm(TRUE);  /* Inverse video */
  cup(max_lines - 1, 1);
  el(0);
  tprintf("Light background. ");
  holdit();

  decscnm(FALSE);

  ed(2);
  /* *INDENT-OFF* */
  cup(8,12); tprintf("normal");
  cup(8,24); tprintf("bold");
  cup(8,36); tprintf("underscored");
  cup(8,48); tprintf("blinking");
  cup(8,60); tprintf("reversed");
  cup(10,1); tprintf("stars:");
  cup(12,1); tprintf("line:");
  cup(14,1); tprintf("x'es:");
  cup(16,1); tprintf("diamonds:");
  /* *INDENT-ON* */

  for (cset = 0; cset <= 3; cset++) {
    for (i = 0; i <= 4; i++) {
      cup(10 + 2 * cset, 12 + 12 * i);
      sgr(attr[i]);
      if (cset == 0 || cset == 2)
        scs_normal();
      else
        scs_graphics();
      for (j = 0; j <= 4; j++) {
        tprintf("%c", tststr[cset]);
      }
      decsc();
      cup(cset + 1, i + 1);
      sgr("");
      scs_normal();
      tprintf("A");
      decrc();
      for (j = 0; j <= 4; j++) {
        tprintf("%c", tststr[cset]);
      }
    }
  }

  sgr("0");
  scs_normal();
  cup(21, 1);
  println("Test of the SAVE/RESTORE CURSOR feature. There should");
  println("be ten characters of each flavour, and a rectangle");
  println("of 5 x 4 A's filling the top left of the screen.");

  restore_ttymodes();
  return MENU_HOLD;
}

int
tst_doublesize(MENU_ARGS)
{
  /* Test of:
     DECSWL  (Single Width Line)
     DECDWL  (Double Width Line)
     DECDHL  (Double Height Line) (also implicit double width)
   */

  int col, i, w;

  /* Print the test pattern in both 80 and 132 character width  */

  for (w = 0; w <= 1; w++) {
    int w1 = 13 * w;

    ed(2);
    cup(1, 1);
    if (w) {
      deccolm(TRUE);
      tprintf("%3d column mode", max_cols);
    } else {
      deccolm(FALSE);
      tprintf("%3d column mode", min_cols);
    }

    cup(5, 3 + 2 * w1);
    tprintf("v------- left margin");

    cup(7, 3 + 2 * w1);
    tprintf("This is a normal-sized line");

    decdhl(0);
    decdhl(1);
    decdwl();
    decswl();
    cup(9, 2 + w1);
    tprintf("This is a Double-width line");

    decswl();
    decdhl(0);
    decdhl(1);
    decdwl();

    cup(11, 2 + w1);
    decdwl();
    decswl();
    decdhl(1);
    decdhl(0);
    tprintf("This is a Double-width-and-height line");

    cup(12, 2 + w1);
    decdwl();
    decswl();
    decdhl(0);
    decdhl(1);
    tprintf("This is a Double-width-and-height line");

    cup(14, 2 + w1);
    decdwl();
    decswl();
    decdhl(1);
    decdhl(0);
    el(2);
    tprintf("This is another such line");

    cup(15, 2 + w1);
    decdwl();
    decswl();
    decdhl(0);
    decdhl(1);
    tprintf("This is another such line");

    cup(17, 3 + 2 * w1);
    tprintf("^------- left margin");

    cup(21, 1);
    tprintf("This is not a double-width line");
    for (i = 0; i <= 1; i++) {
      cup(21, 6);
      if (i) {
        tprintf("**is**");
        decdwl();
      } else {
        tprintf("is not");
        decswl();
      }
      cup(max_lines - 1, 1);
      holdit();
    }
  }
  /* Set vanilla tabs for next test */
  cup(1, 1);
  tbc(3);
  for (col = 1; col <= max_cols; col += TABWIDTH) {
    cuf(TABWIDTH);
    hts();
  }
  deccolm(FALSE);
  ed(2);
  /* *INDENT-OFF* */
  scs_graphics();
  cup( 8,1); decdhl(0); tprintf("lqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqk");
  cup( 9,1); decdhl(1); tprintf("lqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqk");
  cup(10,1); decdhl(0); tprintf("x%c%c%c%c%cx",9,9,9,9,9);
  cup(11,1); decdhl(1); tprintf("x%c%c%c%c%cx",9,9,9,9,9);
  cup(12,1); decdhl(0); tprintf("x%c%c%c%c%cx",9,9,9,9,9);
  cup(13,1); decdhl(1); tprintf("x%c%c%c%c%cx",9,9,9,9,9);
  scs(1, '0');  /* should look the same as scs_graphics() */
  cup(14,1); decdhl(0); tprintf("x                                      x");
  cup(15,1); decdhl(1); tprintf("x                                      x");
  cup(16,1); decdhl(0); tprintf("mqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqj");
  cup(17,1); decdhl(1); tprintf("mqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqj");
  scs_normal();
  /* *INDENT-ON* */

  sgr("1;5");
  cup(12, 3);
  tprintf("* The mad programmer strikes again * ");

  cup(13, 3);
  tprintf("%c", 9);

  cub(6);
  tprintf("* The mad programmer strikes again *");
  sgr("0");

  cup(max_lines - 2, 1);
  println("Another test pattern...  a frame with blinking bold text,");
  printxx("all in double-height double-width size. ");
  holdit();

  decstbm(8, max_lines);  /* Absolute origin mode, so cursor is set at (1,1) */
  cup(8, 1);
  for (i = 1; i <= 12; i++)
    ri();
  decstbm(0, 0);  /* No scroll region     */
  cup(1, 1);
  printxx("%s", "Exactly half of the box should remain. ");
  return MENU_HOLD;
}

int
tst_insdel(MENU_ARGS)
{
  /* Test of:
     SM/RM(4) (= IRM (Insertion/replacement mode))
     ICH (Insert Character)
     DCH (Delete character)
     IL  (Insert line)
     DL  (Delete line)
   */

  int i, row, col, sw, dblchr, scr132;

  for (scr132 = 0; scr132 <= 1; scr132++) {
    if (scr132) {
      deccolm(TRUE);
      sw = max_cols;
    } else {
      deccolm(FALSE);
      sw = min_cols;
    }
    ed(2);
    cup(1, 1);
    for (row = 1; row <= max_lines; row++) {
      cup(row, 1);
      for (col = 1; col <= sw; col++)
        tprintf("%c", 'A' - 1 + row);
    }
    cup(4, 1);
    printxx("Screen accordion test (Insert & Delete Line). ");
    holdit();

    ri();
    el(2);
    decstbm(2, max_lines - 1);
    decom(TRUE);
    cup(1, 1);
    for (row = 1; row <= max_lines; row++) {
      il(row);
      dl(row);
    }
    decom(FALSE);
    decstbm(0, 0);
    cup(2, 1);
    printxx("Top line: A's, bottom line: %c's, this line, nothing more. ",
            'A' - 1 + max_lines);
    holdit();
    cup(2, 1);
    ed(0);
    cup(1, 2);
    tprintf("B");
    cub(1);
    sm("4");
    for (col = 2; col <= sw - 1; col++)
      tprintf("*");
    rm("4");
    cup(4, 1);
    printxx("Test of 'Insert Mode'. The top line should be 'A*** ... ***B'. ");
    holdit();
    ri();
    el(2);
    cup(1, 2);
    dch(sw - 2);
    cup(4, 1);
    printxx("Test of 'Delete Character'. The top line should be 'AB'. ");
    holdit();

    for (dblchr = 1; dblchr <= 2; dblchr++) {
      ed(2);
      for (row = 1; row <= max_lines; row++) {
        cup(row, 1);
        if (dblchr == 2)
          decdwl();
        for (col = 1; col <= sw / dblchr; col++)
          tprintf("%c", 'A' - 1 + row);
        cup(row, sw / dblchr - row);
        dch(row);
      }
      cup(4, 1);
      println("The right column should be staggered ");
      printxx("by one.  ");
      holdit();
    }
    ed(2);
    cup(1, 1);
    println("If your terminal has the ANSI 'Insert Character' function");
    println("(the VT102 does not), then you should see a line like this");
    println("  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z");
    println("below:");
    println("");
    for (i = 'Z'; i >= 'A'; i--) {
      tprintf("%c%c", i, BS);
      ich(2);
    }
    cup(10, 1);
    holdit();

    if (sw == max_cols)
      deccolm(FALSE);
  }
  return MENU_NOHOLD;
}

/*  Test of some known VT100 bugs and misfeatures  */

int
tst_bugs(MENU_ARGS)
{
  int i;
  /* *INDENT-OFF* */
  static MENU menutable[] = {
    { "Exit to main menu",                                   NULL },
    { "Bug A: Smooth scroll to jump scroll",                 bug_a },
    { "Bug B: Scrolling region",                             bug_b },
    { "Bug C: Wide to narrow screen",                        bug_c },
    { "Bug D: Narrow to wide screen",                        bug_d },
    { "Bug E: Cursor move from double- to single-wide line", bug_e },
    { "Bug F: Column mode escape sequence",                  bug_f },
    { "Wrap around with cursor addressing",                  bug_w },
    { "Erase right half of double width lines",              bug_l },
    { "Funny scroll regions",                                bug_s },
    /* Add more here */
    { "",                                                    NULL }
  };
  /* *INDENT-ON* */

  static const char *hmsg[] =
  {
    "Test of known bugs in the DEC VT100 series. The numbering of some of",
    "the bugs (A-F) refers to the article 'VT100 MAGIC' by Sami Tabih in",
    "the 'Proceedings of the DEC Users Society' at St. Louis, Missouri, May",
    "1983. To understand some of the tests, you have to look at the source",
    "code or the article. Of course, a good VT100-compatible terminal",
    "should not have these bugs (or have some means of disabling them)! If",
    "a bug appears, you might want to RESET the terminal before continuing",
    "the test. There is a test of the RESET function in the main menu.",
    ""};

  do {
    vt_clear(2);
    vt_move(1, 1);
    for (i = 0; *hmsg[i]; i++)
      println(hmsg[i]);
    println("");
    println("          Choose bug test number:");
  } while (menu2(menutable, i + 1));
  return MENU_NOHOLD;
}

/* Bug A: Smooth scroll to jump scroll */

int
bug_a(MENU_ARGS)
{
  int i;

  cup(10, 1);
  println("This is a test of the VT100 'Scroll while toggle softscroll'");
  println("bug.  The cursor may disappear, or move UP the screen, or");
  println("multiple copies of some lines may appear.");
  holdit();

  /*  Invoke the bug  */

  esc("[24H");  /* Simplified cursor movement   */
  decsclm(FALSE);
  for (i = 1; i <= 20; i++)
    tprintf("\n");

  decsclm(TRUE);
  for (i = 1; i <= 10; i++)
    tprintf("\n");

  decsclm(FALSE);
  for (i = 1; i <= 5; i++)
    tprintf("\n");

  /* That should be enough to show the bug. But we'll try another way:  */
  decsclm(TRUE);  /* Set soft scroll              */
  nel();        /* "NextLine", move down        */
  decsclm(FALSE);   /* Reset soft scroll            */
  nel();        /* "NextLine", move down        */
  for (i = 1; i <= 10; i++) {   /* Show the bug                 */
    printxx("Softscroll bug test, line %d.  ", i);
    holdit();
  }
  println("That should have been enough to show the bug, if present.");
  return MENU_HOLD;
}

/*  Bug B: Scrolling region  */

int
bug_b(MENU_ARGS)
{
  char c;

  decaln();

  cup(1, 1);
  el(0);
  printxx("Line 11 should be double-wide, line 12 should be cleared.");

  cup(2, 1);
  el(0);
  printxx("Then, the letters A-P should be written at the beginning");

  cup(3, 1);
  el(0);
  printxx("of lines 12-%d, and the empty line and A-E are scrolled away.", max_lines);

  cup(4, 1);
  el(0);
  printxx("If the bug is present, some lines are confused, look at K-P.");

  cup(11, 1);
  decdwl();
  decstbm(12, max_lines);

  cup(12, 1);
  el(0);
  printxx("Here we go... ");
  holdit();

  cup(12, 1);
  ri();         /* Bug comes here */
  for (c = 'A'; c <= 'P'; c++)
    tprintf("%c\n", c);   /* Bug shows here */
  holdit();
  decstbm(0, 0);  /* No scr. region */
  return MENU_NOHOLD;
}

/*  Bug C: Wide to narrow screen  */

int
bug_c(MENU_ARGS)
{
  deccolm(TRUE);  /* 132 column mode */
  cup(1, 81);
  deccolm(FALSE);   /*  80 column mode */
  cup(12, 5);
  printxx("Except for this line, the screen should be blank. ");
  return MENU_HOLD;
}

/*  Bug D: Narrow to wide screen  */

int
bug_d(MENU_ARGS)
{
  int i;
  char result;
  /* Make the bug appear */
  do {
    cup(14, 1);

    /* The original code in the article says
     * PRINT ESC$; "[13;1H"; CHR$(10%);
     * but I guess a cup(14,1); would do.
     * (To output a pure LF might be tricky).
     */

    deccolm(TRUE);  /* Make the bug visible */

    cup(1, 9);
    decdwl();
    println("You should see blinking text at the bottom line.");

    cup(3, 9);
    decdwl();
    println("Enter 0 to exit, 1 to try to invoke the bug again.");

    cup(max_lines, 9);
    decdwl();
    sgr("1;5;7");
    printxx("If you can see this then the bug did not appear.");
    sgr("");

    cup(4, 9);
    decdwl();
    result = (char) get_char();
    readnl();
    deccolm(FALSE);
  } while (result == '1');
  decsclm(TRUE);  /* Syrup scroll */
  cup(max_lines - 1, 1);
  for (i = 1; i <= 5; i++)
    println("If the bug is present, this should make things much worse!");
  holdit();
  decsclm(FALSE);   /* Jump scroll */
  return MENU_NOHOLD;
}

/*  Bug E: Cursor move from double- to single-wide line  */

int
bug_e(MENU_ARGS)
{
  int i;
  static const char *rend[2] =
  {"m", "7m"};
  deccolm(TRUE);
  cup(1, 1);
  decdwl();
  println("This test should put an 'X' at line 3 column 100.");
  for (i = 1; i <= 12; i++)
    tprintf("1234567890%s%s", csi_output(), rend[i & 1]);
  cup(1, 1);    /* The bug appears when we jump from a double-wide line */
  cup(3, 100);  /* to a single-wide line, column > 66.                  */
  printxx("X");
  cup(4, max_cols / 2);
  printxx("!                                 !");
  cup(5, 1);
  printxx("--------------------------- The 'X' should NOT be above here -");
  printxx("---+------------ but above here -----+");
  cup(10, 1);
  decdwl();
  holdit();
  deccolm(FALSE);
  return MENU_NOHOLD;
}

/*  Bug F: Column mode escape sequence  */

int
bug_f(MENU_ARGS)
{
  /*
   *  VT100 "toggle origin mode, forget rest" bug.  If you try to set
   *     (or clear) parameters and one of them is the "origin mode"
   *     ("?6") parameter, parameters that appear after the "?6"
   *     remain unaffected.  This is also true on CIT-101 terminals.
   */
  decscnm(TRUE);  /* Set reverse mode             */
  deccolm(TRUE);  /* Set 132 column mode          */
  println("Test VT100 'Toggle origin mode, forget rest' bug, part 1.");
  printxx("The screen should be in reverse, %d column mode.\n", max_cols);
  holdit();
  ed(2);
  rm("?6;5;3"); /* Reset (origin, reverse, 132 col)     */
  println("Test VT100 'Toggle origin mode, forget rest' bug, part 2.\n");
  printxx("The screen should be in non-reverse, %d column mode.\n", min_cols);
  return MENU_HOLD;
}

  /*    Bug W:
   *    The dreaded "wraparound" bug!  You CUP to col 80, write a char,
   *    CUP to another line in col 80, write a char. And the brain-damaged
   *    terminal thinks that "Hokay, so he's written a char in col 80, so
   *    I stay in col 80 and wait for next character. Let's see now, here
   *    comes another character, and I'm still in col 80, so I must make
   *    a NewLine first." -- It doesn't clear that "still in col 80" flag
   *    on a CUP. Argh!
   */

int
bug_w(MENU_ARGS)
{
  int row, col;

  cup(16, 1);
  println("   This illustrates the \"wrap around bug\" which exists on a");
  println("   standard VT100. At the top of the screen there should be");
  println("   a row of +'s, and the rightmost column should be filled");
  println("   with *'s. But if the bug is present, some of the *'s may");
  println("   be placed in other places, e.g. in the leftmost column,");
  println("   and the top line of +'s may be scrolled away.");

  cup(1, 1);
  for (col = 1; col <= min_cols - 1; col++)
    tprintf("+");
  for (row = 1; row <= max_lines; row++) {
    hvp(row, min_cols);
    tprintf("*");
  }
  cup(max_lines, 1);
  return MENU_HOLD;
}

  /*    Bug L:
   *    Check if the right half of double-width lines comes back
   *    when a line is first set to single-width, filled with stuff,
   *    set to double-width, and finally reset to single-width.
   *
   *    A VT100 has this misfeature, and many others. Foo!
   */

int
bug_l(MENU_ARGS)
{
  cup(15, 1);
  printxx("This-is-a-long-line-This-is-a-long-line-");
  printxx("This-is-a-long-line-This-is-a-long-line-");
  cup(1, 1);
  printxx("This is a test of what happens to the right half of double-width");
  println(" lines.");
  printxx("A common misfeature is that the right half does not come back");
  println(" when a long");
  printxx("single-width line is set to double-width and then reset to");
  println(" single-width.");

  cup(5, 1);
  println("Now the line below should contain 80 characters in single width.");
  holdit();
  cup(15, 1);
  decdwl();
  cup(8, 1);
  println("Now the line below should contain 40 characters in double width.");
  holdit();
  cup(15, 1);
  decswl();
  cup(11, 1);
  println("Now the line below should contain 80 characters in single width.");
  holdit();

  /* ...and in 132 column mode  */

  deccolm(TRUE);
  ed(2);
  cup(15, 1);
  printxx("This-is-a-long-line-This-is-a-long-line-");
  printxx("This-is-a-long-line-This-is-a-long-line-");
  printxx("This-is-a-long-line-This-is-a-long-line-");
  printxx("ending-here-");

  cup(1, 1);
  printxx("This is the same test in %d column mode.", max_cols);

  cup(5, 1);
  printxx("Now the line below should contain %d characters in single width.\n", max_cols);
  holdit();

  cup(15, 1);
  decdwl();

  cup(8, 1);
  printxx("Now the line below should contain %d characters in double width.\n",
          max_cols / 2);
  holdit();

  cup(15, 1);
  decswl();

  cup(11, 1);
  printxx("Now the line below should contain %d characters in single width.\n", max_cols);

  holdit();
  deccolm(FALSE);
  return MENU_NOHOLD;
}

int
bug_s(MENU_ARGS)
{
  int i;
  decstbm(20, 10);  /* 20-10=-10, < 2, so no scroll region. */
  cup(1, 1);
  for (i = 1; i <= 20; i++)
    tprintf("This is 20 lines of text (line %d), no scroll region.\n", i);
  holdit();
  ed(2);
  decstbm(0, 1);  /* Should be interpreted as decstbm(1,1) = none */
  cup(1, 1);
  for (i = 1; i <= 20; i++)
    tprintf("This is 20 lines of text (line %d), no scroll region.\n", i);
  holdit();
  decstbm(0, 0);  /* No scroll region (just in case...)   */
  return MENU_NOHOLD;
}

void
initterminal(int pn)
{
  init_ttymodes(pn);
  setup_terminal("");
}

static void
enable_iso2022(void)
{
  const char *env;

#ifdef HAVE_LANGINFO_CODESET
  (void) setlocale(LC_CTYPE, "");
  env = nl_langinfo(CODESET);
  assume_utf8 = !strcmp(env, "UTF-8");
#else
#if defined(LOCALE)
  /*
   * This is preferable to using getenv() since it ensures that we are using
   * the locale which was actually initialized by the application.
   */
  env = setlocale(LC_CTYPE, 0);
#else
  if (((env = getenv("LANG")) != 0 && *env != '\0')
      || ((env = getenv("LC_CTYPE")) != 0 && *env != '\0')
      || ((env = getenv("LC_ALL")) != 0 && *env != '\0')) {
    ;
  }
#endif
  if (env != NULL && strstr(env, ".UTF-8") != NULL) {
    assume_utf8 = TRUE;
  }
#endif

  if (assume_utf8) {
    if (LOG_ENABLED) {
      fakeio::_fprintf(log_fp, NOTE_STR "%senable ISO-2022 (%s)\n",
              allows_utf8 ? "Do not " : "",
              env);
    }
    if (!allows_utf8)
      esc("%@");
  } else {
    if (LOG_ENABLED) {
      fakeio::_fprintf(log_fp, NOTE_STR "UTF-8 is not enabled\n");
    }
  }
}

static void
disable_iso2022(void)
{
  if (assume_utf8 && !allows_utf8) {
    esc("%G");
  }
}

  /* Set up my personal prejudices      */
int
setup_terminal(MENU_ARGS)
{
  if (LOG_ENABLED)
    fakeio::_fprintf(log_fp, NOTE_STR "Setup Terminal with test-defaults\n");

  enable_iso2022();

  default_level();  /* Enter ANSI mode (if in VT52 mode)    */
  decckm(FALSE);  /* cursor keys normal   */
  deccolm(FALSE);   /* 80 col mode          */
  decsclm(FALSE);   /* Jump scroll          */
  decscnm(FALSE);   /* Normal screen        */
  decom(FALSE); /* Absolute origin mode */
  decawm(TRUE); /* Wrap around on       */
  decarm(FALSE);  /* Auto repeat off      */
  sm("?40");    /* Enable 80/132 switch (xterm) */
  rm("?45");    /* Disable reverse wrap (xterm) */
  decstbm(0, 0);  /* No scroll region     */
  sgr("0");     /* Normal character attributes  */

  disable_iso2022();

  return MENU_NOHOLD;
}

void
bye(void)
{
  /* Force my personal prejudices upon the poor luser   */
  if (LOG_ENABLED)
    fakeio::_fprintf(log_fp, NOTE_STR "Cleanup & exit\n");

  default_level();  /* Enter ANSI mode (if in VT52 mode)    */
  decckm(FALSE);  /* cursor keys normal   */
  deccolm(FALSE);   /* 80 col mode          */
  decscnm(FALSE);   /* Normal screen        */
  decom(FALSE); /* Absolute origin mode */
  decawm(TRUE); /* Wrap around on       */
  decarm(TRUE); /* Auto repeat on       */
  decstbm(0, 0);  /* No scroll region     */
  sgr("0");     /* Normal character attributes  */

  /* Say goodbye */

  vt_clear(2);
  vt_move(12, 30);
  printxx("That's all, folks!\n");
  printxx("\n\n\n");
  inflush();
  close_tty();

  if (LOG_ENABLED) {
    fakeio::_fclose(log_fp);
  }

  fakeio::_exit(EXIT_SUCCESS);
}

#ifdef UNIX
RETSIGTYPE
onbrk(SIG_ARGS GCC_UNUSED)
{
  signal(SIGINT, onbrk);
  if (reading) {
    brkrd = TRUE;
#ifdef HAVE_ALARM
    alarm(0);
#endif
  } else {
    longjmp(intrenv, 1);
  }
}

RETSIGTYPE
onterm(SIG_ARGS GCC_UNUSED)
{
  signal(SIGTERM, onterm);
  longjmp(intrenv, 1);
}
#endif

int
scanto(const char *str, int *pos, int toc)
{
  char c;
  int result = 0;
  int save = *pos;

  while (toc != (c = str[(*pos)])) {
    *pos += 1;
    if (isdigit(CharOf(c)))
      result = result * 10 + c - '0';
    else
      break;
  }
  if (c == toc) {
    *pos += 1;  /* point past delimiter */
  } else {
    result = 0;
    *pos = save;
  }
  return (result);
}

int
scan_DA(const char *str, int *pos)
{
  int value = -1;
  if (str[*pos] != '\0') {
    int save = *pos;
    value = scanto(str, pos, ';');
    if (value == 0 && *pos == save) {
      value = scanto(str, pos, 'c');
      if (str[*pos] != '\0')
        value = -1;
    }
  }
  return value;
}

int
scan_any(const char *str, int *pos, int toc)
{
  int value = 0;

  if (str[*pos] != '\0') {
    int save = *pos;

    value = scanto(str, pos, ';');
    if (value == 0 && (save == *pos)) {
      *pos = save;
      value = scanto(str, pos, toc);
      if (str[*pos] != '\0')
        value = 0;
    }
  }
  return value;
}

static const char *
push_menu(int number)
{
  const char *saved = current_menu;
  if ((current_menu = (char *) malloc(strlen(saved) + 10)) == NULL)
    no_memory();
  sprintf(current_menu, "%s%s%d", saved, *saved ? "." : "", number);
  return saved;
}

static void
pop_menu(const char *saved)
{
  if (current_menu && *current_menu)
    free(current_menu);
  current_menu = (char *) saved;
}

#define end_of_menu(table, number) \
        (table[number].description[0] == '\0')

static void
show_entry(MENU *table, int number)
{
  printxx("          %d%c %s\n", number,
          (table[number].dispatch == not_impl) ? '*' : '.',
          table[number].description);
}

static int
next_menu(const MENU *table, int top, int size)
{
  int last;
  int next = top + size;
  for (last = top; last <= next && !end_of_menu(table, last); ++last) {
    ;
  }
  return (last >= next) ? next : top;
}

static int
prev_menu(int top, int size)
{
  return (top > 1) ? (top - size) : top;
}

int
menu2(MENU *table, int top)
{
  int i, tablesize, choice;
  char c;
  char storage[BUF_SIZE];
  int pagesize = max_lines - 7 - TITLE_LINE;
  int pagetop = 1;

  tablesize = 0;
  for (i = 0; !end_of_menu(table, i); i++) {
    tablesize++;
  }
  tablesize--;

  for (;;) {
    vt_move(top, 1);
    vt_clear(0);

    println("");
    show_entry(table, 0);
    for (i = 0; i < pagesize; i++) {
      int j = pagetop + i;
      if (end_of_menu(table, j))
        break;
      show_entry(table, pagetop + i);
    }

    println("");
    printxx("          Enter choice number (0 - %d): ", tablesize);
    for (;;) {
      char *s = storage;
      int redraw = FALSE;

      inputline(s);
      choice = 0;
      while ((c = *s++) != '\0') {
        if (c == '*') {
          choice = -1;
          break;
        } else if (c == '?') {
          redraw = TRUE;
          break;
        } else if (tablesize > pagesize && c == 'n') {
          pagetop = next_menu(table, pagetop, pagesize);
          redraw = TRUE;
          break;
        } else if (tablesize > pagesize && c == 'p') {
          pagetop = prev_menu(pagetop, pagesize);
          redraw = TRUE;
          break;
        } else if (c >= '0' && c <= '9') {
          choice = 10 * choice + c - '0';
        } else {
          choice = tablesize + 1;
          break;
        }
      }

      if (redraw) {
        if (LOG_ENABLED)
          fakeio::_fprintf(log_fp, NOTE_STR "Redrawing screen\n");
        break;
      }

      if (choice < 0) {
        if (LOG_ENABLED)
          fakeio::_fprintf(log_fp, NOTE_STR "Selecting all choices\n");
        for (choice = 0; choice <= tablesize; choice++) {
          vt_clear(2);
          if (table[choice].dispatch != NULL) {
            const char *save = push_menu(choice);
            const char *name = table[choice].description;
            if (LOG_ENABLED)
              fakeio::_fprintf(log_fp, NOTE_STR "choice %s: %s\n", current_menu, name);
            if ((*table[choice].dispatch) (name) == MENU_HOLD)
              holdit();
            pop_menu(save);
          }
        }
        if (LOG_ENABLED)
          fakeio::_fflush(log_fp);
        return 1;
      } else if (choice <= tablesize) {
        vt_clear(2);
        if (table[choice].dispatch != NULL) {
          const char *save = push_menu(choice);
          const char *name = table[choice].description;
          if (LOG_ENABLED)
            fakeio::_fprintf(log_fp, NOTE_STR "choice %s: %s\n", current_menu, name);
          if ((*table[choice].dispatch) (name) != MENU_NOHOLD)
            holdit();
          pop_menu(save);
        }
        if (LOG_ENABLED)
          fakeio::_fflush(log_fp);
        return (table[choice].dispatch != NULL);
      }
      printxx("          Bad choice, try again: ");
    }
  }
}

int
menu(MENU *table)
{
  return menu2(table, 6);
}

/*
 * Format a response-string, to highlight in chrprint2() and print.  It may be
 * multi-line, depending on the length and the column limits.
 */
char *
chrformat(const char *s, int col, int first)
{
  int pass;
  int wrap = (min_cols - col);
  char *result = NULL;

  if (quick_reply) {
    const char *quicker = s;
    int j;
    for (j = 0; s[j] != '\0'; ++j) {
      int c = CharOf(s[j]);
      int d = CharOf(s[j + 1]);
      /* reset on CSI, OSC, DCS, SOS, PM, APC */
      if ((strchr("\233\235\220\230\236\237", c) != NULL)
          || (c == '\033' && d != '\0' && strchr("[]PX^_", d) != NULL)) {
        if (j != 0)
          quicker = s + j;
      } else if (c == '\n') {
        if (s[j + 1] != '\0')
          quicker = s + j + 1;
      }
    }
    s = quicker;
  }
  for (pass = 0; pass < 2; ++pass) {
    int j, k;
    int base;

    for (j = k = base = 0; s[j] != '\0'; ++j) {
      int c = CharOf(s[j]);
      char temp[80];

      if (c <= ' ' || c >= '\177') {
        sprintf(temp, "<%d> ", c);
      } else {
        sprintf(temp, "%c ", c);
      }
      if ((k + first - base) >= wrap) {
        if (pass) {
          result[k] = '\n';
        }
        base = ++k;
        first = 0;
      }
      if (pass) {
        strcpy(result + k, temp);
      }
      k += (int) strlen(temp);
    }
    if (!pass) {
      if ((result = (char*)malloc((size_t) k + 2)) == NULL)
        no_memory();
      *result = '\0';
    }
  }
  return result;
}

/*
 * Return updated row-number based on the number of characters printed to the
 * screen, e.g., for test_report_ops() to handle very long results.
 */
int
chrprint2(const char *s, int row, int col)
{
  int i;
  int result = row;
  char *temp;

  vt_hilite(TRUE);
  printxx(" ");
  temp = chrformat(s, col, 1);

  for (i = 0; temp[i] != '\0'; ++i) {
    if (temp[i] == '\n') {
      vt_move(++result, col);
    } else {
      fakeio::_putchar(temp[i]);
    }
  }
  if (quick_reply)
    vt_el(0);

  vt_hilite(FALSE);
  free(temp);

  return result + 1;
}

/*
 * Returns a pointer past the prefix, or null if no match is found
 */
char *
skip_prefix(const char *prefix, char *input)
{
  while (*prefix != '\0') {
    if (*prefix++ != *input++)
      return NULL;
  }
  return input;
}

char *
skip_csi(char *input)
{
  if (CharOf(*input) == CSI) {
    return input + 1;
  }
  return skip_prefix(csi_input(), input);
}

char *
skip_dcs(char *input)
{
  if (CharOf(*input) == DCS) {
    return input + 1;
  }
  return skip_prefix(dcs_input(), input);
}

char *
skip_osc(char *input)
{
  if (CharOf(*input) == OSC) {
    return input + 1;
  }
  return skip_prefix(osc_input(), input);
}

char *
skip_ss3(char *input)
{
  if (CharOf(*input) == SS3) {
    return input + 1;
  }
  return skip_prefix(ss3_input(), input);
}

/*
 * Variant with const params
 */
const char *
skip_prefix_2(const char *prefix, const char *input)
{
  while (*prefix != '\0') {
    if (*prefix++ != *input++)
      return NULL;
  }
  return input;
}

const char *
skip_csi_2(const char *input)
{
  if (CharOf(*input) == CSI) {
    return input + 1;
  }
  return skip_prefix_2(csi_input(), input);
}

const char *
skip_dcs_2(const char *input)
{
  if (CharOf(*input) == DCS) {
    return input + 1;
  }
  return skip_prefix_2(dcs_input(), input);
}

const char *
skip_ss3_2(const char *input)
{
  if (CharOf(*input) == SS3) {
    return input + 1;
  }
  return skip_prefix_2(ss3_input(), input);
}

/*
 * Returns a pointer past digits, or null if none are found
 */
char *
skip_digits(char *src)
{
  char *base = src;
  while (*src != '\0' && isdigit(CharOf(*src)))
    src++;
  return (base == src) ? NULL : src;
}

#define xdigitOf(c) \
        (((c) >= '0' && (c) <= '9') \
         ? ((c) - '0') \
         : (((c) >= 'A' && (c) <= 'F') \
            ? ((c) + 10 - 'A') \
            : ((c) + 10 - 'a')))

const char *
skip_xdigits(const char *src, int *value)
{
  const char *base = src;
  *value = 0;
  while (*src != '\0' && isxdigit(CharOf(*src))) {
    int ch = CharOf(*src);
    *value <<= 4;
    *value += xdigitOf(ch);
    src++;
  }
  return (base == src) ? NULL : src;
}

const char *
skip_digits_2(const char *src)
{
  const char *base = src;
  while (*src != '\0' && isdigit(CharOf(*src)))
    src++;
  return (base == src) ? NULL : src;
}

/*
 * Strip the string terminator (ST) from the given string, returning true if
 * we did this.
 */
int
strip_suffix(char *src, const char *suffix)
{
  int have = (int) strlen(src);
  int want = (int) strlen(suffix);
  if (have > want) {
    have -= want;
    if (!strcmp(src + have, suffix)) {
      src[have] = '\0';
      return TRUE;
    }
  }
  return FALSE;
}

/*
 * Strip the string terminator (ST) from the given string, returning true if
 * we did this.
 */
int
strip_terminator(char *src)
{
  int ok = strip_suffix(src, st_input());
  if (!ok) {
    int have = (int) strlen(src);
    if (have > 0 && CharOf(src[have - 1]) == ST) {
      ok = TRUE;
      src[--have] = '\0';
    }
  }
  if (!ok && LOG_ENABLED)
    fakeio::_fprintf(log_fp, NOTE_STR "Missing ST\n");
  return ok;
}

/* Parse the contents of a report from DECRQSS, returning the data as well */
int
parse_decrqss(char *report, const char *func)
{
  int code = -1;
  char *parse = report;

  if ((parse = skip_dcs(parse)) != NULL
      && strip_terminator(parse)
      && strip_suffix(parse, func)) {
    if (!strncmp(parse, "1$r", (size_t) 3))
      code = 1;
    else if (!strncmp(parse, "0$r", (size_t) 3))
      code = 0;
  }

  if (code >= 0) {
    while ((*report++ = parse[3]) != '\0')
      parse++;
  }
  return code;
}

int
title(int offset)
{
  vt_move(TITLE_LINE + offset, 10);
  if (offset == 0 && *current_menu)
    printxx("Menu %s: ", current_menu);
  return 1;     /* used for indenting */
}

static void
my_vfprintf(FILE *fp, va_list ap, const char *fmt)
{
  while (*fmt != '\0') {
    if (*fmt == '%') {
      switch (*++fmt) {
      case 'c':
        fakeio::_fputc(va_arg(ap, int), fp);
        break;
      case 'x':
        fakeio::_fprintf(fp, "%x", va_arg(ap, int));
        break;
      case 'd':
        fakeio::_fprintf(fp, "%d", va_arg(ap, int));
        break;
      case 'u':
        fakeio::_fprintf(fp, "%u", va_arg(ap, unsigned));
        break;
      case 's':
        fakeio::_fputs(va_arg(ap, char *), fp);
        break;
      }
    } else if (*fmt != '\n') {
      fakeio::_fputc(*fmt, fp);
    }
    fmt++;
  }
}

/*
 * Show a test-result, optionally logging it as well.
 */
void
show_result(const char *fmt, ...)
{
  va_list ap;

  if (*fmt != ' ')
    fakeio::_fputc(' ', stdout);
  va_start(ap, fmt);
  my_vfprintf(stdout, ap, fmt);
  va_end(ap);

  if (LOG_ENABLED) {
    fakeio::_fputs(NOTE_STR "result ", log_fp);
    va_start(ap, fmt);
    my_vfprintf(log_fp, ap, fmt);
    va_end(ap);
    fakeio::_fputc('\n', log_fp);
  }
}

/*
 * Use this to make some complex stuff (such as scrolling) slow enough to see.
 */
void
slowly(void)
{
  if (slow_motion) {
#ifdef HAVE_USLEEP
    fakeio::_fflush(stdout);
    zleep(100);
#endif
  }
}

/*
 * Bypass normal logging for control sequences that are used only to format
 * the test results.
 */
void
vt_clear(int code)
{
  log_disabled++;
  ed(code);
  log_disabled--;
}

void
vt_el(int code)
{
  log_disabled++;
  el(code);
  log_disabled--;
}

int
vt_move(int row, int col)
{
  log_disabled++;
  cup(row, col);
  log_disabled--;
  return 1;
}

void
vt_hilite(int flag)
{
  log_disabled++;
  sgr(flag ? "7" : "");
  log_disabled--;
}
