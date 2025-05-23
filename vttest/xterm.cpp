/* $Id: xterm.c,v 1.87 2024/12/05 00:43:14 tom Exp $ */

#include "vttest.h"
#include "esc.h"
#include "ttymodes.h"
#include "fakeio.h"
#define Pause(secs) fakeio::_fflush(stdout); fakeio::_sleep(secs)

static int do_decaln;

static int
toggle_color(MENU_ARGS)
{
  do_colors = !do_colors;
  return MENU_NOHOLD;
}

static int
toggle_decaln(MENU_ARGS)
{
  do_decaln = !do_decaln;
  return MENU_NOHOLD;
}

static void
check_rc(int row, int col)
{
  char *report;
  char *params;
  char expected[80];

  sprintf(expected, "%d;%dR", row, col);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);
  do_csi("6n");
  report = get_reply();
  restore_ttymodes();

  vt_move(row, 1);
  el(2);
  if ((params = skip_csi(report)) == NULL
      || strcmp(params, expected) != 0) {
    printxx("cursor save/restore %s, got \"%s\", expected \"%s\"",
            SHOW_FAILURE, params, expected);
  } else {
    printxx("cursor save/restore %s", SHOW_SUCCESS);
  }
}

static void
begin_altscreen(MENU_ARGS, const char *explain, int except_tite)
{
  vt_move(1, 1);
  println(the_title);
  vt_move(3, 1);
  println(explain);
  vt_move(4, 1);
  println("The next screen will be filled with E's down to the prompt.");
  if (except_tite) {
    vt_move(5, 1);
    println("unless titeInhibit resource is set, or alternate-screen is disabled.");
  }
  vt_move(7, 5);
}

static void
fill_altscreen(void)
{
  vt_move(12, 1);
  if (do_decaln) {
    println("This message should not be here.");
    decaln();   /* fill the screen */
  } else {
    int y, x;

    for (y = 0; y < max_lines; ++y) {
      cup(y + 1, 1);
      for (x = 0; x < min_cols; ++x)
        fakeio::_putchar('E');
    }
    vt_move(12, 1);
  }
}

static void
finish_altscreen(void)
{
  vt_move(4, 1);
  el(2);
  println("The original screen should be restored except for this line.");
  vt_move(max_lines - 2, 1);
}

static int
test_altscrn_47(MENU_ARGS)
{
  begin_altscreen(PASS_ARGS,
                  "Test private setmode 47 (to/from alternate screen)",
                  0);
  decsc();
  cup(max_lines - 2, 1);
  holdit();

  if (do_colors)
    set_colors(WHITE_ON_BLUE);

  sm("?47");
  fill_altscreen();
  vt_move(15, 7);
  decsc();
  cup(max_lines - 2, 1);
  ed(0);
  holdit();

  rm("?47");
  vt_move(max_lines - 2, 1);
  decrc();
  check_rc(7, 5);
  finish_altscreen();

  if (do_colors)
    reset_colors();

  return MENU_HOLD;
}

static int
test_altscrn_1047(MENU_ARGS)
{
  begin_altscreen(PASS_ARGS,
                  "Test private setmode 1047 (to/from alternate screen)",
                  1);
  decsc();
  vt_move(9, 7);  /* move away from the place we saved with DECSC */
  sm("?1048");  /* this saves the cursor position */
  cup(max_lines - 2, 1);
  holdit();

  if (do_colors)
    set_colors(WHITE_ON_BLUE);

  sm("?1047");
  fill_altscreen();
  vt_move(15, 7);
  decsc();
  cup(max_lines - 2, 1);
  ed(0);
  holdit();

  rm("?1047");
  vt_move(max_lines - 2, 1);
  decrc();
  rm("?1048");
  check_rc(9, 7);
  finish_altscreen();

  if (do_colors)
    reset_colors();

  return MENU_HOLD;
}

