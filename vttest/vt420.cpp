/* $Id: vt420.c,v 1.236 2024/12/05 00:43:14 tom Exp $ */

/*
 * Reference:  Installing and Using the VT420 Video Terminal (North American
 *             Model (EK-VT420-UG.002)
 */
#include "vttest.h"
#include "draw.h"
#include "esc.h"
#include "ttymodes.h"
#include "fakeio.h"
typedef enum {
  marNone = -1,
  marReset = 0,
  marFirst = 1,
  marLast = 2,
  marMiddle = 3,
  marEnd
} MARS;

typedef enum {
  /* VT525 specific */
  chkFOREGROUND = 0x1,
  chkBACKGROUND = 0x2,
  /* these are not visually distinct, not useful for testing */
  chkPROTECTED = 0x4,           /* DECSCA */
  chkINVISIBLE = 0x8,           /* SGR 8 */
  /* these are visually distinct, used in the test */
  chkUNDERLINE = 0x10,          /* SGR 4 */
  chkINVERSE = 0x20,            /* SGR 7 */
  chkBLINK = 0x40,              /* SGR 5 */
  chkBOLD = 0x80                /* SGR 1 */
} CHKS;

int origin_mode = FALSE;
char txt_override_color[80];

static int do_lines = FALSE;
static int use_colors = FALSE;

/******************************************************************************/

void
reset_colors(void)
{
  if (use_colors) {
    sgr("0");
    use_colors = FALSE;
    if (LOG_ENABLED) {
      fakeio::_fprintf(log_fp, NOTE_STR "turned off colors\n");
    }
  }
}

void
set_colors(const char *value)
{
  if (do_colors) {
    if (value == NULL)
      value = "0";
    sgr(value);
    use_colors = strcmp(value, "0");
    if (LOG_ENABLED) {
      fakeio::_fprintf(log_fp, NOTE_STR "turned %s colors\n",
              use_colors ? "on" : "off");
    }
  }
}

/******************************************************************************/

int lrmm_flag;

static MARS lr_marg_flag;
static int lr_marg1, lr_marg2;
static MARS tb_marg_flag;
static int tb_marg1, tb_marg2;

char origin_mode_mesg[80];
char lrmm_mesg[80];
char lr_marg_mesg[80];
char tb_marg_mesg[80];

int
toggle_LRMM(MENU_ARGS)
{
  lrmm_flag = !lrmm_flag;
  if (lrmm_flag)
    sm("?69");
  else
    rm("?69");
  return MENU_NOHOLD;
}

/*
 * The message tells what margins will be used in the test, not what their
 * value is while drawing the menu (since actually setting margins would
 * interfere with the menu).
 */
int
toggle_STBM(MENU_ARGS)
{ switch (tb_marg_flag) { case marReset: tb_marg_flag = tb_marg_flag = marFirst; break; case marFirst: tb_marg_flag = tb_marg_flag = marLast; break; case marLast: tb_marg_flag = tb_marg_flag = marMiddle; break; case marMiddle: tb_marg_flag = tb_marg_flag = marReset; break; default: break; }
  switch (tb_marg_flag) {
  default:
    tb_marg_flag = marReset;
    tb_marg1 = 0;
    tb_marg2 = 0;
    strcpy(tb_marg_mesg, "Top/Bottom margins are reset");
    break;
  case marFirst:
    tb_marg1 = 1;
    tb_marg2 = max_lines / 2;
    strcpy(tb_marg_mesg, "Top/Bottom margins are set to top half of screen");
    break;
  case marLast:
    tb_marg1 = max_lines / 2;
    tb_marg2 = max_lines;
    strcpy(tb_marg_mesg,
           "Top/Bottom margins are set to bottom half of screen");
    break;
  case marMiddle:
    tb_marg1 = max_lines / 4;
    tb_marg2 = (3 * max_lines) / 4;
    strcpy(tb_marg_mesg,
           "Top/Bottom margins are set to middle half of screen");
    break;
  }
  return MENU_NOHOLD;
}

int
toggle_SLRM(MENU_ARGS)
{ switch (lr_marg_flag) { case marReset: lr_marg_flag = lr_marg_flag = marFirst; break; case marFirst: lr_marg_flag = lr_marg_flag = marLast; break; case marLast: lr_marg_flag = lr_marg_flag = marMiddle; break; case marMiddle: lr_marg_flag = lr_marg_flag = marReset; break; default: break; }
  switch (lr_marg_flag) {
  default:
    lr_marg_flag = marReset;
    lr_marg1 = 0;
    lr_marg2 = 0;
    strcpy(lr_marg_mesg, "Left/right margins are reset");
    break;
  case marFirst:
    lr_marg1 = 1;
    lr_marg2 = min_cols / 2;
    strcpy(lr_marg_mesg, "Left/right margins are set to left half of screen");
    break;
  case marLast:
    lr_marg1 = (min_cols / 2) + 1;
    lr_marg2 = min_cols;
    strcpy(lr_marg_mesg,
           "Left/right margins are set to right half of screen");
    break;
  case marMiddle:
    lr_marg1 = (min_cols / 4) + 1;
    lr_marg2 = (3 * min_cols) / 4;
    strcpy(lr_marg_mesg,
           "Left/right margins are set to middle half of screen");
    break;
  }
  return MENU_NOHOLD;
}

int
get_top_margin(void)
{
  int result = (tb_marg1 ? tb_marg1 : 1);
  if (origin_mode)
    result = 1;
  return result;
}

int
get_left_margin(void)
{
  int result = ((lrmm_flag && lr_marg1) ? lr_marg1 : 1);
  if (origin_mode)
    result = 1;
  return result;
}

int
get_right_margin(void)
{
  int result = ((lrmm_flag && lr_marg2) ? lr_marg2 : min_cols);
  if (origin_mode) {
    result = (((lrmm_flag && lr_marg2) ? lr_marg2 : min_cols) -
              ((lrmm_flag && lr_marg1) ? lr_marg1 : 1) +
              1);
  }
  return result;
}

int
get_bottom_margin(int n)
{
  int result = (tb_marg2 ? tb_marg2 : (n));
  if (origin_mode) {
    result = ((tb_marg2 ? tb_marg2 : max_lines) -
              (tb_marg1 ? tb_marg1 : 1) +
              1);
  }
  return result;
}

static int
get_hold_col(void)
{
  int hold_col = 1;

  if (lrmm_flag) {
    switch (lr_marg_flag) {
    default:
      break;
    case marFirst:
      hold_col = get_right_margin() + 1;
      break;
    case marMiddle:
      hold_col = get_left_margin();
      break;
    }
  }
  return hold_col;
}

/*
 * Return a good row value at which to print a prompt, avoiding most overwrite
 * of test-results.
 */
static int
get_hold_row(void)
{
  int hold_row;

  switch (tb_marg_flag) {
  default:
    hold_row = max_lines / 2;
    break;
  case marFirst:
    hold_row = (origin_mode
                ? (max_lines - 4)
                : (get_bottom_margin(max_lines) + 1));
    break;
  case marMiddle:
    hold_row = (max_lines > 16) ? (max_lines - 4) : (max_lines / 2);
    break;
  case marLast:
    hold_row = 1;
    break;
  }
  return hold_row;
}

static int
hold_clear(void)
{
  int result;

  switch (tb_marg_flag) {
  default:
    result = 0;
    break;
  case marFirst:
    result = 1;
    break;
  case marReset:
  case marLast:
    result = 0;
    break;
  }

  return result;
}

/*
 * Prompt as part of a multi-step test, temporarily resetting DECOM so we can
 * put the prompt anywhere.
 */
static void
special_prompt(int row, int col, const char *msg)
{
  if (origin_mode)
    decom(FALSE);

  vt_move(row, col);
  if (msg != NULL) {
    printxx("%s", msg);
    vt_move(row + 1, col);
  }
  holdit();

  if (origin_mode)
    decom(TRUE);
}

/*
 * Fill area outside margins with given character, to help show that changes
 * are limited to the area within margins.
 */
static void
fill_outside(int ch)
{
  int row, col;

  if (LOG_ENABLED) {
    fakeio::_fprintf(log_fp, NOTE_STR "filling outside margins with '%c'\n", ch);
  }

  if (!do_colors)
    set_colors("0");

  if (origin_mode)
    decom(FALSE);

  for (row = 1; row <= max_lines; ++row) {
    if (row < tb_marg1 ||
        row > tb_marg2 ||
        lr_marg1 > 1 ||
        lr_marg2 < min_cols) {
      int first = 1;
      int next = 0;

      for (col = 1; col <= min_cols; ++col) {
        if ((lrmm_flag && lr_marg1 && col < lr_marg1) ||
            (lrmm_flag && lr_marg2 && col > lr_marg2) ||
            (tb_marg1 != 0 && row < tb_marg1) ||
            (tb_marg2 != 0 && row > tb_marg2)) {
          if (first || (next != col)) {
            vt_move(row, col);
            first = 0;
            next = col + 1;
          }
          fakeio::_putchar(ch);
          ++next;
        }
      }
    }
  }

  if (origin_mode)
    decom(TRUE);
}

