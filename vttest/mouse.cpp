/* $Id: mouse.c,v 1.47 2024/12/05 00:37:39 tom Exp $ */

#include "vttest.h"
#include "esc.h"
#include "ttymodes.h"
#include "fakeio.h"
#define MCHR(c) (unsigned)((unsigned)((c) - ' ') & 0xff)

#define isQuit(c)   (((c) == 'q') || ((c) == 'Q'))
#define isReport(c) (get_level() >= 3 && (((c) == 'r') || ((c) == 'R')))
#define isClear(c)  ((c) == ' ')

#define ToData(n)  vt_move(report_row = 4 + n, report_col = 10)

typedef enum {
  cDFT = 0,
  cUTF = 1005,
  cSGR = 1006,
  cURX = 1015,
  cSGRp = 1016
} COORDS;

static int do_ExtCoords;
static int do_ExtPixels;
static int do_FocusEvent;

static int chars_high;
static int chars_wide;

static int pixels_high;
static int pixels_wide;
static int pixels_xpos;
static int pixels_ypos;
static int pixels_xchr;
static int pixels_ychr;

/*
 * If the terminal responds to the window-size reports, use that to determine
 * the scaling for pixel-coordinates.
 */
static int
can_do_pixels(void)
{
  static int first = TRUE;
  int result = FALSE;
  if (first) {
    char *report;
    int reply;
    char final;
    char extra;

    first = FALSE;
    result = TRUE;

    set_tty_raw(TRUE);
    set_tty_echo(FALSE);

    brc(13, 't');
    report = get_reply();
    if ((report = skip_csi(report)) == NULL
        || sscanf(report,
                  "%d;%d;%d%c%c",
                  &reply,
                  &pixels_ypos,
                  &pixels_xpos,
                  &final,
                  &extra) != 4
        || reply != 3
        || pixels_ypos <= 0
        || pixels_xpos <= 0
        || final != 't') {
      result = FALSE;
    } else {
      brc(14, 't');
      report = get_reply();
      if ((report = skip_csi(report)) == NULL
          || sscanf(report,
                    "%d;%d;%d%c%c",
                    &reply,
                    &pixels_high,
                    &pixels_wide,
                    &final,
                    &extra) != 4
          || reply != 4
          || pixels_high <= 0
          || pixels_wide <= 0
          || final != 't') {
        result = FALSE;
      } else {
        brc(16, 't');
        report = get_reply();
        if ((report = skip_csi(report)) == NULL
            || sscanf(report,
                      "%d;%d;%d%c%c",
                      &reply,
                      &pixels_ychr,
                      &pixels_xchr,
                      &final,
                      &extra) != 4
            || reply != 6
            || pixels_ychr <= 0
            || pixels_xchr <= 0
            || final != 't') {
          result = FALSE;
        }
      }
    }

    restore_ttymodes();

    if (result && LOG_ENABLED) {
      fakeio::_fprintf(log_fp, NOTE_STR "screen %dx%d at %d,%d (cell %dx%d)\n",
              pixels_high, pixels_wide,
              pixels_ypos, pixels_xpos,
              pixels_ychr, pixels_xchr);
    }
  }
  return result;
}

static const char *
nameOfExtCoords(int code)
{
  const char *result;

  switch (code) {
  case cUTF:
    result = "UTF-8";
    break;
  case cSGR:
    result = "SGR";
    break;
  case cURX:
    result = "urxvt-style";
    break;
  case cSGRp:
    result = "SGR/pixels";
    break;
  default:
    result = "normal";
    break;
  }
  return result;
}

static void
show_mousehelp(void)
{
  if (get_level() >= 3)
    println("Press 'q' to quit, 'r' to report modes, ' ' to clear.");
  else
    println("Press 'q' to quit, ' ' to clear.");
}

