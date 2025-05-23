/* $Id: vt220.c,v 1.44 2024/12/05 00:40:24 tom Exp $ */

/*
 * Reference:  VT220 Programmer Pocket Guide (EK-VT220-HR-002).
 * Reference:  VT220 Program Reference Manual (EK-VT220-RM-002).
 * Reference:  DEC STD 070 Video Systems Reference Manual.
 */
#include "vttest.h"
#include "ttymodes.h"
#include "draw.h"
#include "esc.h"
#include "fakeio.h"
int
any_DSR(MENU_ARGS, const char *text, void (*explain) (const char *report))
{
  int row, col;
  char *report;
  unsigned pmode = (unsigned) ((*text == '?') ? 1 : 0);

  vt_move(1, 1);
  printxx("Testing DSR: %s\n", the_title);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  do_csi("%s", text);
  report = get_reply();
  vt_move(row = 3, col = 10);
  chrprint2(report, row, col);
  if ((report = skip_csi(report)) != NULL
      && strlen(report) > (1 + pmode)
      && (!pmode || (*report++ == '?'))) {
    if (explain != NULL)
      (*explain) (report);
    else
      show_result(SHOW_SUCCESS);
  } else {
    show_result(SHOW_FAILURE);
  }

  restore_ttymodes();
  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

static void
report_ok(const char *ref, const char *tst)
{
  if ((tst = skip_csi_2(tst)) == NULL)
    tst = "?";
  show_result(!strcmp(ref, tst) ? SHOW_SUCCESS : SHOW_FAILURE);
}

/*
 * Request  CSI ? 26 n            keyboard dialect
 * Response CSI ? 27; Ps n
 */
static void
show_KeyboardStatus(const char *report)
{
  int pos = 0;
  int code;
  int save;
  const char *show = SHOW_FAILURE;

  if (scanto(report, &pos, ';') == 27
      && (save = pos) != 0
      && (code = scan_any(report, &pos, 'n')) >= 0
      && (pos != save)) {
    /* *INDENT-OFF* */
    switch(code) {
    case  1:  show = "North American/ASCII";  break;
    case  2:  show = "British";               break;
    case  3:  show = "Flemish";               break;
    case  4:  show = "French Canadian";       break;
    case  5:  show = "Danish";                break;
    case  6:  show = "Finnish";               break;
    case  7:  show = "German";                break;
    case  8:  show = "Dutch";                 break;
    case  9:  show = "Italian";               break;
    case 10:  show = "Swiss (French)";        break;
    case 11:  show = "Swiss (German)";        break;
    case 12:  show = "Swedish";               break;
    case 13:  show = "Norwegian/Danish";      break;
    case 14:  show = "French/Belgian";        break;
    case 15:  show = "Spanish Int.";          break;
    case 16:  show = "Portuguese";            break; /* vt3XX */
    case 17:  show = "Katakana";              break; /* EK-VT382-RM-001 p 11-9 */
    case 19:  show = "Hebrew";                break; /* vt5XX: kermit says 14 */
    case 22:  show = "Greek";                 break; /* vt5XX */
    case 27:  show = "Thai";                  break; /* EK-VT38T-UG-001 p C-42 */
    case 28:  show = "Canadian (English)";    break; /* vt4XX */
    case 29:  show = "Turkish Q/Turkish";     break; /* vt5XX */
    case 30:  show = "Turkish F/Turkish";     break; /* vt5XX */
    case 31:  show = "Hungarian";             break; /* vt5XX */
    case 32:  show = "Spanish National";      break; /* vt4XX in PC mode */
    case 33:  show = "Slovak";                break; /* vt5XX */
    case 34:  show = "Czech";                 break; /* vt5XX */
    case 35:  show = "Polish";                break; /* vt5XX */
    case 36:  show = "Romanian";              break; /* vt5XX */
    case 38:  show = "SCS";                   break; /* vt5XX */
    case 39:  show = "Russian";               break; /* vt5XX */
    case 40:  show = "Latin American";        break; /* vt5XX */
    default:  show = "unknown";
    }
    /* *INDENT-ON* */

  }
  show_result("%s", show);

  /*
   * VT420 implements additional parameters past those reported by the VT220.
   * see:
   *   EK-VT420-RM 002, p 239, 277
   *   EK-VT510-RM B01, p 4-30, 5-166
   *   EK-VT520-RM A01, p 4-28, 5-176
   */
  save = pos;
  code = scan_any(report, &pos, 'n');
  if (save != pos) {
    vt_move(5, 10);
    /* *INDENT-OFF* */
    switch(code) {
    case 0:  show = "keyboard ready";         break;
    case 3:  show = "no keyboard";            break;
    case 8:  show = "keyboard busy";          break;
    default: show = "unknown keyboard status";
    }
    /* *INDENT-ON* */

    show_result("%s", show);

    vt_move(6, 10);
    /* *INDENT-OFF* */
    switch (scan_any(report, &pos, 'n')) {
    case 0:  show = "LK201/LK301";            break;
    case 1:  show = "LK401";                  break;
    case 2:  show = "LK443/LK444";            break;
    case 3:  show = "LK421";                  break;
    case 4:  show = "LK411/LK450";            break;
    case 5:  show = "PCXAL";                  break;
    default: show = "unknown keyboard type";
    }
    /* *INDENT-ON* */

    show_result("%s", show);
  }
}

static void
show_PrinterStatus(const char *report)
{
  int pos = 0;
  int code = scanto(report, &pos, 'n');
  const char *show;
  /* *INDENT-OFF* */
  switch (code) {
  case 13: show = "No printer";               break;
  case 10: show = "Printer ready";            break;
  case 11: show = "Printer not ready";        break;
  case 18: show = "Printer busy";             break;
  case 19: show = "Printer assigned to other session"; break;
  default: show = SHOW_FAILURE;
  }
  /* *INDENT-ON* */

  show_result("%s", show);
}

static void
show_UDK_Status(const char *report)
{
  int pos = 0;
  int code = scanto(report, &pos, 'n');
  const char *show;
  /* *INDENT-OFF* */
  switch(code) {
  case 20: show = "UDKs unlocked";            break;
  case 21: show = "UDKs locked";              break;
  case 23: show = "UDKs unsupported";         break;
  default: show = SHOW_FAILURE;
  }
  /* *INDENT-ON* */

  show_result("%s", show);
}

/* VT220 & up.
 */
static int
tst_S8C1T(MENU_ARGS)
{
  int flag = input_8bits;
  int pass;
  char temp[80];

  vt_move(1, 1);
  println(the_title);

  vt_move(5, 1);
  println("This tests the VT200+ control sequence to direct the terminal to emit 8-bit");
  println("control-sequences instead of <esc> sequences.");

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  for (pass = 0; pass < 2; pass++) {
    const char *report;
    int row, col;

    flag = !flag;
    s8c1t(flag);
    cup(1, 1);
    dsr(6);
    report = instr();
    vt_move(row = 10 + pass * 3, col = 1);
    sprintf(temp, "8-bit controls %s:", STR_ENABLED(flag));
    printxx("%s", temp);
    chrprint2(report, row, col + (int) strlen(temp));
    report_ok("1;1R", report);
  }

  restore_ttymodes();
  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

/*
 * Test DEC's selective-erase (set-protected area) by drawing a box of
 * *'s that will remain, and a big X of *'s that gets cleared..
 */
static void
tst_DECSCA_selective(MENU_ARGS)
{
  BOX inner;

  if (make_box_params(&inner, 6, 20) == 0) {
    int pass;

    for (pass = 0; pass < 2; pass++) {
      int i;

      if (pass == 0)
        decsca(1);

      for (i = inner.top; i <= inner.bottom; i++) {
        int j;

        cup(i - 1, 1 + inner.left);
        for (j = inner.left; j <= inner.right; j++) {
          fakeio::_putchar('*');
        }
      }

      if (pass == 0) {
        int j;

        decsca(0);

        for (j = 0; j <= 2; j++) {
          for (i = 1; i < inner.top - 1; i++) {
            cup(i, inner.left - inner.top + (i + j) + 3);
            fakeio::_putchar('*');
            cup(i, inner.right + inner.top - (i + j) - 1);
            fakeio::_putchar('*');
          }
          for (i = inner.bottom; i < max_lines; i++) {
            cup(i, (inner.left + inner.bottom) - (i - j) + 1);
            fakeio::_putchar('*');
            cup(i, (inner.right - inner.bottom) + (i - j) + 1);
            fakeio::_putchar('*');
          }
          cup((max_lines / 2) - 1, (min_cols / 2) - 1);
          decsed(j);
        }

        for (i = inner.right + 1; i <= min_cols; i++) {
          cup(inner.top + 1, 1 + i);
          fakeio::_putchar('*');
          cup(max_lines / 2 + 1, 1 + i);
          fakeio::_putchar('*');
        }
        cup(max_lines / 2 + 1, 1 + min_cols / 2);
        decsel(0);  /* after the cursor */

        for (i = 1; i < inner.left; i++) {
          cup(inner.top + 1, 1 + i);
          fakeio::_putchar('*');
          cup(max_lines / 2 + 1, 1 + i);
          fakeio::_putchar('*');
        }
        cup(max_lines / 2 + 1, 1 + min_cols / 2);
        decsel(1);  /* before the cursor */

        cup(inner.top + 1, 1 + min_cols / 2);
        decsel(2);  /* the whole line */

        vt_move(max_lines - 3, 1);
        vt_clear(0);
        println("If your terminal supports DEC protected areas (DECSCA, DECSED, DECSEL),");
        println("there will be an solid box made of *'s in the middle of the screen.");
        holdit();
      }
    }
  } else {
    vt_move(max_lines - 3, 1);
    vt_clear(0);
    println("The screen is too small for this test");
    holdit();
  }
}

static void
tst_DECSCA_edit_line(MENU_ARGS)
{
  BOX outer;
  BOX inner;

  if (make_box_params(&outer, 0, 0) == 0 &&
      make_box_params(&inner, 6, 20) == 0) {
    int i;

    decsca(1);

    for (i = outer.top; i <= outer.bottom; i++) {
      int j;

      cup(i + 1, 1 + outer.left);
      for (j = outer.left; j <= outer.right; j++) {
        fakeio::_putchar((i >= inner.top &&
                 i <= inner.bottom &&
                 j >= inner.left &&
                 j <= inner.right) ? '*' : '#');
      }
    }
    cup(inner.top - 2, max_cols);
    ed(1);
    cup(inner.bottom, 1);
    ed(0);
    for (i = outer.top; i <= outer.bottom; i++) {
      cup(i + 1, inner.left);
      el(1);
      cup(i + 1, inner.right + 2);
      el(0);
    }

    decsca(0);
    vt_move(max_lines - 3, 1);
    vt_clear(0);
    println("If your terminal supports DEC protected areas (DECSCA, ignoring EL/ED),");
    println("there will be an solid box made of *'s in the middle of the screen.");
  } else {
    vt_move(max_lines - 3, 1);
    vt_clear(0);
    println("The screen is too small for this test");
  }
  holdit();
}

static void
tst_DECSCA_edit_char(MENU_ARGS)
{
  BOX outer;
  BOX inner;

  if (make_box_params(&outer, 0, 0) == 0 &&
      make_box_params(&inner, 6, 20) == 0) {
    int i;

    decsca(1);

    for (i = outer.top; i <= outer.bottom; i++) {
      int j;

      cup(i + 1, 1 + outer.left);
      for (j = outer.left; j <= outer.right; j++) {
        fakeio::_putchar((i >= inner.top &&
                 i <= inner.bottom &&
                 j >= inner.left &&
                 j <= inner.right) ? '*' : '#');
      }
    }
    for (i = 1; i < inner.top - 1; ++i) {
      cup(i, 1);
      ech(min_cols);
    }
    for (i = inner.top; i <= inner.bottom; i++) {
      cup(i - 1, 1);
      ech(inner.left);
      cup(i - 1, inner.right + 2);
      if (i % 2) {
        ich(19);
      } else {
        dch(19);
      }
    }
    for (i = inner.bottom; i < outer.bottom; ++i) {
      cup(i, 1);
      dch(min_cols);
    }
    decsca(0);

    vt_move(max_lines - 3, 1);
    vt_clear(0);
    println("If your terminal supports DEC protected areas (DECSCA, ignoring ECH/ICH/DCH),");
    println("there will be an solid box made of *'s in the middle of the screen.");
  } else {
    vt_move(max_lines - 3, 1);
    vt_clear(0);
    println("The screen is too small for this test");
  }
  holdit();
}

static int
tst_DECSCA(MENU_ARGS)
{
  tst_DECSCA_selective(PASS_ARGS);
  tst_DECSCA_edit_line(PASS_ARGS);
  tst_DECSCA_edit_char(PASS_ARGS);
  return MENU_NOHOLD;
}

/*
 * VT220 & up
 *
 * Test if the terminal can make the cursor invisible
 */
static int
tst_DECTCEM(MENU_ARGS)
{
  vt_move(1, 1);
  rm("?25");
  println("The cursor should be invisible");
  holdit();
  sm("?25");
  println("The cursor should be visible again");
  return MENU_HOLD;
}

static int
tst_DECUDK(MENU_ARGS)
{
  int key;
  /* *INDENT-OFF* */
  static struct {
    int code;
    const char *name;
  } keytable[] = {
    /* xterm programs these: */
    { 11, "F1" },
    { 12, "F2" },
    { 13, "F3" },
    { 14, "F4" },
    { 15, "F5" },
    /* vt420 programs these: */
    { 17, "F6" },
    { 18, "F7" },
    { 19, "F8" },
    { 20, "F9" },
    { 21, "F10" },
    { 23, "F11" },
    { 24, "F12" },
    { 25, "F13" },
    { 26, "F14" },
    { 28, "F15" },
    { 29, "F16" },
    { 31, "F17" },
    { 32, "F18" },
    { 33, "F19" },
    { 34, "F20" } };
  /* *INDENT-ON* */

  for (key = 0; key < TABLESIZE(keytable); key++) {
    char temp[80];
    const char *s;
    temp[0] = '\0';
    for (s = keytable[key].name; *s; s++)
      sprintf(temp + strlen(temp), "%02x", *s & 0xff);
    do_dcs("1;1|%d/%s", keytable[key].code, temp);
  }

  vt_move(1, 1);
  println(the_title);
  println("Press 'q' to quit.  Function keys should echo their labels.");
  println("(On a DEC terminal you must press SHIFT as well).");

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  for (;;) {
    int row, col;
    const char *report = instr();

    if (*report == 'q')
      break;
    vt_move(row = 5, col = 10);
    vt_clear(0);
    chrprint2(report, row, col);
  }

  do_dcs("0");  /* clear all keys */

  restore_ttymodes();
  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

/* vt220 & up */
int
tst_DSR_keyboard(MENU_ARGS)
{
  return any_DSR(PASS_ARGS, "?26n", show_KeyboardStatus);
}

int
tst_DSR_printer(MENU_ARGS)
{
  return any_DSR(PASS_ARGS, "?15n", show_PrinterStatus);
}

int
tst_DSR_userkeys(MENU_ARGS)
{
  return any_DSR(PASS_ARGS, "?25n", show_UDK_Status);
}

static void
show_OperatingStatus(const char *report)
{
  int pos = 0;
  int Ps1 = scan_any(report, &pos, 'n');
  int Ps2 = scanto(report, &pos, 'n');

  switch (Ps1) {
  case 0:
    show_result("Terminal is in good operating condition: %d", Ps2);
    break;
  case 3:
    show_result("Terminal has a malfunction: %d", Ps2);
    break;
  default:
    show_result(SHOW_FAILURE);
  }
}

static int
tst_DSR_operating_status(MENU_ARGS)
{
  return any_DSR(PASS_ARGS, "5n", show_OperatingStatus);
}

/*
 * VT200 and up
 *
 * Test to ensure that 'ech' (erase character) is honored, with no parameter,
 * explicit parameter, and longer than the screen width (to ensure that the
 * terminal doesn't try to wrap-around the erasure).
 */
static int
tst_ECH(MENU_ARGS)
{
  int i;
  int last = max_lines - 4;

  decaln();
  for (i = 1; i <= max_lines; i++) {
    cup(i, min_cols - i - 2);
    do_csi("X");  /* make sure default-parameter works */
    cup(i, min_cols - i - 1);
    fakeio::_putchar('*');
    ech(min_cols);
    fakeio::_putchar('*');   /* this should be adjacent, in the upper-right corner */
  }

  vt_move(last, 1);
  vt_clear(0);

  vt_move(last, min_cols - (last + 10));
  println("diagonal: ^^ (clear)");
  println("ECH test: there should be E's with a gap before diagonal of **'s");
  println("The lower-right diagonal region should be cleared.  Nothing else.");
  return MENU_HOLD;
}

/******************************************************************************/

int
tst_vt220_device_status(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test Keyboard Status",                              tst_DSR_keyboard },
      { "Test Operating Status",                             tst_DSR_operating_status },
      { "Test Printer Status",                               tst_DSR_printer },
      { "Test UDK Status",                                   tst_DSR_userkeys },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    title(0);
    printxx("VT220 Device Status Reports");
    title(2);
    println("Choose test type:");
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/******************************************************************************/

int
tst_vt220_screen(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test Send/Receive mode (SRM)",                      tst_SRM },
      { "Test Visible/Invisible Cursor (DECTCEM)",           tst_DECTCEM },
      { "Test Erase Char (ECH)",                             tst_ECH },
      { "Test Protected-Areas (DECSCA)",                     tst_DECSCA },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    title(0);
    printxx("VT220 Screen-Display Tests");
    title(2);
    println("Choose test type:");
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/******************************************************************************/

int
tst_vt220_reports(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test Device Status Report (DSR)",                   tst_vt220_device_status },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("VT220 Reports"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}

/******************************************************************************/

int
tst_vt220(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                              NULL },
      { "Test reporting functions",                          tst_vt220_reports },
      { "Test screen-display functions",                     tst_vt220_screen },
      { "Test 8-bit controls (S7C1T/S8C1T)",                 tst_S8C1T },
      { "Test Printer (MC)",                                 tst_printing },
      { "Test Soft Character Sets (DECDLD)",                 tst_softchars },
      { "Test Soft Terminal Reset (DECSTR)",                 tst_DECSTR },
      { "Test User-Defined Keys (DECUDK)",                   tst_DECUDK },
      { "",                                                  NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    title(0);
    printxx("VT220 Tests");
    title(2);
    println("Choose test type:");
  } while (menu(my_menu));
  return MENU_NOHOLD;
}