void
test_with_margins(int enable)
{
  switch (enable) {
  case 1:
    fill_outside('.');
    /* FALLTHRU */
  case 2:
    decstbm(tb_marg1, tb_marg2);
    decslrm(lr_marg1, lr_marg2);
    if (origin_mode)
      decom(TRUE);
    break;
  default:
    decstbm(0, 0);
    decslrm(0, 0);
    if (origin_mode)
      decom(FALSE);
    break;
  }
}

/*
 * Fill the area within margins with a test pattern.  The top line is numbers,
 * the bottom line is alphas.  In between, use asterisks.
 */
static void
fill_margins(void)
{
  int top = get_top_margin();
  int bot = get_bottom_margin(max_lines);
  int lft = get_left_margin();
  int rgt = get_right_margin();
  int row, col;

  set_colors(WHITE_ON_BLUE);

  decawm(FALSE);  /* do this to allow writing in lower-right */
  for (row = top; row <= bot; ++row) {
    cup(row, lft);
    for (col = lft; col <= rgt; ++col) {
      if (row == top) {
        fakeio::_putchar((col - lft) % 10 + '0');
      } else if (row == bot) {
        fakeio::_putchar((col - lft) % 26 + 'a');
      } else {
        fakeio::_putchar('*');
      }
    }
  }
  decawm(TRUE);
}

static void
setup_rectangle(BOX *box, int last)
{
  box->top = 5;
  box->left = 5;
  box->right = min_cols - 5;
  box->bottom = max_lines - 10;

  if (origin_mode) {
    int top = get_top_margin();
    int lft = get_left_margin();
    int rgt = get_right_margin();
    int bot = get_bottom_margin(last - 1);
    int wide = (rgt - lft + 1);
    int high = (bot - top + 1);

    if (high > 20) {
      box->top = 5;
      box->bottom = high - 10;
    } else {
      box->top = 2;
      box->bottom = high - 2;
    }

    if (wide > 20) {
      box->left = 5;
      box->right = wide - 5;
    } else {
      box->left = 2;
      box->right = wide - 2;
    }
  }
}

#define DATA(name,level) { name, #name, level }

static int
show_DECLRMM(MENU_ARGS)
{
  /* *INDENT-OFF* */
  RQM_DATA dec_modes[] = { /* this list is sorted by code, not name */
    DATA( DECLRMM, 4 /* left/right margin mode */),
  };
  /* *INDENT-ON* */

  int code;
  int old_DECRPM = set_DECRPM(4);

  code = any_RQM(PASS_ARGS, dec_modes, TABLESIZE(dec_modes), 1);
  set_DECRPM(old_DECRPM);
  return code;
}

#undef DATA

/*
 * Allow user to test the same screens with/without lines.
 */
static int
toggle_lines_mode(MENU_ARGS)
{
  do_lines = !do_lines;
  return MENU_NOHOLD;
}

/*
 * Allow user to test the same screens with/without origin-mode.
 */
int
toggle_DECOM(MENU_ARGS)
{
  origin_mode = !origin_mode;
  return MENU_NOHOLD;
}

/*
 * DECALN does not set attributes; we want a colored screen for some tests.
 */
static void
fill_screen(void)
{
  if (do_colors) {
    int y, x;

    set_colors(WHITE_ON_BLUE);
    for (y = 0; y < max_lines - 4; ++y) {
      cup(y + 1, 1);
      for (x = 0; x < min_cols; ++x)
        fakeio::_putchar('E');
    }
    /* make this a different color to show fill versus erase diffs */
    set_colors(WHITE_ON_GREEN);
  } else {
    decaln();   /* fill the screen */
  }
}

/******************************************************************************/

static int
rpt_DECSACE(MENU_ARGS)
{
  return any_decrqss(the_title, "*x");
}

static int
rpt_DECSNLS(MENU_ARGS)
{
  return any_decrqss(the_title, "*|");
}

static int
rpt_DECSLRM(MENU_ARGS)
{
  return any_decrqss(the_title, "s");
}

static int
rpt_DECELF(MENU_ARGS)
{
  return any_decrqss(the_title, "+q");
}

/*
 * VT420 manual shows "=}", but the terminal returns an error.  VT510 sequences
 * show "*}".
 */
static int
rpt_DECLFKC(MENU_ARGS)
{
  return any_decrqss(the_title, "*}");
}

static int
rpt_DECSMKR(MENU_ARGS)
{
  return any_decrqss(the_title, "+r");
}

/******************************************************************************/

static void
show_DataIntegrity(const char *report)
{
  int pos = 0;
  int code = scanto(report, &pos, 'n');
  const char *show;

  switch (code) {
  case 70:
    show = "No communication errors";
    break;
  case 71:
    show = "Communication errors";
    break;
  case 73:
    show = "Not reported since last power-up or RIS";
    break;
  default:
    show = SHOW_FAILURE;
  }
  show_result("%s", show);
}

static void
show_keypress(int row, int col)
{
  const char *report;
  char last[BUF_SIZE];

  last[0] = '\0';
  vt_move(row++, 1);
  println("When you are done, press any key twice to quit.");
  vt_move(row, col);
  fakeio::_fflush(stdout);
  pause_replay();
  while (strcmp(report = instr(), last)) {
    vt_move(row, col);
    vt_clear(0);
    chrprint2(report, row, col);
    strncpy(last, report, sizeof(last) - 2)[sizeof(last) - 2] = '\0';
  }
  resume_replay();
}

static void
show_MultisessionStatus(const char *report)
{
  int pos = 0;
  int Ps1 = scan_any(report, &pos, 'n');
  int Ps2 = scanto(report, &pos, 'n');

  switch (Ps1) {
  case 80:
    show_result("SSU sessions enabled (%d max)", Ps2);
    break;
  case 81:
    show_result("SSU sessions available but pending (%d max)", Ps2);
    break;
  case 83:
    show_result("SSU sessions not ready");
    break;
  case 87:
    show_result("Sessions on separate lines");
    break;
  default:
    show_result(SHOW_FAILURE);
  }
}

/******************************************************************************/

/*
 * VT400 & up.
 * DECBI - Back Index
 * This control function moves the cursor backward one column.  If the cursor
 * is at the left margin, then all screen data within the margin moves one
 * column to the right.  The column that shifted past the right margin is lost.
 *
 * Format:  ESC 6
 * Description:
 * DECBI adds a new column at the left margin with no visual attributes.  DECBI
 * is not affected by the margins.  If the cursor is at the left border of the
 * page when the terminal received DECBI, then the terminal ignores DECBI.
 */
static int
tst_DECBI(MENU_ARGS)
{
  int n, m;
  int last = max_lines - 4;
  int final;
  int top;
  int lft;
  int rgt;

  test_with_margins(1);

  top = get_top_margin();
  lft = get_left_margin();
  rgt = get_right_margin();

  final = (rgt - lft + 1) / 4;

  set_colors(WHITE_ON_BLUE);

  for (n = final; n > 0; n--) {
    slowly();
    cup(top, lft);
    if (n != final) {
      for (m = 0; m < 4; m++)
        decbi();
    }
    printxx("%3d", n);
  }

  reset_colors();

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  println("If your terminal supports DECBI (backward index), then the top row");
  printxx("should be numbered 1 through %d.\n", final);
  return MENU_HOLD;
}

static int
tst_DECBKM(MENU_ARGS)
{
  int row, col;
  const char *report;

  vt_move(1, 1);
  println(the_title);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  reset_inchar();
  decbkm(TRUE);
  println("Press the backspace key");
  vt_move(row = 3, col = 10);
  report = instr();
  chrprint2(report, row, col);
  show_result(!strcmp(report, "\010") ? SHOW_SUCCESS : SHOW_FAILURE);

  reset_inchar();
  vt_move(5, 1);
  decbkm(FALSE);
  println("Press the backspace key again");
  vt_move(row = 6, col = 10);
  report = instr();
  chrprint2(report, row, col);
  show_result(!strcmp(report, "\177") ? SHOW_SUCCESS : SHOW_FAILURE);

  vt_move(max_lines - 1, 1);
  restore_ttymodes();
  return MENU_HOLD;
}

/*
 * VT400 & up
 * Change Attributes in Rectangular Area
 */
static int
tst_DECCARA(MENU_ARGS)
{
  int last = max_lines - 4;
  BOX box;

  setup_rectangle(&box, last);

  test_with_margins(1);

  set_colors(WHITE_ON_BLUE);

  decsace(TRUE);
  fill_screen();
  deccara(box.top, box.left, box.bottom, box.right, 7);   /* invert a rectangle) */
  deccara(box.top + 1, box.left + 1, box.bottom - 1, box.right - 1, 0);   /* invert a rectangle) */

  test_with_margins(0);
  sgr("0");
  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  println("There should be an open rectangle formed by reverse-video E's");
  holdit();

  test_with_margins(2);
  decsace(FALSE);
  fill_screen();
  deccara(box.top, box.left, box.bottom, box.right, 7);   /* invert a rectangle) */
  deccara(box.top + 1, box.left + 1, box.bottom - 1, box.right - 1, 0);   /* invert a rectangle) */

  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  println("There should be an open rectangle formed by reverse-video E's");
  println("combined with wrapping at the margins.");
  return MENU_HOLD;
}