static unsigned
xterm_coord(char *source, int *pos)
{
  unsigned result;

  switch (do_ExtCoords) {
  case cUTF:
    {
      int used;
      const char *real_src = source + *pos;
      unsigned limit = (unsigned) strlen(real_src);

      used = conv_to_utf32((unsigned *) 0, real_src, limit);
      if (used > 0) {
        (void) conv_to_utf32(&result, real_src, limit);
        *pos += used;
        if (result > ' ')
          result -= ' ';
        else
          result = 0;
      } else {
        result = 0;
      }
    }
    break;
  case cSGR:
  case cSGRp:
    result = 0;
    break;
  case cURX:
    result = 0;
    break;
  default:
    {
      result = MCHR(source[*pos]);
      *pos += 1;
    }
    break;
  }
  return result;
}

static unsigned
sgr_param(char **report, int final, unsigned offset)
{
  unsigned result = 0;

  if (*report != NULL) {
    char *base = *report;
    char *endp = NULL;
    long value = strtol(base, &endp, 10);

    if (value >= (long) offset
        && (endp == NULL
            || (*endp == 0 || *endp == ';' || *endp == final))) {
      result = (unsigned) value - offset;
      if (endp != NULL) {
        if (*endp == ';')
          ++endp;
        *report = endp;
      } else {
        *report = base + strlen(base);
      }
    } else {
      *report = NULL;
      result = offset;
    }
  }

  return result;
}

static char *
skip_params(char *report)
{
  return report + strspn(report, "0123456789;");
}

/*
 * Parse the mouse position report.  This is the usual case, containing the 'M'
 * response (as well as variations on it).
 */
static char *
parse_mouse_M(char *report, unsigned *b, unsigned *x, unsigned *y)
{
  char *result = NULL;

  if ((report = skip_csi(report)) != NULL) {
    char *finalp;

    switch (do_ExtCoords) {
    default:
    case cUTF:
      if (*report == 'M'
          && strlen(report) >= 4) {
        int pos = 2;
        *b = MCHR(report[1]);
        *x = xterm_coord(report, &pos);
        *y = xterm_coord(report, &pos);
        result = report + pos;
      }
      break;

    case cSGR:
    case cSGRp:
      if (*report++ == '<') {
        finalp = skip_params(report);
        if (*finalp == 'M') {
          /* pressed */
          *b = sgr_param(&report, 'M', 0);
          *x = sgr_param(&report, 'M', 0);
          *y = sgr_param(&report, 'M', 0);
          result = ++finalp;
        } else if (*finalp == 'm') {
          /* released */
          *b = sgr_param(&report, 'm', 0);
          *x = sgr_param(&report, 'm', 0);
          *y = sgr_param(&report, 'm', 0);
          result = ++finalp;
        }
      }
      break;

    case cURX:
      finalp = skip_params(report);
      if (*finalp == 'M') {
        *b = sgr_param(&report, 'M', 32);
        *x = sgr_param(&report, 'M', 0);
        *y = sgr_param(&report, 'M', 0);
        result = ++finalp;
      }
      break;
    }
  }

  return result;
}

/*
 * Parse the mouse report, looking for the 'T' response, which is part of
 * mouse highlight-tracking.
 */
static char *
parse_mouse_T(char *report,
              unsigned *start_x,
              unsigned *start_y,
              unsigned *end_x,
              unsigned *end_y,
              unsigned *mouse_x,
              unsigned *mouse_y)
{
  char *result = NULL;

  if ((report = skip_csi(report)) != NULL) {
    char *finalp;

    switch (do_ExtCoords) {
    default:
    case cUTF:
      if (*report == 'M'
          && strlen(report) >= 7) {
        int pos = 1;
        *start_x = xterm_coord(report, &pos);
        *start_y = xterm_coord(report, &pos);
        *end_x = xterm_coord(report, &pos);
        *end_y = xterm_coord(report, &pos);
        *mouse_x = xterm_coord(report, &pos);
        *mouse_y = xterm_coord(report, &pos);
        result = report + pos;
      }
      break;

    case cSGR:
    case cSGRp:
      if (*report++ != '<')
        break;
      /* FALLTHRU */

    case cURX:
      finalp = skip_params(report);
      if (*finalp == 'T') {
        *start_x = sgr_param(&report, 'T', 0);
        *start_y = sgr_param(&report, 'T', 0);
        *end_x = sgr_param(&report, 'T', 0);
        *end_y = sgr_param(&report, 'T', 0);
        *mouse_x = sgr_param(&report, 'T', 0);
        *mouse_y = sgr_param(&report, 'T', 0);
        result = ++finalp;
      }
      break;
    }
  }

  return result;
}