static int
test_altscrn_1049(MENU_ARGS)
{
  begin_altscreen(PASS_ARGS,
                  "Test private setmode 1049 (to/from alternate screen)",
                  1);
  decsc();
  cup(max_lines - 2, 1);
  holdit();     /* cursor location will be one line down */

  if (do_colors)
    set_colors(WHITE_ON_BLUE);

  sm("?1049");  /* this saves the cursor location */
  fill_altscreen();
  cup(max_lines - 2, 1);
  ed(0);
  holdit();

  vt_move(max_lines - 2, 1);
  rm("?1049");
  decrc();
  check_rc(max_lines - 1, 1);
  finish_altscreen();

  if (do_colors)
    reset_colors();

  return MENU_HOLD;
}

/*
 * XTerm implements an alternate screen, which is used to save the command-line
 * screen to restore it after running a full-screen application.
 *
 * The original scheme used separate save/restore-cursor and clear-screen
 * operations in conjunction with a toggle to/from the alternate screen
 * (private setmode 47).  Since not all users want the feature, xterm also
 * implements the titeInhibit resource to make it populate the $TERMCAP
 * variable without the ti/te (smcup/rmcup) strings which hold those sequences.
 * The limitation of titeInhibit is that it cannot work for terminfo, because
 * that information is read from a file rather than the environment.  I
 * implemented a corresponding set of strings for private setmode 1047 and 1048
 * to model the termcap behavior in terminfo.
 *
 * The behavior of the save/restore cursor operations still proved
 * unsatisfactory since users would (even in the original private setmode 47)
 * occasionally run shell programs from within their full-screen application
 * which would do a save-cursor to a different location, causing the final
 * restore-cursor to place the cursor in an unexpected location.  The private
 * setmode 1049 works around this by using a separate memory location to store
 * its version of the cursor location.
 */
static int
tst_altscrn(MENU_ARGS)
{
  static char txt_do_colors[80];
  static char txt_do_decaln[80];
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
    { "Exit",                                                NULL },
    { txt_do_colors,                                         toggle_color },
    { txt_do_decaln,                                         toggle_decaln },
    { "Switch to/from alternate screen (xterm)",             test_altscrn_47 },
    { "Improved alternate screen (XFree86 xterm mode 1047)", test_altscrn_1047 },
    { "Better alternate screen (XFree86 xterm mode 1049)",   test_altscrn_1049 },
    { "",                                                    NULL }
  };
  /* *INDENT-ON* */

  int save_colors = do_colors;

  do_decaln = 1;

  do {
    sprintf(txt_do_colors, "%s after switching to alternate",
            do_colors ? "Color" : "Do not color");
    sprintf(txt_do_decaln, "%s after switching to alternate",
            do_decaln ? "Use DECALN" : "Do not use DECALN");
    vt_clear(2);
    __(title(0), println("XTERM Alternate-Screen features"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));

  if (do_colors && !save_colors)
    reset_colors();
  do_colors = save_colors;

  return MENU_NOHOLD;
}

#define NUMFONTS 7

static int
tst_modify_font(MENU_ARGS)
{
  char temp[BUF_SIZE];

  vt_move(1, 1);
  println("Please enter the font name.");
  println("You can use '#' number to set fonts relative (with +/- sign) and");
  println("absolute (with a number) based on the current font's position in");
  println("the font-menu.  Examples:");
  println("    \"fixed\" to set the current font to \"fixed\"");
  println("    \"#fixed\" to set the current font to \"fixed\"");
  println("    \"#-fixed\" to set the previous font to \"fixed\"");
  println("    \"#-2 fixed\" to set the next-previous font to \"fixed\"");
  println("    \"#+fixed\" to set the following font to \"fixed\"");
  vt_move(11, 1);
  printxx(">");
  inputline(temp);
  do_osc("50;%s", temp);
  return MENU_HOLD;
}

static int
tst_report_font(MENU_ARGS)
{
  int n;
  char *report;
  int row = 1;
  int col = 1;

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  vt_move(row, col);
  println("Current font:");
  ++row;

  vt_move(row, col + 6);
  do_osc("50;?");
  report = get_reply();
  row = chrprint2(report, row, col);

  ++row;
  vt_move(row, col);
  println("Absolute fonts:");
  ++row;

  for (n = 0; n < NUMFONTS; ++n) {
    vt_move(row, col);
    do_osc("50;?%d", n);
    report = get_reply();
    if (strchr(report, ';') != NULL) {
      printxx("  %2d: ", n);
      row = chrprint2(report, row, col);
    }
  }

  ++row;

  vt_move(row, col);
  println("Relative fonts (bell may sound):");
  ++row;

  for (n = -NUMFONTS; n < NUMFONTS; ++n) {
    vt_move(row, col);
    do_osc("50;?%c%d", n >= 0 ? '+' : '-', n >= 0 ? n : -n);
    report = get_reply();
    if (strchr(report, ';') != NULL) {
      printxx("  %2d: ", n);
      row = chrprint2(report, row, col);
    } else if (n >= 0) {
      break;
    }
  }

  restore_ttymodes();
  return MENU_HOLD;
}