#define fmt_DECCKSR "Testing DECCKSR: %s"

static int
parse_DECCKSR(char *report, int Pid, int *digits, int *checksum)
{
  int result = 0;
  int pos = 0;
  int actual;
  const char *after;
  const char *before;

  if ((report = skip_dcs(report)) != NULL
      && strip_terminator(report)
      && strlen(report) > 1
      && scanto(report, &pos, '!') == Pid
      && report[pos++] == '~'
      && (after = skip_xdigits((before = report + pos), &actual)) != NULL
      && *after == '\0') {
    result = 1;
    *digits = (int) (after - before);
    *checksum = actual;
  }
  return result;
}

static char *
check_DECCKSR(char *target, char *source, int Pid, int expected)
{
  int digits;
  int actual;

  if (parse_DECCKSR(source, Pid, &digits, &actual)) {
    if (digits != 4) {
      sprintf(target, "%s: expected 4 digits", SHOW_FAILURE);
    } else if (expected < 0 || (actual == expected)) {
      strcpy(target, SHOW_SUCCESS);
    } else {
      sprintf(target, "expected %04X", (expected & 0xffff));
    }
  } else {
    strcpy(target, SHOW_FAILURE);
  }
  return target;
}

static int
tst_DECCKSR(MENU_ARGS, int Pid, const char *the_csi, int expected)
{
  char *report;
  int row, col;
  char temp[80];

  vt_move(1, 1);
  printxx(fmt_DECCKSR, the_title);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  do_csi("%s", the_csi);
  report = get_reply();
  vt_move(row = 3, col = 10);
  chrprint2(report, row, col);
  show_result("%s", check_DECCKSR(temp, report, Pid, expected));

  restore_ttymodes();
  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

/*
 * VT400 & up.
 * Copy Rectangular area
 */
static int
tst_DECCRA(MENU_ARGS)
{
#define adj_y 3
#define adj_x 4
#define adj_DECCRA " (down %d, right %d)\r\n", box.bottom + 1 - box.top, box.right + 1 - box.left, adj_y, adj_x
#define msg_DECCRA(msg) "The %dx%d box " msg adj_DECCRA
  BOX box;
  int hmargin = origin_mode ? ((get_right_margin() * 3) / 8) : 30;
  int vmargin = origin_mode ? ((get_bottom_margin(max_lines) * 2) / 5) : 10;
  int last = max_lines - 3;

  if (make_box_params(&box, vmargin, hmargin) == 0) {
    box.top = 5;
    box.left = 5;

    test_with_margins(1);

    if (do_colors) {
      set_colors(WHITE_ON_BLUE);
    } else {
      sgr(BLINK_REVERSE);
    }
    draw_box_outline(&box, do_lines ? -1 : '*');
    sgr("0");

    test_with_margins(0);

    vt_move(last, 1);
    println(the_title);
    tprintf(msg_DECCRA("will be copied"));
    holdit();

    test_with_margins(2);

    deccra(box.top, box.left, box.bottom, box.right, 1,
           box.top + adj_y, box.left + adj_x, 1);

    test_with_margins(0);

    vt_move(last, 1);
    vt_clear(0);

    tprintf(msg_DECCRA("should be copied, overlapping"));
    holdit();

    test_with_margins(2);

    make_box_params(&box, vmargin, hmargin);
    box.top = 5;
    box.left = 5;

    if (do_colors) {
      set_colors(YELLOW_ON_BLACK);
    } else {
      sgr("0;7");   /* fill the box in reverse */
    }
    draw_box_filled(&box, -1);

    if (do_colors) {
      set_colors(WHITE_ON_BLUE);
    } else {
      sgr(BLINK_REVERSE);
    }
    draw_box_outline(&box, do_lines ? -1 : '*');
    sgr("0");

    test_with_margins(0);

    vt_move(last, 1);
    println(the_title);
    tprintf(msg_DECCRA("will be copied"));
    holdit();

    test_with_margins(2);

    sgr("0;4"); /* set underline, to check if that leaks through */
    deccra(box.top, box.left, box.bottom, box.right, 1,
           box.top + adj_y, box.left + adj_x, 1);
    sgr("0");

    test_with_margins(0);

    vt_move(last, 1);
    vt_clear(0);

    tprintf(msg_DECCRA("should be copied, overlapping"));
  }
  return MENU_HOLD;
}

static int
marker_of(int n)
{
  return (n - 1) % 26 + 'a';
}

/*
 * VT400 & up.
 * Delete column.
 */
static int
tst_DECDC(MENU_ARGS)
{
  int n;
  int last = max_lines - 3;
  int base_row;
  int base_col;
  int left_col;
  int last_row;
  int real_col;
  int top;
  int bot;
  int lft;
  int rgt;
  int final_dc;
  char mark_1st = 0;
  char mark_2nd = 0;

  test_with_margins(1);

  set_colors(WHITE_ON_BLUE);

  top = get_top_margin();
  lft = get_left_margin();
  rgt = get_right_margin();
  bot = get_bottom_margin(last - 1);

  /*
   * Adjustments so that most of the initial line (before shifting) passes
   * through the area within margins.
   */
  if (origin_mode) {
    base_row = 0;
    if (lrmm_flag) {
      left_col = 1;
      switch (tb_marg_flag) {
      default:
        last_row = bot;
        break;
      case marReset:
      case marLast:
        last_row = bot - 3;
        break;
      }
      base_col = rgt - (bot - top) + last_row;
      if (base_col < 0)
        base_col = 0;
      if (base_col > rgt)
        base_col = rgt;
      real_col = lr_marg1 + lft - (lr_marg1 != 0);
    } else {
      last_row = last;
      base_col = (2 * last);
      left_col = 1;
      real_col = lft;
    }
  } else {
    switch (lr_marg_flag) {
    default:
      base_col = (2 * last);
      left_col = 1;
      break;
    case marFirst:
      base_col = (min_cols / 2);
      left_col = 1;
      break;
    case marMiddle:
      base_col = (3 * min_cols) / 4;
      left_col = (min_cols / 4) + 1;
      break;
    case marLast:
      base_col = min_cols + 0;
      left_col = (min_cols / 2) + 1;
      break;
    }
    if (tb_marg_flag == marLast) {
      base_row = max_lines / 2;
    } else {
      base_row = 0;
    }
    last_row = last;
    real_col = lft;
  }

  final_dc = base_col - 1;

  for (n = 1; n < last_row; n++) {
    int row = base_row + n;
    int col = base_col - n;

    if (row <= last_row) {
      int mark = marker_of(n);

      if (row >= top && row <= bot && row < last_row) {
        mark_2nd = (char) mark;
        if (mark_1st == 0) {
          mark_1st = (char) mark;
        }
      }

      slowly();
      __(cup(row, col), print_chr(mark));
      if (top > 1 || (lrmm_flag && lft > 1)) {
        __(cup(1, 1), decdc(1));  /* outside margins, should be ignored */
        __(cup(row, col), print_chr(mark));
      }
      if (final_dc-- > left_col)
        __(cup(top, lft), decdc(1));
    }
  }
  if (final_dc > left_col) {
    slowly();
    __(cup(top, lft), decdc(final_dc - left_col));
  }

  reset_colors();

  test_with_margins(0);

  ruler(last, min_cols);
  vt_move(last + 1, 1);
  vt_clear(0);

  tprintf("If your terminal supports DECDC, letters %c-%c are on column %d\n",
          mark_1st, mark_2nd, real_col);
  return MENU_HOLD;
}

static void
show_DECERA(MENU_ARGS, const char *color_name, const char *color_value)
{
  int last = max_lines - 3;
  BOX box;

  setup_rectangle(&box, last);

  fill_screen();

  test_with_margins(1);

  set_colors(color_value);

  decera(box.top, box.left, box.bottom, box.right);

  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  tprintf("There should be a%s rectangle cleared in the middle of the %s.\r\n",
          color_name,
          origin_mode ? "margins" : "screen");
}

/*
 * VT400 & up
 * Erase Rectangular area
 */
static int
tst_DECERA(MENU_ARGS)
{
  if (do_colors) {
    show_DECERA(PASS_ARGS, " blue", WHITE_ON_BLUE);
    holdit();
    show_DECERA(PASS_ARGS, " green", WHITE_ON_GREEN);
  } else {
    show_DECERA(PASS_ARGS, "", WHITE_ON_BLUE);
  }
  return MENU_HOLD;
}

/*
 * This is two tests:  IND (index) and RI (reverse index).  For each test, we
 * start by filling the area inside (including) the margins with a test
 * pattern, and then after the user presses "return", update the screen so that
 * only one line of the test-pattern should remain visible.
 */
static int
tst_IND_RI(MENU_ARGS)
{
  int hold_row = get_hold_row();
  int hold_col = get_hold_col();  /* where to put "Push RETURN" */
  int row;
  int top = get_top_margin();
  int bot = get_bottom_margin(max_lines);
  int lft = get_left_margin();
  int rgt = get_right_margin();

  test_with_margins(1);

  fill_margins();

  set_colors(NULL);
  special_prompt(hold_row, hold_col, NULL);

  set_colors(WHITE_ON_GREEN);
  cup(bot, (lft + rgt) / 2);
  for (row = top; row < bot; ++row) {
    slowly();
    ind();
  }

  set_colors(NULL);
  special_prompt(hold_row, hold_col, "\"abcd...\" should be at top. ");

  fill_margins();
  fill_outside('.');

  set_colors(NULL);
  special_prompt(hold_row, hold_col, NULL);

  set_colors(WHITE_ON_GREEN);
  cup(top, (lft + rgt) / 2);
  for (row = top; row < bot; ++row) {
    slowly();
    ri();
  }

  set_colors(NULL);
  special_prompt(hold_row, hold_col, "\"0123...\" should be at bottom. ");

  test_with_margins(0);

  return MENU_NOHOLD;
}

static int
tst_IL_DL(MENU_ARGS)
{
  int hold_row = get_hold_row();
  int hold_col = get_hold_col();  /* where to put "Push RETURN" */
  int row;
  int top = get_top_margin();
  int bot = get_bottom_margin(max_lines);
  int lft = get_left_margin();
  int rgt = get_right_margin();

  test_with_margins(1);

  fill_margins();

  set_colors(NULL);
  special_prompt(hold_row, hold_col, NULL);

  /*
   * This should be ignored because it is outside margins.
   */
  set_colors(WHITE_ON_GREEN);
  if (!origin_mode) {
    if (top > 1) {
      cup(top - 1, lft);
      il(1);
    } else if (bot < max_lines) {
      cup(bot + 1, lft);
      il(1);
    } else if (lft > 1) {
      cup(top, lft - 1);
      il(1);
    } else if (rgt < min_cols) {
      cup(top, rgt + 1);
      il(1);
    }
  }

  cup(top, (lft + rgt) / 2);
  for (row = top; row < bot;) {
    int skip = (row % 2) + 1;
    row += skip;
    if (row >= bot)
      skip = 1;
    slowly();
    il(skip);
  }

  set_colors(NULL);
  special_prompt(hold_row, hold_col, "\"0123...\" should be at bottom. ");

  fill_margins();
  fill_outside('.');

  set_colors(NULL);
  special_prompt(hold_row, hold_col, NULL);

  set_colors(WHITE_ON_GREEN);
  cup(top, (lft + rgt) / 2);
  for (row = top; row < bot;) {
    int skip = (row % 2) + 1;
    row += skip;
    if (row >= bot)
      skip = 1;
    slowly();
    dl(skip);
  }

  set_colors(NULL);
  special_prompt(hold_row, hold_col, "\"abcd...\" should be at top. ");

  test_with_margins(0);

  return MENU_NOHOLD;
}

static int
tst_ICH_DCH(MENU_ARGS)
{
  int n;
  int last = max_lines - 3;
  int base_row;
  int base_col;
  int last_row;
  int real_col;
  int top;
  int bot;
  int lft;
  int rgt;
  char mark_1st = 0;
  char mark_2nd = 0;

  test_with_margins(1);

  set_colors(WHITE_ON_BLUE);

  top = get_top_margin();
  bot = get_bottom_margin(last - 1);
  lft = get_left_margin();
  rgt = get_right_margin();

  /*
   * Adjustments so that most of the initial line (before shifting) passes
   * through the area within margins.
   */
  if (origin_mode) {
    base_row = 0;
    if (lrmm_flag) {
      base_col = rgt - (bot - top) - 2;
      if (base_col < 0)
        base_col = 0;
      switch (tb_marg_flag) {
      default:
        last_row = bot;
        break;
      case marReset:
      case marLast:
        last_row = bot - 3;
        break;
      }
      real_col = rgt + lr_marg1 - (lr_marg1 != 0);
    } else {
      last_row = last;
      base_col = (2 * last);
      real_col = rgt;
    }
  } else {
    switch (lr_marg_flag) {
    default:
      base_col = (2 * last);
      break;
    case marFirst:
      base_col = 0;
      break;
    case marMiddle:
      base_col = min_cols / 4;
      break;
    case marLast:
      base_col = (min_cols / 2);
      break;
    }
    if (tb_marg_flag == marLast) {
      base_row = max_lines / 2;
    } else {
      base_row = 0;
    }
    last_row = last;
    real_col = rgt;
  }

  for (n = 1; n < last_row; n++) {
    int row = base_row + n;
    int col = base_col + n;

    if (row < last_row) {
      int mark = marker_of(n);

      if (row >= top && row <= bot) {
        mark_2nd = (char) mark;
        if (mark_1st == 0) {
          mark_1st = (char) mark;
        }
      }

      slowly();
      __(cup(row, col), print_chr(mark));
      if (col < rgt) {
        cup(row, lft);
        print_chr('?');
        cup(row, lft);
        ich(rgt - col);
      }
    }
  }

  reset_colors();

  test_with_margins(0);

  ruler(last, min_cols);
  vt_move(last + 1, 1);
  vt_clear(0);

  tprintf("If your terminal supports ICH, letters %c-%c are on column %d\n",
          mark_1st, mark_2nd, real_col);
  holdit();

  vt_clear(0);
  test_with_margins(1);

  set_colors(WHITE_ON_BLUE);

  /*
   * Adjustments so that most of the initial line (before shifting) passes
   * through the area within margins.
   */
  if (origin_mode) {
    base_row = 0;
    if (lrmm_flag) {
      switch (tb_marg_flag) {
      default:
        last_row = bot;
        break;
      case marReset:
      case marLast:
        last_row = bot - 3;
        break;
      }
      base_col = rgt - (bot - top) + last_row;
      if (base_col < 0)
        base_col = 0;
      if (base_col > rgt)
        base_col = rgt;
      real_col = lr_marg1 + lft - (lr_marg1 != 0);
    } else {
      last_row = last;
      base_col = (2 * last);
      real_col = lft;
    }
  } else {
    switch (lr_marg_flag) {
    default:
      base_col = (2 * last);
      break;
    case marFirst:
      base_col = (min_cols / 2);
      break;
    case marMiddle:
      base_col = (3 * min_cols) / 4;
      break;
    case marLast:
      base_col = min_cols + 0;
      break;
    }
    if (tb_marg_flag == marLast) {
      base_row = max_lines / 2;
    } else {
      base_row = 0;
    }
    last_row = last;
    real_col = lft;
  }

  for (n = 1; n < last_row; n++) {
    int row = base_row + n;
    int col = base_col - n;

    if (row <= last_row) {
      int mark = marker_of(n);

      if (row >= top && row <= bot && row < last_row) {
        mark_2nd = (char) mark;
        if (mark_1st == 0) {
          mark_1st = (char) mark;
        }
      }

      __(cup(row, col), print_chr(mark));
      slowly();
      if (col < rgt)
        ech(rgt - col);
      if (col > lft) {
        cup(row, lft);
        dch(col - lft);
      } else {
        cup(row, 1);
        dch(col - 1);
      }
    }
  }

  reset_colors();

  test_with_margins(0);

  ruler(last, min_cols);
  vt_move(last + 1, 1);
  vt_clear(0);

  tprintf("If your terminal supports DCH, letters %c-%c are on column %d\n",
          mark_1st, mark_2nd, real_col);
  return MENU_HOLD;
}

/*
 * Check to see if ASCII formatting controls (BS, HT, CR) are affected by
 * left/right margins.  Do this by starting after the left-margin, and
 * backspacing "before" the left margin.  Then fill the margins with a usable
 * test pattern.  After that, use tabs to go to the right margin, adding
 * another usable test (+), and use carriage returns to go to the left margin,
 * adding another usable test (-).
 */
static int
tst_ASCII_format(MENU_ARGS)
{
  int last = max_lines - 4;
  int top;
  int bot;
  int lft;
  int rgt;
  int n;
  int tab;
  int size;

  test_with_margins(1);

  top = get_top_margin();
  bot = get_bottom_margin(last - 1);
  lft = get_left_margin();
  rgt = get_right_margin();

  /*
   * This should stop at the left margin, and the result overwritten by a
   * fill-pattern.
   */
  set_colors(WHITE_ON_BLUE);
  cup(top, rgt);
  for (n = 0; n < rgt; ++n) {
    tprintf("*%c%c", BS, BS);
  }

  /*
   * Fill the margins with a repeating pattern.  Do it twice, to force it to
   * scroll up.
   */
  set_colors(WHITE_ON_GREEN);
  size = 2 * (rgt - lft + 1) * (bot - top + 1);
  for (n = 0; n < size; ++n) {
    int ch = ((n % 10) ? ((n % 10) + '0') : '_');
    fakeio::_putchar(ch);
  }

  /*
   * Mark the margins with '-' (left) and '+' (right).
   */
  set_colors(YELLOW_ON_BLACK);
  cup(top, lft);
  for (n = top; n <= bot; ++n) {
    for (tab = 0; tab < (rgt - lft + 16) / TABWIDTH; ++tab) {
      fakeio::_putchar(TAB);
    }
    fakeio::_putchar('+');
    fakeio::_putchar(CR);
    fakeio::_putchar('-');
    fakeio::_putchar(TAB);
    fakeio::_putchar('*');
    if (n < bot) {
      fakeio::_putchar(LF);
    }
  }

  test_with_margins(0);

  set_colors("0");

  vt_move(last, 1);
  vt_clear(0);

  ruler(last, min_cols);
  println(the_title);
  println("A repeating \"0123456789_\" pattern should fall within the -/+ margins");
  return MENU_HOLD;
}

/*
 * VT400 & up.
 *
 * DECFI - Forward Index
 * This control function moves the column forward one column.  If the cursor is
 * at the right margin, then all screen data within the margins moves one
 * column to the left.  The column shifted past the left margin is lost.
 *
 * Format: ESC 9
 * Description:
 * DECFI adds a new column at the right margin with no visual attributes.
 * DECFI is not affected by the margins.  If the cursor is at the right border
 * of the page when the terminal receives DECFI, then the terminal ignores
 * DECFI.
 */
static int
tst_DECFI(MENU_ARGS)
{
  int n, m;
  int last = max_lines - 4;
  int final;
  int top;
  int lft;
  int rgt;

  test_with_margins(1);

  set_colors(WHITE_ON_BLUE);

  top = get_top_margin();
  lft = get_left_margin();
  rgt = get_right_margin();

  final = (rgt - lft + 1) / 4;

  for (n = 1; n <= final; n++) {
    slowly();
    cup(top, rgt - 3);
    tprintf("%3d", n);  /* leaves cursor in rightmost column */
    if (n != final) {
      for (m = 0; m < 4; m++)
        decfi();
    }
  }

  reset_colors();

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  println("If your terminal supports DECFI (forward index), then the top row");
  printxx("should be numbered 1 through %d.\n", final);
  return MENU_HOLD;
}

/*
 * Demonstrate whether cursor movement is limited by margins.  VT420 manual
 * says that CUU/CUD will stop on the margins, but if outside the margins
 * will proceed to the page border.  So we can test this by
 *
 * a) moving to the margin, and cursor up/down toward the border, placing a
 * marker at the end of the cursor movement (to overwrite a prior marker placed
 * explicitly on the border).
 *
 * b) repeat the process, going from the border into the area within margins.
 *
 * c) Even for the no-margins case, this is useful, since it demonstrates
 * whether the cursor forces scrolling.
 */
static int
tst_cursor_margins(MENU_ARGS)
{
  BOX box;
  int last = get_hold_row();
  int row;
  int col;

  test_with_margins(1);

  box.top = get_top_margin();
  box.left = get_left_margin();
  box.right = get_right_margin();
  box.bottom = get_bottom_margin(max_lines);

  set_colors(WHITE_ON_BLUE);
  draw_box_filled(&box, ' ');
  draw_box_outline(&box, '*');
  set_colors(WHITE_ON_GREEN);

  for (row = box.top; row <= box.bottom; ++row) {
    cup(row, box.left);
    for (col = min_cols; col > 0; col--) {
      cub(1);
    }
    fakeio::_putchar('l');
  }

  for (row = box.top; row <= box.bottom; ++row) {
    cup(row, box.right);
    for (col = get_left_margin(); col <= min_cols; col++) {
      cuf(1);
    }
    fakeio::_putchar('r');
  }

  for (col = box.left; col <= box.right; ++col) {
    cup(box.top, col);
    for (row = box.top; row > 0; row--) {
      cuu(1);
    }
    fakeio::_putchar('u');
  }

  for (col = box.left; col <= box.right; ++col) {
    cup(box.bottom, col);
    for (row = box.bottom; row <= max_lines; row++) {
      cud(1);
    }
    fakeio::_putchar('d');
  }

  set_colors("0");
  test_with_margins(0);

  vt_move(last, 1);
  if (last > box.bottom)
    vt_clear(0);

  println(the_title);
  println("A box of *'s was written on screen border, overwritten using margins (u/d/l/r)");
  return MENU_HOLD;
}

/*
 * Test movement with other things than cursor controls, i.e., BS, HT, CR, LF,
 * to see how margins affect them.
 */
static int
tst_other_margins(MENU_ARGS)
{
  BOX box;
  int last = get_hold_row();
  int row;
  int col;

  test_with_margins(1);

  box.top = get_top_margin();
  box.left = get_left_margin();
  box.right = get_right_margin();
  box.bottom = get_bottom_margin(max_lines);

  set_colors(WHITE_ON_BLUE);
  draw_box_filled(&box, ' ');
  draw_box_outline(&box, '*');
  set_colors(WHITE_ON_GREEN);

  for (row = box.top; row <= box.bottom; ++row) {
    cup(row, box.left);
    for (col = box.left; col > 0; col--) {
      fakeio::_putchar('\b');
    }
    fakeio::_putchar('l');
  }

  for (row = box.top; row <= box.bottom; ++row) {
    cup(row, box.right);
    for (col = 1; col <= min_cols; col++) {
      fakeio::_putchar('\t');
    }
    fakeio::_putchar('r');
  }

  for (col = box.left; col <= box.right; ++col) {
    cup(box.top, col);
    fakeio::_putchar('u');
    for (row = box.bottom; row > box.top; row--) {
      ind();
    }
  }

  for (col = box.left; col <= box.right; ++col) {
    switch (col % 4) {
    case 0:
      cup(box.bottom, col);
      fakeio::_putchar('d');
      break;
    case 1:
      cup(box.top, col);
      for (row = box.top; row < box.bottom; row++) {
        fakeio::_putchar('\f');
      }
      fakeio::_putchar('d');
      break;
    case 2:
      cup(box.top, col);
      for (row = box.top; row < box.bottom; row++) {
        nel();
        cuf(col - get_left_margin());
      }
      fakeio::_putchar('d');
      break;
    case 3:
      cup(box.bottom, col);
      fakeio::_putchar('d');
      for (row = box.top; row < box.bottom; row++) {
        ri();
      }
      fakeio::_putchar('u');
      break;
    }
  }

  set_colors("0");
  test_with_margins(0);

  vt_move(last, 1);
  if (hold_clear())
    vt_clear(0);

  println(the_title);
  println("A box of *'s was written on screen border, overwritten using margins (u/d/l/r)");
  return MENU_HOLD;
}

/*
 * VT400 & up
 * Fill Rectangular area
 */
static int
tst_DECFRA(MENU_ARGS)
{
  int last = max_lines - 3;
  BOX box;

  setup_rectangle(&box, last);

  test_with_margins(1);

  if (do_colors) {
    set_colors(WHITE_ON_BLUE);
    vt_clear(2);  /* xterm fills the whole screen's background */
    set_colors(WHITE_ON_GREEN);
  }
  decfra('*', box.top, box.left, box.bottom, box.right);

  set_colors("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  if (origin_mode)
    println("There should be a rectangle of *'s in the middle of the margins.");
  else
    println("There should be a rectangle of *'s in the middle of the screen.");
  holdit();

  test_with_margins(2);

  set_colors(WHITE_ON_BLUE);

  decfra(' ', box.top, box.left, box.bottom, box.right);
  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  println("The rectangle of *'s should be gone.");
  return MENU_HOLD;
}

/*
 * VT400 & up.
 * Insert column.
 */
static int
tst_DECIC(MENU_ARGS)
{
  int n;
  int last = max_lines - 3;
  int base_row;
  int base_col;
  int last_row;
  int last_col;
  int real_col;
  int top;
  int bot;
  int lft;
  int rgt;
  int final_ic;
  char mark_1st = 0;
  char mark_2nd = 0;

  test_with_margins(1);

  set_colors(WHITE_ON_BLUE);

  top = get_top_margin();
  bot = get_bottom_margin(last - 1);
  lft = get_left_margin();
  rgt = get_right_margin();

  /*
   * Adjustments so that most of the initial line (before shifting) passes
   * through the area within margins.
   */
  if (origin_mode) {
    base_row = 0;
    if (lrmm_flag) {
      base_col = rgt - (bot - top) - 2;
      if (base_col < 0)
        base_col = 0;
      last_col = rgt - 1;
      switch (tb_marg_flag) {
      default:
        last_row = bot;
        break;
      case marReset:
      case marLast:
        last_row = bot - 3;
        break;
      }
      real_col = rgt + lr_marg1 - (lr_marg1 != 0);
    } else {
      last_row = last;
      base_col = (2 * last);
      last_col = min_cols - 1;
      real_col = rgt;
    }
  } else {
    if (lrmm_flag) {
      switch (lr_marg_flag) {
      default:
        base_col = (2 * last);
        last_col = min_cols - 1;
        break;
      case marFirst:
        base_col = 0;
        last_col = min_cols / 2 - 1;
        break;
      case marMiddle:
        base_col = min_cols / 4;
        last_col = (3 * min_cols) / 4 - 1;
        break;
      case marLast:
        base_col = (min_cols / 2);
        last_col = min_cols - 1;
        break;
      }
    } else {
      base_col = (2 * last);
      last_col = min_cols - 1;
    }
    if (tb_marg_flag == marLast) {
      base_row = max_lines / 2;
    } else {
      base_row = 0;
    }
    last_row = last;
    real_col = rgt;
  }

  final_ic = base_col;

  for (n = 1; n < last_row; n++) {
    int row = base_row + n;
    int col = base_col + n;

    if (row < last_row) {
      int mark = marker_of(n);

      if (row >= top && row <= bot) {
        mark_2nd = (char) mark;
        if (mark_1st == 0) {
          mark_1st = (char) mark;
        }
      }

      slowly();
      __(cup(row, col), print_chr(mark));
      if (!origin_mode && (top > 1 || (lrmm_flag && lft > 1))) {
        __(cup(1, 1), decic(1));  /* outside margins, should be ignored */
        __(cup(row, col), print_chr(mark));
      }
      if (final_ic++ <= last_col)
        __(cup(top, lft), decic(1));
    }
  }
  if (final_ic <= last_col) {
    slowly();
    decic(last_col - final_ic);
  }

  reset_colors();

  test_with_margins(0);

  ruler(last, min_cols);
  vt_move(last + 1, 1);
  vt_clear(0);

  tprintf("If your terminal supports DECIC, letters %c-%c are on column %d\n",
          mark_1st, mark_2nd, real_col);
  return MENU_HOLD;
}

static int
tst_DECIC_DECDC(MENU_ARGS)
{
  tst_DECIC(PASS_ARGS);
  holdit();
  vt_clear(2);
  tst_DECDC(PASS_ARGS);
  return MENU_HOLD;
}

static int
tst_DECKBUM(MENU_ARGS)
{
  vt_move(1, 1);
  println(the_title);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  deckbum(TRUE);
  println("The keyboard is set for data processing.");
  show_keypress(3, 10);

  vt_move(10, 1);
  deckbum(FALSE);
  println("The keyboard is set for normal (typewriter) processing.");
  show_keypress(11, 10);

  restore_ttymodes();
  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

static int
tst_DECKPM(MENU_ARGS)
{
  vt_move(1, 1);
  println(the_title);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  deckpm(TRUE);
  println("The keyboard is set for position reports.");
  show_keypress(3, 10);

  vt_move(10, 1);
  deckpm(FALSE);
  println("The keyboard is set for character codes.");
  show_keypress(11, 10);

  restore_ttymodes();
  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

static int
tst_DECNKM(MENU_ARGS)
{
  vt_move(1, 1);
  println(the_title);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  decnkm(FALSE);
  println("Press one or more keys on the keypad.  They should generate numeric codes.");
  show_keypress(3, 10);

  vt_move(10, 1);
  decnkm(TRUE);
  println("Press one or more keys on the keypad.  They should generate control codes.");
  show_keypress(11, 10);

  decnkm(FALSE);
  vt_move(max_lines - 1, 1);
  restore_ttymodes();
  return MENU_HOLD;
}

/*
 * VT400 & up
 * Reverse Attributes in Rectangular Area
 */
static int
tst_DECRARA(MENU_ARGS)
{
  int last = max_lines - 4;
  BOX box;

  setup_rectangle(&box, last);

  decsace(TRUE);
  fill_screen();

  test_with_margins(1);

  decrara(box.top, box.left, box.bottom, box.right, 7);   /* invert a rectangle) */
  decrara(box.top + 1, box.left + 1, box.bottom - 1, box.right - 1, 7);   /* invert a rectangle) */

  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  println("There should be an open rectangle formed by reverse-video E's");
  holdit();

  decsace(FALSE);
  fill_screen();

  test_with_margins(1);

  decrara(box.top, box.left, box.bottom, box.right, 7);   /* invert a rectangle) */
  decrara(box.top + 1, box.left + 1, box.bottom - 1, box.right - 1, 7);   /* invert a rectangle) */

  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  println("There should be an open rectangle formed by reverse-video E's");
  println("combined with wrapping at the margins.");
  return MENU_HOLD;
}

int
tst_vt420_DECRQSS(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test VT320 features (DECRQSS)",                     tst_vt320_DECRQSS },
      { "Enable local functions (DECELF)",                   rpt_DECELF },
      { "Local function key control (DECLFKC)",              rpt_DECLFKC },
      { "Select attribute change extent (DECSACE)",          rpt_DECSACE },
      { "Select modifier key reporting (DECSMKR)",           rpt_DECSMKR },
      { "Set left and right margins (DECSLRM)",              rpt_DECSLRM },
      { "Set number of lines per screen (DECSNLS)",          rpt_DECSNLS },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Status-Strings Reports"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/*
 * Selective-Erase Rectangular area
 */
static int
tst_DECSERA(MENU_ARGS)
{
  int last = max_lines - 3;
  BOX box;

  setup_rectangle(&box, last);

  /*
   * Part 1: clear the borders of a rectangle, leaving protect inner rectangle.
   */
  fill_screen();

  test_with_margins(1);

  set_colors(WHITE_ON_GREEN);

  /*
   * Protect an area slightly smaller than we will erase.
   *
   * That way (since the SGR color at this point differs from the color used to
   * fill the screen), we can see whether the colors are modified by the erase,
   * and if so, whether they come from the SGR color.
   */
  decfra('*', box.top, box.left, box.bottom, box.right);
  decsca(1);
  decfra('*', box.top + 1, box.left + 1, box.bottom - 1, box.right - 1);
  decsca(0);

  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  tprintf("Rectangle %d,%d - %d,%d was filled using DECFRA\n",
          box.top,
          box.left,
          box.bottom,
          box.right);
  holdit();

  test_with_margins(2);   /* reenable but do not paint margins */

  set_colors(WHITE_ON_BLUE);

  decsera(box.top, box.left, box.bottom, box.right);  /* erase the border */

  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  tprintf("Border %d,%d - %d,%d is cleared using DECSERA\n",
          box.top,
          box.left,
          box.bottom,
          box.right);
  holdit();

  /*
   * Part 2: clear within the borders instead of clearing the borders.
   */
  fill_screen();

  test_with_margins(1);

  set_colors(WHITE_ON_GREEN);

  /*
   * Protect a rectangle and overwrite an inner rectangle which is not
   * protected.
   *
   * That way (since the SGR color at this point differs from the color used to
   * fill the screen), we can see whether the colors are modified by the erase,
   * and if so, whether they come from the SGR color.
   */
  decsca(1);
  decfra('*', box.top, box.left, box.bottom, box.right);
  decsca(0);
  decfra('*', box.top + 1, box.left + 1, box.bottom - 1, box.right - 1);

  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  tprintf("Rectangle %d,%d - %d,%d was filled using DECFRA\n",
          box.top,
          box.left,
          box.bottom,
          box.right);
  holdit();

  test_with_margins(2);   /* reenable but do not paint margins */

  set_colors(WHITE_ON_BLUE);

  decsera(box.top, box.left, box.bottom, box.right);  /* erase inside border */

  sgr("0");

  test_with_margins(0);

  vt_move(last, 1);
  vt_clear(0);

  println(the_title);
  tprintf("Inside %d,%d - %d,%d is cleared using DECSERA\n",
          box.top + 1,
          box.left + 1,
          box.bottom - 1,
          box.right - 1);

  return MENU_HOLD;
}

static int
tst_DECSNLS(MENU_ARGS)
{
  int rows;
  int row;
  char temp[80];

  vt_move(row = 1, 1);
  println("Testing Select Number of Lines per Screen (DECSNLS)");
  println("");

  for (rows = 48; rows >= 24; rows -= 12) {
    set_tty_raw(TRUE);
    set_tty_echo(FALSE);

    row += 2;
    sprintf(temp, "%d Lines/Screen:", rows);
    fakeio::_fputs(temp, stdout);
    decsnls(rows);
    decrqss("*|");
    chrprint2(get_reply(), row, (int) strlen(temp));
    println("");

    restore_ttymodes();
    holdit();
  }

  return MENU_NOHOLD;
}

#define SOH             1
#define CHK(n)          ((-(n)) & 0xffff)

#define to_fill(ch)     (((ch) == ' ') ? SOH : (ch))
#define from_fill(ch)   (((ch) == SOH) ? ' ' : (ch))

static int
tst_DSR_area_sum(MENU_ARGS, int g)
{
  char buffer[1024];            /* allocate buffer for lines */
  int expected = 0;
  int title_sum = 0;
  int first = TRUE;
  int pid = 1;
  int page = 1;
  int i, j;
  int r, c;
  int rows = 2;                 /* first two rows have known content */
  int ch;
  int full = 0;
  int report_len;
  int mask_bg = (decac_bg >= 0 && decac_bg < 16) ? decac_bg : 0;
  int mask_fg = (decac_fg >= 0 && decac_fg < 16) ? (decac_fg << 4) : 0;
  int ch_1st = g ? 160 : 32;
  int ch_end = g ? 254 : 126;
  char *report;
  char **lines;                 /* keep track of "drawn" characters */
  char temp[80];
  char *temp2;

  /* make an array of blank lines, to track text on the screen */
  if ((lines = (char**)calloc((size_t) max_lines, sizeof(char *))) == NULL)
      no_memory();
  for (r = 0; r < max_lines; ++r) {
    if ((lines[r] = (char*)malloc((size_t) min_cols + 1)) == NULL)
      no_memory();
    memset(lines[r], to_fill(' '), (size_t) min_cols);
    lines[r][min_cols] = '\0';
  }
  sprintf(buffer, fmt_DECCKSR, the_title);
  memcpy(lines[0], buffer, strlen(buffer));

  for (r = 0; r < rows; ++r) {
    for (c = 0; c < min_cols; ++c) {
      ch = from_fill((unsigned char) lines[r][c]);
      expected += ch + mask_fg + mask_bg;
      if (first || (ch != ' '))
        title_sum = expected;
      first = FALSE;
    }
  }

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  vt_move(1, 1);
  fakeio::_fputs(buffer, stdout);

  /* compute a checksum on the title line, which contains some text */
  sprintf(buffer, "%d;%d;1;1;%d;%d*y", pid, page, rows, min_cols);
  do_csi("%s", buffer);
  report = get_reply();
  vt_move(r = 3, c = 10);

  /* like chrprint2, but shadowed into the lines[][] array */
  vt_hilite(TRUE);
  fakeio::_putchar(' ');
  lines[r - 1][c - 1] = ' ';
  report_len = 1;
  temp2 = chrformat(report, c, 1);
  for (i = 0, j = 1; temp2[i] != '\0'; ++i) {
    if (temp2[i] == '\n') {
      vt_move(++r, c);
      j = 0;
    } else {
      fakeio::_putchar(temp2[i]);
      lines[r - 1][c - 1 + j++] = temp2[i];
      ++report_len;
    }
  }
  free(temp2);
  vt_hilite(FALSE);

  show_result("%s", check_DECCKSR(temp, report, pid, CHK(title_sum)));
  lines[r - 1][c - 1 + j++] = ' ';
  for (i = 0; temp[i] != '\0'; ++i) {
    lines[r - 1][c - 1 + j++] = temp[i];
  }

  ch = ch_1st;
  for (c = 4; c < min_cols - 10; c += 12) {
    for (r = 5; r < max_lines - 3; ++r) {
      char *s;

      vt_move(r, c);

      if (ch > ch_end) {
        sgr("1;4");
        sprintf(buffer, "All");
        fakeio::_fputs(buffer, stdout);
        memcpy(&lines[r - 1][c - 1], buffer, strlen(buffer));
        sgr("0;1");
        sprintf(buffer, ": ");
        fakeio::_fputs(buffer, stdout);
        memcpy(&lines[r - 1][c + 2], buffer, strlen(buffer));
        sgr("0");
        /* request checksum of the entire page */
        do_csi("%d;%d*y", pid, page);
      } else {
        sprintf(buffer, "%c %02X ", ch, ch);
        fakeio::_fputs(buffer, stdout);
        memcpy(&lines[r - 1][c - 1], buffer, strlen(buffer));
        /* request checksum of the given cell at (r,c) */
        do_csi("%d;%d;%d;%d;%d;%d*y", pid, page, r, c, r, c);
      }

      /* FIXME - use check_DECCKSR? */
      report = get_reply();
      if ((s = strchr(report, '!')) != NULL
          && (*++s == '~')
          && strlen(++s) > 4) {
        char test[5];

        if (ch > ch_end) {
          int row_limit = r;
          int y, x;

          for (y = 0; y < row_limit; ++y) {
            /*
             * If trimming, count through space after ":" in "All:".
             * Otherwise, count the whole screen.
             */
            int col_limit = (y < (r - 1)) ? min_cols : (c + 4);

            for (x = 0; x < col_limit; ++x) {
              int yx = (unsigned char) lines[y][x];

              if (yx >= ' ') {
                full += yx;
                full += mask_bg;
                full += mask_fg;
              }
              /*
               * Tidy up the logfile (for debugging, turn this off).
               */
              if (yx == SOH + 0)
                lines[y][x] = ' ';
            }
            if (y == 2) {
              /* add attributes for highlighted report-string */
              full += report_len * (chkINVERSE);
            }
            if (y == (r - 1)) {
              /* add attributes for "All:" */
              full += 3 * (chkBOLD | chkUNDERLINE);
              full += 2 * (chkBOLD);
            }
            if (LOG_ENABLED) {
              fakeio::_fprintf(log_fp,
                      NOTE_STR "check %04X %2d:%s\n",
                      CHK(full), y + 1, lines[y]);
            }
          }
          sprintf(test, "%04X", CHK(full));
        } else {
          int part = ch + mask_bg + mask_fg;
          sprintf(test, "%04X", CHK(part));
        }
        if (memcmp(test, s, (size_t) 4)) {
          vt_hilite(TRUE);
          sprintf(buffer, "%.4s", s);
          if (LOG_ENABLED) {
            unsigned actual;
            int ok = sscanf(s, "%x", &actual);
            fakeio::_fprintf(log_fp, NOTE_STR "actual %.4s%s\n",
                    s, ok ? "" : " (ERR)");
            fakeio::_fprintf(log_fp, NOTE_STR "expect %.4s\n", test);
          }
          fakeio::_fputs(buffer, stdout);
          vt_hilite(FALSE);
        } else {
          sprintf(buffer, "%.4s", test);
          fakeio::_fputs(buffer, stdout);
        }
        memcpy(&lines[r - 1][c + 4], buffer, (size_t) 4);
      }
      ++ch;
      if (ch > ch_end + 1)
        break;
    }
    if (ch > ch_end)
      break;
  }
  restore_ttymodes();
  vt_move(max_lines - 2, 1);
  printxx("Checksum for individual character is highlighted if mismatched.");

  for (r = 0; r < max_lines; r++) {
    free(lines[r]);
  }
  free(lines);

  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

static int
tst_DSR_area_sum_gl(MENU_ARGS)
{
  return tst_DSR_area_sum(PASS_ARGS, 0);
}

static int
tst_DSR_area_sum_gr(MENU_ARGS)
{
  switch (get_level()) {
  case 0:
  case 1:
    break;
  case 2:
    esc(")A");  /* select the 94-character NRCS, closest to MCS */
    break;
  default:
    esc("-A");  /* select the 96-character set */
    break;
  }
  esc("~");
  return tst_DSR_area_sum(PASS_ARGS, 1);
}

static int
tst_DSR_data_ok(MENU_ARGS)
{
  return any_DSR(PASS_ARGS, "?75n", show_DataIntegrity);
}

static int
tst_DSR_macrospace(MENU_ARGS)
{
  int row, col;
  char *report;
  const char *show;

  vt_move(1, 1);
  printxx("Testing DECMSR: %s\n", the_title);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  do_csi("?62n");
  report = get_reply();
  vt_move(row = 3, col = 10);
  chrprint2(report, row, col);
  if ((report = skip_csi(report)) != NULL
      && (report = skip_digits(report)) != NULL
      && !strcmp(report, "*{")) {
    show = SHOW_SUCCESS;
  } else {
    show = SHOW_FAILURE;
  }
  show_result("%s", show);

  restore_ttymodes();
  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

static int
tst_DSR_memory_sum(MENU_ARGS)
{
  return tst_DECCKSR(PASS_ARGS, 1, "?63;1n", -1);
}

static int
tst_DSR_multisession(MENU_ARGS)
{
  return any_DSR(PASS_ARGS, "?85n", show_MultisessionStatus);
}

int
tst_SRM(MENU_ARGS)
{
  int oldc, newc;

  vt_move(1, 1);
  println(the_title);

  set_tty_raw(TRUE);

  set_tty_echo(FALSE);
  srm(FALSE);

  println("Local echo is enabled, remote echo disabled.  Press any keys, repeat to quit.");
  vt_move(3, 10);

  oldc = -1;
  pause_replay();
  while ((newc = inchar()) != oldc && newc > 0)
    oldc = newc;
  resume_replay();

  set_tty_echo(TRUE);
  srm(TRUE);

  vt_move(10, 1);
  println("Local echo is disabled, remote echo enabled.  Press any keys, repeat to quit.");
  vt_move(11, 10);

  oldc = -1;
  pause_replay();
  while ((newc = inchar()) != oldc && newc > 0)
    oldc = newc;
  resume_replay();

  vt_move(max_lines - 1, 1);
  restore_ttymodes();
  return MENU_HOLD;
}

/******************************************************************************/

void
setup_vt420_cursor(MENU_ARGS)
{
  tb_marg_flag = marNone;
  toggle_STBM(PASS_ARGS);

  lr_marg_flag = marNone;
  toggle_SLRM(PASS_ARGS);
}

void
menus_vt420_cursor(void)
{
  sprintf(origin_mode_mesg, "%s DECOM (origin mode)", STR_ENABLE(origin_mode));
  sprintf(lrmm_mesg, "%s DECLRMM (left/right mode)", STR_ENABLE(lrmm_flag));
  sprintf(txt_override_color, "%s test-regions (xterm)",
          do_colors ? "Color" : "Do not color");
}

void
finish_vt420_cursor(MENU_ARGS)
{
  reset_colors();
  do_colors = FALSE;

  if (tb_marg_flag > marReset)
    decstbm(0, 0);

  if (lr_marg_flag > marReset) {
    if (!lrmm_flag)
      toggle_LRMM(PASS_ARGS);
    decslrm(0, 0);
  }

  if (lrmm_flag)
    toggle_LRMM(PASS_ARGS);

  if (origin_mode) {
    decom(FALSE);
    origin_mode = FALSE;
  }
}

/*
 * The main vt100 module tests CUP, HVP, CUF, CUB, CUU, CUD
 */
int
tst_vt420_cursor(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test VT320 features",                               tst_vt320_cursor },
      { origin_mode_mesg,                                    toggle_DECOM },
      { lrmm_mesg,                                           toggle_LRMM },
      { tb_marg_mesg,                                        toggle_STBM },
      { lr_marg_mesg,                                        toggle_SLRM },
      { txt_override_color,                                  toggle_color_mode, },
      { "Test Back Index (DECBI)",                           tst_DECBI },
      { "Test Forward Index (DECFI)",                        tst_DECFI },
      { "Test cursor movement within margins",               tst_cursor_margins },
      { "Test other movement (CR/HT/LF/FF) within margins",  tst_other_margins },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  setup_vt420_cursor(PASS_ARGS);

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Cursor-Movement Tests"));
    __(title(2), println("Choose test type:"));
    menus_vt420_cursor();
  } while (menu(my_menu));

  finish_vt420_cursor(PASS_ARGS);

  return MENU_NOHOLD;
}

/******************************************************************************/

static int
show_DECSTBM(MENU_ARGS)
{
  int code;
  decstbm(tb_marg1, tb_marg2);
  code = rpt_DECSTBM(PASS_ARGS);
  return code;
}

static int
show_DECSLRM(MENU_ARGS)
{
  int code;
  decslrm(lr_marg1, lr_marg2);
  code = rpt_DECSLRM(PASS_ARGS);
  return code;
}

/*
 * The main vt100 module tests IRM, DL, IL, DCH, ICH, ED, EL
 * The vt220 module tests ECH and DECSCA
 */
static int
tst_VT420_editing(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { origin_mode_mesg,                                    toggle_DECOM },
      { lrmm_mesg,                                           toggle_LRMM },
      { tb_marg_mesg,                                        toggle_STBM },
      { lr_marg_mesg,                                        toggle_SLRM },
      { txt_override_color,                                  toggle_color_mode, },
      { "Show DECRQM response for DECLRMM",                  show_DECLRMM },
      { "Show DECRQSS response for DECSTBM",                 show_DECSTBM },
      { "Show DECRQSS response for DECSLRM",                 show_DECSLRM },
      { "Test insert/delete column (DECIC, DECDC)",          tst_DECIC_DECDC },
      { "Test vertical scrolling (IND, RI)",                 tst_IND_RI },
      { "Test insert/delete line (IL, DL)",                  tst_IL_DL },
      { "Test insert/delete char (ICH, DCH)",                tst_ICH_DCH },
      { "Test ASCII formatting (BS, CR, TAB)",               tst_ASCII_format },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  setup_vt420_cursor(PASS_ARGS);

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Editing Sequence Tests"));
    __(title(2), println("Choose test type:"));
    menus_vt420_cursor();
  } while (menu(my_menu));

  finish_vt420_cursor(PASS_ARGS);

  return MENU_NOHOLD;
}

/******************************************************************************/

/*
 * The main vt100 module tests LNM, DECKPAM, DECARM, DECAWM
 */
static int
tst_VT420_keyboard_ctl(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test Backarrow key (DECBKM)",                       tst_DECBKM },
      { "Test Numeric keypad (DECNKM)",                      tst_DECNKM },
      { "Test Keyboard usage (DECKBUM)",                     tst_DECKBUM },
      { "Test Key position (DECKPM)",                        tst_DECKPM },
      { "Test Enable Local Functions (DECELF)",              not_impl },
      { "Test Local Function-Key Control (DECLFKC)",         not_impl },
      { "Test Select Modifier-Key Reporting (DECSMKR)",      not_impl }, /* DECEKBD */
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Keyboard-Control Tests"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/******************************************************************************/

static int
tst_VT420_rectangle(MENU_ARGS)
{
  static char txt_override_lines[80];
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { origin_mode_mesg,                                    toggle_DECOM },
      { lrmm_mesg,                                           toggle_LRMM },
      { tb_marg_mesg,                                        toggle_STBM },
      { lr_marg_mesg,                                        toggle_SLRM },
      { txt_override_color,                                  toggle_color_mode, },
      { txt_override_lines,                                  toggle_lines_mode, },
      { "Test Change-Attributes in Rectangular Area (DECCARA)", tst_DECCARA },
      { "Test Copy Rectangular area (DECCRA)",               tst_DECCRA },
      { "Test Erase Rectangular area (DECERA)",              tst_DECERA },
      { "Test Fill Rectangular area (DECFRA)",               tst_DECFRA },
      { "Test Reverse-Attributes in Rectangular Area (DECRARA)", tst_DECRARA },
      { "Test Selective-Erase Rectangular area (DECSERA)",   tst_DECSERA },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  setup_vt420_cursor(PASS_ARGS);

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Rectangular Area Tests%s",
                         ((terminal_id() < 400)
                          ? " (should not work)"
                          : "")));
    __(title(2), println("Choose test type:"));
    menus_vt420_cursor();
    sprintf(txt_override_lines, "%s line-drawing characters",
            do_lines ? "Use" : "Do not use");
  } while (menu(my_menu));

  finish_vt420_cursor(PASS_ARGS);

  return MENU_NOHOLD;
}

/******************************************************************************/

/* UDK and rectangle-checksum status are available only on VT400 */

int
tst_vt420_device_status(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test VT320 features",                               tst_vt320_device_status },
      { "Test Printer Status",                               tst_DSR_printer },
      { "Test UDK Status",                                   tst_DSR_userkeys },
      { "Test Keyboard Status",                              tst_DSR_keyboard },
      { "Test Macro Space",                                  tst_DSR_macrospace },
      { "Test Memory Checksum",                              tst_DSR_memory_sum },
      { "Test Data Integrity",                               tst_DSR_data_ok },
      { "Test Multiple Session Status",                      tst_DSR_multisession },
      { "Test Checksum of Rectangular Area (DECRQCRA): GL",  tst_DSR_area_sum_gl },
      { "Test Checksum of Rectangular Area (DECRQCRA): GR",  tst_DSR_area_sum_gr },
      { "Test Extended Cursor-Position (DECXCPR)",           tst_DSR_cursor },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Device Status Reports (DSR)"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/******************************************************************************/

int
tst_vt420_report_presentation(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test VT320 features",                               tst_vt320_report_presentation },
      { "Request Mode (DECRQM)/Report Mode (DECRPM)",        tst_DECRPM },
      { "Status-String Report (DECRQSS)",                    tst_vt420_DECRQSS },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  int old_DECRPM = set_DECRPM(4);

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Presentation State Reports"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  set_DECRPM(old_DECRPM);
  return MENU_NOHOLD;
}

int
tst_vt420_reports(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test VT320 features",                               tst_vt320_reports },
      { "Test Presentation State Reports",                   tst_vt420_report_presentation },
      { "Test Device Status Reports (DSR)",                  tst_vt420_device_status },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Reports"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/******************************************************************************/

static int
tst_VT420_screen(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test VT320 features",                               tst_vt320_screen },
      { "Test Select Number of Lines per Screen (DECSNLS)",  tst_DECSNLS },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Screen-Display Tests"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/******************************************************************************/

/*
 * These apply only to VT400's & above.
 */
int
tst_vt420(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test VT320 features",                               tst_vt320 },
      { "Test cursor-movement",                              tst_vt420_cursor },
      { "Test editing sequences",                            tst_VT420_editing },
      { "Test keyboard-control",                             tst_VT420_keyboard_ctl },
      { "Test macro-definition (DECDMAC)",                   not_impl },
      { "Test rectangular area functions",                   tst_VT420_rectangle },
      { "Test reporting functions",                          tst_vt420_reports },
      { "Test screen-display functions",                     tst_VT420_screen },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("VT420 Tests"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}