/*
 * Parse the mouse report, looking for the 't' response, which is part of mouse
 * highlight-tracking.
 */
static char *
parse_mouse_t(char *report, unsigned *x, unsigned *y)
{
  char *result = NULL;

  if ((report = skip_csi(report)) != NULL) {
    char *finalp;

    switch (do_ExtCoords) {
    default:
    case cUTF:
      if (*report == 't'
          && strlen(report) >= 3) {
        int pos = 1;
        *x = xterm_coord(report, &pos);
        *y = xterm_coord(report, &pos);
      }
      break;

    case cSGR:
    case cSGRp:
      if (*report++ != '<')
        break;
      /* FALLTHRU */

    case cURX:
      finalp = skip_params(report);
      if (*finalp == 't') {
        *x = sgr_param(&report, 't', 0);
        *y = sgr_param(&report, 't', 0);
        result = ++finalp;
      }
      break;
    }
  }

  return result;
}

static void
cat_button(char *dst, const char *src)
{
  if (*dst != '\0')
    strcat(dst, ", ");
  strcat(dst, src);
}

static char *
locator_button(unsigned b)
{
  static char result[80];

  if (b) {
    result[0] = 0;
    if (b & 1)
      cat_button(result, "right");
    if (b & 2)
      cat_button(result, "middle");
    if (b & 4)
      cat_button(result, "left");
    if (b & 8)
      cat_button(result, "M4");
  } else {
    strcpy(result, "no buttons down");
  }
  return result;
}

static const char *
locator_event(int e)
{
  const char *result;
  switch (e) {
  case 0:
    result = "locator unavailable";
    break;
  case 1:
    result = "request - received a DECRQLP";
    break;
  case 2:
    result = "left button down";
    break;
  case 3:
    result = "left button up";
    break;
  case 4:
    result = "middle button down";
    break;
  case 5:
    result = "middle button up";
    break;
  case 6:
    result = "right button down";
    break;
  case 7:
    result = "right button up";
    break;
  case 8:
    result = "M4 button down";
    break;
  case 9:
    result = "M4 button up";
    break;
  case 10:
    result = "locator outside filter rectangle";
    break;
  default:
    result = "unknown event";
    break;
  }
  return result;
}

static void
show_click(unsigned y, unsigned x, int c)
{
  if (do_ExtPixels) {
    y = (y + (unsigned) pixels_ychr - 1) / (unsigned) pixels_ychr;
    x = (x + (unsigned) pixels_xchr - 1) / (unsigned) pixels_xchr;
  }
  cup((int) y, (int) x);
  fakeio::_putchar(c);
  vt_move((int) y, (int) x);
  fakeio::_fflush(stdout);
}

/* Print the corners of the highlight-region.  Note that xterm doesn't use
 * the last row.
 */
static void
show_hilite(int first, int last)
{
  /* *INDENT-OFF* */
  vt_move(first, 1);          tprintf("+");
  vt_move(last-1,  1);        tprintf("+");
  vt_move(first, min_cols);   tprintf("+");
  vt_move(last-1,  min_cols); tprintf("+");
  /* *INDENT-ON* */

  fakeio::_fflush(stdout);
}

static void
show_locator_rectangle(void)
{
  const int first = 10;
  const int last = 20;

  decefr(first, 1, last, min_cols);
  show_hilite(first, last);
}