static int
tst_font(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
    { "Exit",                                                NULL },
    { "Modify font",                                         tst_modify_font },
    { "Report font(s)",                                      tst_report_font },
    { "",                                                    NULL }
  };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), println("XTERM Font features"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

static int
test_modify_ops(MENU_ARGS)
{
  int n;
  int wide, high;
  char temp[100];

  vt_move(1, 1);
  println("Test of Window modifying.");

  brc(2, 't');  /* iconify window */
  println("Iconify");
  Pause(2);

  brc(1, 't');  /* de-iconify window */
  println("De-Iconify");
  Pause(1);

  ed(2);
  for (n = 0; n <= 200; n += 5) {
    sprintf(temp, "Position (%d,%d)", n, n * 2);
    println(temp);
    esc("K");   /* Erase to end of line */
    brc3(3, n, n * 2, 't');
    fakeio::_fflush(stdout);
  }
  holdit();

  ed(2);
  brc3(3, 0, 0, 't');

  for (n = 0; n <= 200; n += 10) {
    wide = n + 20;
    high = n + 50;
    brc3(4, high, wide, 't');
    sprintf(temp, "%d x %d pixels", high, wide);
    println(temp);
    fakeio::_fflush(stdout);
  }
  holdit();

  ed(2);
  for (n = 0; n <= 200; n += 10) {
    high = n + 50;
    brc3(4, high, 0, 't');
    sprintf(temp, "%d x (screen-width) pixels", high);
    println(temp);
    fakeio::_fflush(stdout);
  }
  holdit();

  ed(2);
  for (n = 0; n <= 300; n += 10) {
    wide = n + 50;
    brc3(4, 0, wide, 't');
    sprintf(temp, "(screen-height) x %d pixels", wide);
    println(temp);
    fakeio::_fflush(stdout);
  }
  holdit();

  while (n >= 200) {
    wide = n + 50;
    high = 500 - n;
    brc3(4, high, wide, 't');
    sprintf(temp, "%d x %d pixels", high, wide);
    println(temp);
    fakeio::_fflush(stdout);
    n -= 5;
  }
  holdit();

  while (n <= 300) {
    wide = n + 50;
    high = 500 - n;
    brc3(4, high, wide, 't');
    sprintf(temp, "%d x %d pixels", high, wide);
    println(temp);
    fakeio::_fflush(stdout);
    n += 5;
  }
  holdit();

  ed(2);
  for (n = 5; n <= 20; n++) {
    wide = n * 4;
    high = n + 5;
    brc3(8, high, wide, 't');
    sprintf(temp, "%d x %d chars", high, wide);
    while ((int) strlen(temp) < wide - 1)
      strcat(temp, ".");
    println(temp);
    fakeio::_fflush(stdout);
  }
  holdit();

  ed(2);
  for (n = 5; n <= 24; n++) {
    high = n;
    brc3(8, high, 0, 't');
    sprintf(temp, "%d x (screen-width) chars", high);
    println(temp);
    fakeio::_fflush(stdout);
  }
  holdit();

  ed(2);
  for (n = 5; n <= 80; n++) {
    wide = n;
    brc3(8, 0, wide, 't');
    sprintf(temp, "(screen-height) x %d chars", wide);
    println(temp);
    fakeio::_fflush(stdout);
  }
  holdit();

  brc3(3, 200, 200, 't');
  brc3(8, 24, 80, 't');
  println("Reset to 24 x 80");

  ed(2);
  println("Lower");
  brc(6, 't');
  holdit();

  ed(2);
  println("Raise");
  brc(5, 't');

  /*
   * That window resizing can cause a terminal emulator to send lots of
   * SIGWINCH's, which may be queued up for delivery (and interrupt subsequent
   * system calls).  Check if we can successfully receive replies from the
   * terminal before proceeding.
   */
  set_tty_raw(TRUE);
  set_tty_echo(FALSE);
  vt_move(20, 1);
  pause_replay();
  printxx("Working");
  for (n = 0; n < 20; ++n) {
    char *report;
    char expect[20];
    printxx(".");
    fakeio::_fflush(stdout);
    do_csi("6n");
    sprintf(expect, "20;%dR", n + 9);
    report = get_reply();
    fakeio::_fflush(stdout);
    if (LOG_ENABLED) {
      fakeio::_fprintf(log_fp, NOTE_STR "expect '%s'\n", expect);
    }
    if (report != NULL
        && (report = skip_csi(report)) != NULL
        && !strcmp(report, expect)) {
      break;
    }
    if (LOG_ENABLED)
      fakeio::_fflush(log_fp);
    zleep(1000);
  }
  resume_replay();
  restore_ttymodes();
  return MENU_HOLD;
}

static int
test_report_ops(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static struct {
    int csi_or_osc;
    int pprefix;
    const char *params;
    const char *testing;
  } table[] = {
    { 0, 0, "11",   "state of window (normal/iconified)" },
    { 0, 1, "13",   "position of window in pixels" },
    { 0, 1, "13;2", "position of text-area in pixels" },
    { 0, 1, "14",   "size of text-area in pixels" },
    { 0, 1, "14;2", "size of window in pixels" },
    { 0, 1, "15",   "size of screen in pixels" },
    { 0, 1, "16",   "size of character in pixels" },
    { 0, 1, "18",   "size of window in chars" },
    { 1, 0, "20",   "icon label" },
    { 1, 0, "21",   "window label" },
  };
  /* *INDENT-ON* */

  int row = 3;
  int col = 10;
  int test = 0;

  vt_move(1, 1);
  println("Test of Window reporting.");
  fakeio::_fflush(stdout);
  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  for (test = 0; test < TABLESIZE(table); ++test) {
    char *report;
    const char *params;
    char buffer[80];

    if (row + 3 > max_lines - 3) {
      restore_ttymodes();
      vt_move(row, 1);
      holdit();
      vt_move(row = 3, 1);
      vt_clear(0);
      set_tty_raw(TRUE);
      set_tty_echo(FALSE);
    }

    vt_move(row++, 1);
    printxx("Report %s (%s):", table[test].testing, table[test].params);
    vt_move(row, col);
    do_csi("%st", table[test].params);
    report = get_reply();
    row = chrprint2(report, row, col);
    vt_move(row++, col);

    if (!strcmp(table[test].params, "11")) {
      params = skip_csi(report);
      if (params != NULL
          && isdigit(CharOf(*params))
          && !strcmp(params + 1, "t")) {
        switch (*params) {
        case '1':
          params = "normal";
          break;
        case '2':
          params = "icon";
          break;
        default:
          params = "?";
        }
      } else {
        params = "?";
      }
    } else if (table[test].csi_or_osc) {
      params = skip_osc(report);
      if (params == NULL)
        params = "?";
      else if (!strip_suffix(report, st_input()))
        params = "?";
      else if (*params == 'L')
        params = "icon label";
      else if (*params == 'l')
        params = "window label";
      else
        params = "?";
    } else {
      params = skip_csi(report);
      if (params != NULL && table[test].pprefix) {
        if (params[0] == table[test].params[1]
            && params[1] == ';'
            && strip_suffix(report, "t")) {
          int high;
          int wide;
          int skip;
          if (sscanf(params, "%d;%d;%d", &skip, &high, &wide) == 3
              && high > 0 && wide > 0) {
            sprintf(buffer, "%d high, %d wide", high, wide);
            params = buffer;
          } else {
            params = "?";
          }
        } else
          params = "?";
      } else {
        params = "? BUG";
      }
    }
    if (*params == '?')
      printxx("ERR");
    else
      printxx("OK: %s", params);
    fakeio::_fflush(stdout);
  }

  vt_move(20, 1);
  restore_ttymodes();
  return MENU_HOLD;
}

/* Set window title */
static int
test_window_name(MENU_ARGS)
{
  char temp[BUF_SIZE];

  vt_move(1, 1);
  println("Please enter the new window name.  Newer xterms may beep when setting the title.");
  inputline(temp);
  do_osc("0;%s", temp);
  return MENU_NOHOLD;
}

static int
tst_xterm_VERSION(MENU_ARGS)
{
  int step;

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  vt_move(1, 1);
  println("Both testcases should all get the same response");
  for (step = -1; step <= 0; ++step) {
    char *report;
    int row, col;

    vt_move(row = 3 + step, col = 3);
    if (step >= 0)
      do_csi(">%dq", step);
    else
      do_csi(">q");
    report = get_reply();
    if ((report = skip_dcs(report)) != NULL
        && strip_terminator(report)
        && !strncmp(report, ">|", 2)) {
      printxx(SHOW_SUCCESS);
      vt_move(row, col + 3);
      chrprint2(report + 2, row, col + 3);
    } else {
      printxx(SHOW_FAILURE);
    }
  }

  vt_move(20, 1);
  restore_ttymodes();
  return MENU_HOLD;
}

static int
tst_XTTITLEPOS(MENU_ARGS)
{
  int now, top;
  int row = 3;
  int first = -1;
  int last = -1;
  int seen = -1;
  char *report;
  char expect;
  char ignore;

  vt_move(1, 1);
  println(the_title);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  vt_move(3, 3);
  do {
    do_csi("#S");
    report = get_reply();
    if ((report = skip_csi(report)) != NULL
        && sscanf(report, "%d;%d#%c%c", &now, &top, &expect, &ignore) == 3
        && now >= seen
        && now >= 0
        && top > now
        && expect == 'S') {
      vt_move(row++, 3);
      printxx("[%d:%d] " SHOW_SUCCESS, now, top);
      if (first < 0) {
        first = now;
        seen = now;
        last = top;
      }
      seen++;
      do_csi("22;0t");
      do_osc("0;Title stack %d", seen);
      zleep(200);
    } else {
      printxx(SHOW_FAILURE);
      break;
    }
  } while (seen + 1 < last);

  while (seen-- > first) {
    do_csi("23;0t");
    do_csi("#S");
    report = get_reply();
    if ((report = skip_csi(report)) != NULL
        && sscanf(report, "%d;%d#%c%c", &now, &top, &expect, &ignore) == 3
        && now <= seen
        && now >= 0
        && top == last
        && expect == 'S') {
      vt_move(--row, 30);
      printxx("[%d:%d] " SHOW_SUCCESS, now, last);
      zleep(200);
    } else {
      printxx(SHOW_FAILURE);
      break;
    }
  }

  vt_move(20, 1);
  restore_ttymodes();
  return MENU_HOLD;
}

#define DATA(name,level) { name, #name, level }

/*
 * xterm's DECRQM codes are based on VT320/VT420/VT520, but there are a few
 * conflicting modes (adapted from rxvt), as well as xterm's extensions.
 * Those are reflected in the naming conventions.
 *
 * The names chosen here are short, to keep display alignment.
 */
#define XT_MSE_X10 9
#define XT_TOOLBAR 10
#define XT_CBLINK  12
#define XT_OPTBLNK 13
#define XT_XORBLNK 14
#define XT_SCRLBAR 30
#define XT_FONTSWT 35
#define XT_TEK4014 38
#define XT_80_132  40
#define XT_CURSES  41
#define XT_MARBELL 44
#define XT_REVWRAP 45
#define XT_LOGGING 46
#define XT_ALTSCRN 47
#define XT_MSE_X11 1000
#define XT_MSE_HL  1001
#define XT_MSE_BTN 1002
#define XT_MSE_ANY 1003
#define XT_MSE_WIN 1004
#define XT_MSE_UTF 1005
#define XT_MSE_SGR 1006
#define XT_ALTSCRL 1007
#define XT_TTY_OUT 1010
#define XT_SCRLKEY 1011
#define XT_FASTSCR 1014
#define XT_SGR_PXL 1016
#define XT_IN_8BIT 1034
#define XT_NUMLOCK 1035
#define XT_METAESC 1036
#define XT_DELTDEL 1037
#define XT_ALT_ESC 1039
#define XT_KEEPSEL 1040
#define XT_SELTCLP 1041
#define XT_BELLURG 1042
#define XT_POPBELL 1043
#define XT_OLDCLIP 1044
#define XT_ALTS_OK 1046
#define XT_ALTS_47 1047
#define XT_ALTS_SC 1048
#define XT_EXTSCRN 1049
#define XT_KT_TCAP 1050
#define XT_KT_SUN  1051
#define XT_KT_HP   1052
#define XT_KT_SCO  1053
#define XT_KT_OLD  1060
#define XT_KT_PC   1061
#define RL_BTN1    2001
#define RL_BTN2    2002
#define RL_DBTN3   2003
#define RL_BRACKET 2004
#define RL_QUOTE   2005
#define RL_LIT_NL  2006

static int
tst_xterm_DECRPM(MENU_ARGS)
{
  /* *INDENT-OFF* */
  RQM_DATA dec_modes[] = { /* this list is sorted by code, not name */
    DATA( DECCKM,     3 /* cursor keys */),
    DATA( DECANM,     3 /* ANSI */),
    DATA( DECCOLM,    3 /* column */),
    DATA( DECSCLM,    3 /* scrolling */),
    DATA( DECSCNM,    3 /* screen */),
    DATA( DECOM,      3 /* origin */),
    DATA( DECAWM,     3 /* autowrap */),
    DATA( DECARM,     3 /* autorepeat */),
    DATA( XT_MSE_X10, 3 /* X10 mouse */),
    DATA( XT_TOOLBAR, 3 /* rxvt toolbar vs DECEDM */),
    DATA( DECLTM,     3 /* line transmit */),
    DATA( XT_CBLINK,  3 /* att610: Start/stop blinking cursor */),
    DATA( XT_OPTBLNK, 3 /* blink-option via menu/resource */),
    DATA( XT_XORBLNK, 3 /* enable XOR or blinking cursoor control */),
    DATA( DECEKEM,    3 /* edit key execution */),
    DATA( DECPFF,     3 /* print form feed */),
    DATA( DECPEX,     3 /* printer extent */),
    DATA( DECTCEM,    3 /* text cursor enable */),
    DATA( XT_SCRLBAR, 3 /* rxvt scrollbar */),
    DATA( DECRLM,     5 /* left-to-right */),
    DATA( XT_FONTSWT, 3 /* rxvt font-switching vs DECTEK */),
    DATA( XT_TEK4014, 3 /* Tektronix 4014 */),
    DATA( DECHEM,     5 /* Hebrew encoding */),
    DATA( XT_80_132,  3 /* 80/132 mode */),
    DATA( XT_CURSES,  3 /* curses hack */),
    DATA( DECNRCM,    3 /* national replacement character set */),
    DATA( DECGEPM,    3 /* graphics expanded print */),
    DATA( XT_MARBELL, 3 /* margin bell vs DECGPCM */),
    DATA( XT_REVWRAP, 3 /* reverse-wrap vs DECGPCS */),
    DATA( XT_LOGGING, 3 /* logging vs DECGPBM */),
    DATA( XT_ALTSCRN, 3 /* alternate screen vs DECGRPM */),
    DATA( DEC131TM,   3 /* VT131 transmit */),
    DATA( DECNAKB,    5 /* Greek/N-A Keyboard Mapping */),
    DATA( DECHCCM,    3 /* horizontal cursor coupling (disabled) */),
    DATA( DECVCCM,    3 /* vertical cursor coupling */),
    DATA( DECPCCM,    3 /* page cursor coupling */),
    DATA( DECNKM,     3 /* numeric keypad */),
    DATA( DECBKM,     3 /* backarrow key */),
    DATA( DECKBUM,    3 /* keyboard usage */),
    DATA( DECLRMM,    4 /* left/right margin mode (VT420) */),
    DATA( DECXRLM,    3 /* transmit rate linking */),
    DATA( DECKPM,     4 /* keyboard positioning */),
    DATA( DECNCSM,    5 /* no clearing screen on column change */),
    DATA( DECRLCM,    5 /* right-to-left copy */),
    DATA( DECCRTSM,   5 /* CRT save */),
    DATA( DECARSM,    5 /* auto resize */),
    DATA( DECMCM,     5 /* modem control */),
    DATA( DECAAM,     5 /* auto answerback */),
    DATA( DECCANSM,   5 /* conceal answerback */),
    DATA( DECNULM,    5 /* null */),
    DATA( DECHDPXM,   5 /* half duplex */),
    DATA( DECESKM,    5 /* enable secondary keyboard language */),
    DATA( DECOSCNM,   5 /* overscan */),
    DATA( DECFWM,     5 /* framed windows */),
    DATA( DECRPL,     5 /* review previous lines */),
    DATA( DECHWUM,    5 /* host wake-up mode (CRT and energy saver) */),
    DATA( DECATCUM,   5 /* alternate text color underline */),
    DATA( DECATCBM,   5 /* alternate text color blink */),
    DATA( DECBBSM,    5 /* bold and blink style */),
    DATA( DECECM,     5 /* erase color */),
    DATA( XT_MSE_X11, 3 /* VT200 mouse */),
    DATA( XT_MSE_HL,  3 /* VT200 highlight mouse */),
    DATA( XT_MSE_BTN, 3 /* button-event mouse */),
    DATA( XT_MSE_ANY, 3 /* any-event mouse */),
    DATA( XT_MSE_WIN, 3 /* focus-event mouse */),
    DATA( XT_MSE_UTF, 3 /* UTF-8 extended mouse-coordinates */),
    DATA( XT_MSE_SGR, 3 /* SGR-style extended mouse-coordinates */),
    DATA( XT_ALTSCRL, 3 /* alternate-scroll */),
    DATA( XT_TTY_OUT, 3 /* rxvt scroll tty output */),
    DATA( XT_SCRLKEY, 3 /* rxvt scroll key */),
    DATA( XT_FASTSCR, 3 /* fast-scroll mode */),
    DATA( XT_SGR_PXL, 3 /* SGR mouse pixel mode */),
    DATA( XT_IN_8BIT, 3 /* input eight bits */),
    DATA( XT_NUMLOCK, 3 /* real num lock */),
    DATA( XT_METAESC, 3 /* meta sends escape */),
    DATA( XT_DELTDEL, 3 /* delete is del */),
    DATA( XT_ALT_ESC, 3 /* alt sends escape */),
    DATA( XT_KEEPSEL, 3 /* keep selection */),
    DATA( XT_SELTCLP, 3 /* select to clipboard */),
    DATA( XT_BELLURG, 3 /* bell is urgent */),
    DATA( XT_POPBELL, 3 /* pop on bell */),
    DATA( XT_ALTS_OK, 3 /* enable alt-screen switching */),
    DATA( XT_ALTS_47, 3 /* first extended alt-screen */),
    DATA( XT_ALTS_SC, 3 /* save cursor for first extended alt-screen */),
    DATA( XT_EXTSCRN, 3 /* second extended alt-screen */),
    DATA( RL_BTN1,    3 /* click1 emit Esc seq to move point*/),
    DATA( RL_BTN2,    3 /* press2 emit Esc seq to move point*/),
    DATA( RL_DBTN3,   3 /* Double click-3 deletes */),
    DATA( RL_BRACKET, 3 /* Surround paste by escapes */),
    DATA( RL_QUOTE,   3 /* Quote each char during paste */),
    DATA( RL_LIT_NL,  3 /* Paste "\n" as C-j */),
  };
  /* *INDENT-ON* */

  int old_DECRPM = set_DECRPM(3);
  int code = any_RQM(PASS_ARGS, dec_modes, TABLESIZE(dec_modes), 1);

  set_DECRPM(old_DECRPM);
  return code;
}

static int
rpt_XTQMODKEYS(MENU_ARGS)
{
  int n;
  char full_title[80];
  char command[80];
  static const char *resource[] =
  {
    "modifyKeyboard",
    "modifyCursorKeys",
    "modifyFunctionKeys",
    "modifyOtherKeys",
  };
  for (n = 0; n < 4; ++n) {
    int code = (n == 3) ? 4 : n;
    sprintf(full_title, "%s (%s)", the_title, resource[n]);
    sprintf(command, ">%dm", code);
    vt_move(1, 1);
    vt_clear(0);
    any_decrqss(full_title, command);
    holdit();
  }
  return MENU_NOHOLD;
}

static int
rpt_XTSMTITLE(MENU_ARGS)
{
  int n;
  static const char *message[] =
  {
    "Set window/icon labels using hexadecimal",
    "Query window/icon labels using hexadecimal",
    "Set window/icon labels using UTF-8",
    "Query window/icon labels using UTF-8"
  };

  for (n = 0; n < 4; ++n) {
    char full_title[80];
    char command[80];

    sprintf(full_title, "%s (%s)", the_title, message[n]);
    vt_move(1, 1);
    vt_clear(0);
    sprintf(command, ">%dt", n);
    /* set the value */
    do_csi(">%dt", n);
    any_decrqss(full_title, command);
    zleep(500);
    /* reset the value */
    do_csi(">%dT", n);
    any_decrqss(full_title, command);
    holdit();
  }
  return MENU_NOHOLD;
}

static int
tst_xterm_DECRQSS(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test VT520 features (DECRQSS)",                     tst_vt520_DECRQSS },
      { "Query Key Modifier Options",                        rpt_XTQMODKEYS },
      { "Query Title Modes",                                 rpt_XTSMTITLE },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("XTerm Status-Strings Reports"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/*
 * Show mouse-modes, offered as an option in the mouse test-screens (since that
 * is really where these can be tested).
 */
void
show_mousemodes(void)
{
  /* *INDENT-OFF* */
  RQM_DATA mouse_modes[] = { /* this list is sorted by code, not name */
    DATA( XT_MSE_X10, 3 /* X10 mouse */),
    DATA( XT_MSE_X11, 3 /* VT200 mouse */),
    DATA( XT_MSE_HL,  3 /* VT200 highlight mouse */),
    DATA( XT_MSE_BTN, 3 /* button-event mouse */),
    DATA( XT_MSE_ANY, 3 /* any-event mouse */),
    DATA( XT_MSE_WIN, 3 /* focus-event mouse */),
    DATA( XT_MSE_UTF, 3 /* UTF-8 extended mouse-coordinates */),
    DATA( XT_MSE_SGR, 3 /* SGR-style extended mouse-coordinates */),
  };
  /* *INDENT-ON* */

  int old_DECRPM = set_DECRPM(3);
  vt_clear(2);
  (void) any_RQM("mouse modes", mouse_modes, TABLESIZE(mouse_modes), 1);
  set_DECRPM(old_DECRPM);
  holdit();
}

#undef DATA
static int
tst_xterm_reports(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
    { "Exit",                                                NULL },
    { "Test VT520 features",                                 tst_vt520_reports },
    { "Report version (XTVERSION)",                          tst_xterm_VERSION },
    { "Request Mode (DECRQM)/Report Mode (DECRPM)",          tst_xterm_DECRPM },
    { "Status-String Report (DECRQSS)",                      tst_xterm_DECRQSS },
    { "Title-Stack Position",                                tst_XTTITLEPOS },
    { "",                                                    NULL }
  };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), println("XTERM miscellaneous reports"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/*
 * xterm (and derived programs such as hpterm, dtterm, rxvt) are the most
 * widely used vt100 near-compatible terminal emulators (other than modem
 * programs).  dtterm emulates a vt220, as does XFree86 xterm.  DECterm
 * emulates a vt320.
 */
int
tst_xterm(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
    { "Exit",                                                NULL },
    { "Test VT520 features",                                 tst_vt520 },
    { "Test reporting functions",                            tst_xterm_reports },
    { "Set window title",                                    test_window_name },
    { "Font features",                                       tst_font },
    { "Mouse features",                                      tst_mouse },
    { "Tektronix 4014 features",                             tst_tek4014 },
    { "Alternate-Screen features (xterm)",                   tst_altscrn },
    { "Window modify-operations (dtterm)",                   test_modify_ops },
    { "Window report-operations (dtterm)",                   test_report_ops },
    { "",                                                    NULL }
  };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), println("XTERM special features"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}