#define SCALED(value,range) \
      ((value * (unsigned) range + (unsigned) (range - 1)) / (unsigned) range)

static int
show_locator_report(char *report, int row, int pixels)
{
  int Pe, Pb, Pp;
  unsigned Pr, Pc;
  int now = row;
  int report_row, report_col;

  ToData(0);
  vt_el(2);
  chrprint2(report, report_row, report_col);
  while ((report = skip_csi(report)) != NULL
         && (sscanf(report,
                    "%d;%d;%u;%u&w", &Pe, &Pb, &Pr, &Pc) == 4
             || sscanf(report,
                       "%d;%d;%u;%u;%d&w", &Pe, &Pb, &Pr, &Pc, &Pp) == 5)) {
    vt_move(row, 10);
    vt_el(2);
    show_result("%s - %s (%d,%d)",
                locator_event(Pe),
                locator_button((unsigned) Pb),
                Pr, Pc);
    vt_el(0);
    if (pixels) {
      if (pixels_high > 0 && pixels_wide > 0) {
        Pr = SCALED(Pr, pixels_high);
        Pc = SCALED(Pc, pixels_wide);
        show_click(Pr, Pc, '*');
      }
    } else {
      show_click(Pr, Pc, '*');
    }
    report = strchr(report, '&') + 2;
    now = row++;
  }
  return now;
}

static int
get_screensize(MENU_ARGS)
{
  char *report;
  char tmp = 0;

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  brc(14, 't'); /* report window's pixel-size */
  report = get_reply();
  if ((report = skip_csi(report)) == NULL
      || sscanf(report, "4;%d;%d%c", &pixels_high, &pixels_wide, &tmp) != 3
      || tmp != 't'
      || pixels_high <= 0
      || pixels_wide <= 0) {
    pixels_high = -1;
    pixels_wide = -1;
  }

  brc(18, 't'); /* report window's char-size */
  report = get_reply();
  if ((report = skip_csi(report)) == NULL
      || sscanf(report, "8;%d;%d%c", &chars_high, &chars_wide, &tmp) != 3
      || tmp != 't'
      || chars_high <= 0
      || chars_wide <= 0) {
    chars_high = 24;
    chars_wide = 80;
  }

  restore_ttymodes();
  return MENU_NOHOLD;
}

static void
show_dec_locator_events(MENU_ARGS, int mode, int pixels)
{
  int row, now;

  pause_replay();
loop:
  vt_move(1, 1);
  ed(0);
  println(the_title);
  show_mousehelp();
  println("Mouse events will be marked with '*'");

  decelr((mode > 0) ? mode : ((mode == 0) ? 2 : -mode), pixels ? 1 : 2);

  if (mode < 0)
    show_locator_rectangle();
  else if (mode == 0)
    do_csi("'w");   /* see decefr() */

  decsle(1);    /* report button-down events */
  decsle(3);    /* report button-up events */
  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  now = 4;
  for (;;) {
    char *report = instr();
    if (isQuit(*report)) {
      decrqlp(1);
      report = instr();
      show_locator_report(report, now + 1, pixels);
      break;
    } else if (isReport(*report)) {
      show_mousemodes();
      goto loop;
    } else if (isClear(*report)) {
      goto loop;
    }
    row = 4;
    while (now > row) {
      vt_move(now, 1);
      vt_el(2);
      now--;
    }
    now = show_locator_report(report, row, pixels);
    if (mode == 0) {
      decelr(2, pixels ? 1 : 2);
      do_csi("'w");   /* see decefr() */
    }
  }
  resume_replay();

  decelr(0, 0);
  restore_ttymodes();

  vt_move(max_lines - 2, 1);
}

/* Normal Mouse Tracking */
static void
show_mouse_tracking(MENU_ARGS, const char *the_mode)
{
  unsigned y = 0, x = 0;
  unsigned b, xx, yy;
  int report_row, report_col;

loop:
  vt_move(1, 1);
  ed(0);
  println(the_title);
  show_mousehelp();
  println("Mouse events will be marked with the button number.");

  sm(the_mode);
  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  pause_replay();
  for (;;) {
    char *report = instr();

    if (isQuit(*report)) {
      break;
    } else if (isReport(*report)) {
      show_mousemodes();
      goto loop;
    } else if (isClear(*report)) {
      goto loop;
    }

    ToData(0);
    chrprint2(report, report_row, report_col);

    while ((report = parse_mouse_M(report, &b, &xx, &yy)) != NULL) {
      unsigned adj = 1;
      char result[80];
      char modifiers[80];

      sprintf(result, "code 0x%x (%u,%u)", b, yy, xx);
      *modifiers = '\0';
      if (b & (unsigned) (~3)) {
        if (b & 4)
          strcat(modifiers, " shift");
        if (b & 8)
          strcat(modifiers, " meta");
        if (b & 16)
          strcat(modifiers, " control");
        if (b & 32)
          strcat(modifiers, " motion");
        if (b & 64)
          adj += 3;
        b &= 3;
      }
      if ((b == 3) && (adj == 1)) {
        if (xx != x || yy != y) {
          ToData(1);
          vt_el(2);
          show_result("%s release%s", result, modifiers);
          show_click(yy, xx, '*');
        }
      } else {
        b += adj;
        ToData(1);
        vt_el(2);
        show_result("%s button %u%s", result, b, modifiers);
        show_click(yy, xx, (int) (b + '0'));
      }
      x = xx;
      y = yy;
      report += 4;
    }
  }
  resume_replay();

  rm(the_mode);
  fakeio::_fflush(stdout);
  restore_ttymodes();

  vt_move(max_lines - 2, 1);
}

static int
test_dec_locator_event(MENU_ARGS)
{
  show_dec_locator_events(PASS_ARGS, 2, FALSE);
  return MENU_HOLD;
}

static int
test_dec_locator_events(MENU_ARGS)
{
  show_dec_locator_events(PASS_ARGS, 1, FALSE);
  return MENU_HOLD;
}

static int
test_dec_locator_event_p(MENU_ARGS)
{
  show_dec_locator_events(PASS_ARGS, 2, TRUE);
  return MENU_HOLD;
}

static int
test_dec_locator_events_p(MENU_ARGS)
{
  show_dec_locator_events(PASS_ARGS, 1, TRUE);
  return MENU_HOLD;
}

static int
test_dec_locator_rectangle(MENU_ARGS)
{
  show_dec_locator_events(PASS_ARGS, -2, FALSE);
  return MENU_HOLD;
}

static int
test_dec_locator_unfiltered(MENU_ARGS)
{
  show_dec_locator_events(PASS_ARGS, 0, FALSE);
  return MENU_HOLD;
}

/* Any-Event Mouse Tracking */
static int
test_mouse_any_event(MENU_ARGS)
{
  show_mouse_tracking(PASS_ARGS, "?1003");
  return MENU_HOLD;
}

/* Button-Event Mouse Tracking */
static int
test_mouse_button_event(MENU_ARGS)
{
  show_mouse_tracking(PASS_ARGS, "?1002");
  return MENU_HOLD;
}

/* Mouse Highlight Tracking */
static int
test_mouse_hilite(MENU_ARGS)
{
  const int first = 10;
  const int last = 20;
  unsigned b;
  unsigned y = 0, x = 0;
  unsigned start_x, end_x;
  unsigned start_y, end_y;
  unsigned mouse_y, mouse_x;
  int report_row, report_col;

loop:
  vt_move(1, 1);
  ed(0);
  println(the_title);
  show_mousehelp();
  println("Mouse events will be marked with the button number.");
  printxx("Highlighting range is [%d..%d)\n", first, last);
  show_hilite(first, last);

  sm("?1001");
  fakeio::_fflush(stdout);
  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  pause_replay();
  for (;;) {
    char *report = instr();

    if (isQuit(*report)) {
      break;
    } else if (isReport(*report)) {
      show_mousemodes();
      goto loop;
    } else if (isClear(*report)) {
      goto loop;
    }

    show_hilite(first, last);
    ToData(1);
    vt_el(2);
    chrprint2(report, report_row, report_col);

    if (parse_mouse_M(report, &b, &x, &y) != NULL) {
      b &= 7;
      if (b != 3) {
        /* send the xterm the highlighting range (it MUST be done first) */
        do_csi("1;%u;%u;%d;%d;T", x, y, 10, 20);
        /* now, show the mouse-click */
        if (b < 3)
          b++;
        show_click(y, x, (int) (b + '0'));
      }
      /* interpret the event */
      ToData(2);
      vt_el(2);
      show_result("tracking: code 0x%x (%d,%d)", b, y, x);
      fakeio::_fflush(stdout);
    } else if (parse_mouse_T(report,
                             &start_x, &start_y,
                             &end_x, &end_y,
                             &mouse_x, &mouse_y)) {
      /* interpret the event */
      ToData(2);
      vt_el(2);

      show_result("done: start(%d,%d), end(%d,%d), mouse(%d,%d)",
                  start_y, start_x,
                  end_y, end_x,
                  mouse_y, mouse_x);
      if (start_y != y
          || start_x != x)
        show_click(start_y, start_x, 's');
      if (end_y != y
          || end_x != x)
        show_click(end_y, end_x, 'e');
      if (mouse_y != y
          || mouse_x != x)
        show_click(mouse_y, mouse_x, 'm');
    } else if (parse_mouse_t(report, &end_x, &end_y)) {
      /* interpret the event */
      ToData(2);
      vt_el(2);

      show_result("done: end(%d,%d)",
                  end_y, end_x);
      if (end_y != y
          || end_x != x)
        show_click(end_y, end_x, 'e');
    }
  }
  resume_replay();

  rm("?1001");
  fakeio::_fflush(stdout);
  restore_ttymodes();

  vt_move(max_lines - 2, 1);
  return MENU_HOLD;
}

/* Normal Mouse Tracking */
static int
test_mouse_normal(MENU_ARGS)
{
  show_mouse_tracking(PASS_ARGS, "?1000");
  return MENU_HOLD;
}

/* X10 Mouse Compatibility */
static int
test_X10_mouse(MENU_ARGS)
{
  unsigned b, x, y;
  int report_row, report_col;

  pause_replay();
loop:
  vt_move(1, 1);
  ed(0);
  println(the_title);
  show_mousehelp();
  println("Mouse events will be marked with the button number.");

  sm("?9");
  fakeio::_fflush(stdout);
  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  for (;;) {
    char *report = instr();
    if (isQuit(*report)) {
      break;
    } else if (isReport(*report)) {
      show_mousemodes();
      goto loop;
    } else if (isClear(*report)) {
      goto loop;
    }
    ToData(0);
    vt_el(2);
    chrprint2(report, report_row, report_col);
    if (parse_mouse_M(report, &b, &x, &y) != NULL) {
      cup((int) y, (int) x);
      tprintf("%u", b + 1);
      vt_move((int) y, (int) x);
      fakeio::_fflush(stdout);
    }
  }
  resume_replay();

  rm("?9");
  fakeio::_fflush(stdout);
  restore_ttymodes();

  vt_move(max_lines - 2, 1);
  return MENU_HOLD;
}

/*
 * DEC locator events are implemented on DECterm, to emulate VT220.
 */
static int
tst_dec_locator_events(MENU_ARGS)
{
  static char pixel_screensize[80];
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
    { "Exit",                                                NULL },
    { "One-Shot",                                            test_dec_locator_event },
    { "Repeated",                                            test_dec_locator_events },
    { "One-Shot (pixels)",                                   test_dec_locator_event_p },
    { "Repeated (pixels)",                                   test_dec_locator_events_p },
    { "Filter Rectangle",                                    test_dec_locator_rectangle },
    { "Filter Rectangle (unfiltered)",                       test_dec_locator_unfiltered },
    { pixel_screensize,                                      get_screensize },
    { "",                                                    NULL }
  };
  /* *INDENT-ON* */

  chars_high = 24;
  chars_wide = 80;
  pixels_high = -1;
  pixels_wide = -1;
  do {
    vt_clear(2);
    __(title(0), println("DEC Locator Events"));
    __(title(2), println("Choose test type:"));
    if (pixels_high > 0 && pixels_wide > 0) {
      sprintf(pixel_screensize,
              "XFree86 xterm: screensize %dx%d chars, %dx%d pixels",
              chars_high, chars_wide, pixels_high, pixels_wide);
    } else {
      strcpy(pixel_screensize, "XFree86 xterm: screensize unknown");
    }
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/*
 * Cycle through the different flavors of mouse-coordinates which are
 * recognized by xterm.
 */
static int
toggle_ExtCoords(MENU_ARGS)
{
  int old_ExtCoords = do_ExtCoords;
  char buffer[80];

  do_ExtPixels = FALSE;
  switch (do_ExtCoords) {
  case cUTF:
    do_ExtCoords = cSGR;
    break;
  case cSGR:
    do_ExtCoords = cURX;
    break;
  case cURX:
    if (can_do_pixels()) {
      do_ExtCoords = cSGRp;
      do_ExtPixels = TRUE;
    } else {
      do_ExtCoords = cDFT;
    }
    break;
  case cSGRp:
    do_ExtCoords = cDFT;
    break;
  default:
    do_ExtCoords = cUTF;
    break;
  }

  if (LOG_ENABLED) {
    fakeio::_fprintf(log_fp, NOTE_STR "toggle from %s to %s\n",
            nameOfExtCoords(old_ExtCoords),
            nameOfExtCoords(do_ExtCoords));
  }

  if (old_ExtCoords) {
    sprintf(buffer, "?%d", old_ExtCoords);
    rm(buffer);
  }

  if (do_ExtCoords) {
    sprintf(buffer, "?%d", do_ExtCoords);
    sm(buffer);
  }

  return MENU_NOHOLD;
}

/*
 * The "focus-event" control adds an event when entering/leaving the window.
 */
static int
toggle_FocusEvent(MENU_ARGS)
{
  do_FocusEvent = !do_FocusEvent;
  if (do_FocusEvent)
    sm("?1004");
  else
    rm("?1004");
  fakeio::_fflush(stdout);
  return MENU_NOHOLD;
}

/*
 * xterm generally implements mouse escape sequences (except for dtterm and
 * DECterm).  XFree86 xterm (and newer) implements additional controls.
 */
int
tst_mouse(MENU_ARGS)
{
  static char txt_Utf8Mouse[80];
  static char txt_FocusEvent[80];
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
    { "Exit",                                                NULL },
    { txt_Utf8Mouse,                                         toggle_ExtCoords },
    { txt_FocusEvent,                                        toggle_FocusEvent },
    { "X10 Mouse Compatibility",                             test_X10_mouse },
    { "Normal Mouse Tracking",                               test_mouse_normal },
    { "Mouse Highlight Tracking",                            test_mouse_hilite },
    { "Mouse Any-Event Tracking (XFree86 xterm)",            test_mouse_any_event },
    { "Mouse Button-Event Tracking (XFree86 xterm)",         test_mouse_button_event },
    { "DEC Locator Events (DECterm)",                        tst_dec_locator_events },
    { "",                                                    NULL }
  };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    sprintf(txt_Utf8Mouse, "Mode: %s coordinates", nameOfExtCoords(do_ExtCoords));
    sprintf(txt_FocusEvent, "Mode: %sFocus-event",
            do_FocusEvent ? "" : "no");
    __(title(0), println("XTERM mouse features"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}
