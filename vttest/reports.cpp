/* $Id: reports.c,v 1.57 2024/12/05 00:37:39 tom Exp $ */

#include "vttest.h"
#include "ttymodes.h"
#include "esc.h"
#include <cctype>
#include "fakeio.h" /* *INDENT-OFF* */
static
struct table {
    int key;
    const char *msg;
} paritytable[] = {
    { 1, "NONE" },
    { 4, "ODD"  },
    { 5, "EVEN" },
    { -1, "" }
},nbitstable[] = {
    { 1, "8" },
    { 2, "7" },
    { -1,"" }
},speedtable[] = {
    {   0,    "50" },
    {   8,    "75" },
    {  16,   "110" },
    {  24,   "134.5"},
    {  32,   "150" },
    {  40,   "200" },
    {  48,   "300" },
    {  56,   "600" },
    {  64,  "1200" },
    {  72,  "1800" },
    {  80,  "2000" },
    {  88,  "2400" },
    {  96,  "3600" },
    { 104,  "4800" },
    { 112,  "9600" },
    { 120, "19200" },
    { 128, "38400" },
    { -1, "" }
},operating_level[] = {
    {   6, "VT102" },
    {  12, "VT125" },
    {  61, "VT100 family" },
    {  62, "VT200 family" },
    {  63, "VT300 family" },
    {  64, "VT400 family" },
    {  65, "VT500 family" },
    {  -1, "" }
},extensions[] = {
    {   1, "132 columns" },                             /* vt400, vt382 */
    {   2, "printer port" },                            /* vt400, vt382 */
    {   3, "ReGIS graphics" },                          /* kermit */
    {   4, "Sixel graphics" },                          /* vt382, kermit */
    {   5, "Katakana extension" },                      /* vt382 */
    {   6, "selective erase" },                         /* vt400, vt382 */
    {   7, "soft character set (DRCS)" },               /* vt400 */
    {   8, "user-defined keys (UDK)" },                 /* vt400, vt382 */
    {   9, "national replacement character-sets" },     /* kermit */
    {  10, "Two-bytes Kanji" },                         /* vt382 */
    {  11, "25th status line" },                        /* ? */
    {  12, "Serbo-Croatian (SCS)" },                    /* vt500 */
    {  13, "local editing mode" },                      /* kermit */
    {  14, "8-bit architecture" },                      /* ? */
    {  15, "DEC technical set" },                       /* vt400, vt382 */
    {  16, "locator device port (ReGIS)" },             /* kermit */
    {  17, "terminal state reports" },                  /* ? */
    {  18, "user windows" },                            /* vt400 */
    {  19, "two sessions" },                            /* vt400 */
    {  20, "APL" },                                     /* vt400? */
    {  21, "horizontal scrolling" },                    /* vt400 */
    {  22, "color" },                                   /* vt500 */
    {  23, "Greek" },                                   /* vt500 */
    {  24, "Turkish" },                                 /* vt500 */
    {  25, "Arabic bilingual M1" },                     /* vt400? */
    {  26, "Arabic bilingual M2" },                     /* vt400? */
    {  27, "Arabic bilingual M3" },                     /* vt400? */
    {  28, "rectangular editing" },                     /* vt400 */
    {  29, "ANSI text locator" },                       /* DXterm */
    {  30, "Hanzi" },                                   /* vt400? */
    {  32, "text macros" },                             /* vt400? */
    {  33, "Hangul Hanja" },                            /* vt400? */
    {  34, "Icelandic" },                               /* vt400? */
    {  35, "Arabic bilingual TC" },                     /* vt400? */
    {  36, "Arabic bilingual no TC" },                  /* vt400? */
    {  36, "Arabic bilingual no TC" },                  /* vt400? */
    {  37, "Thai" },                                    /* vt400? */
    {  38, "character outlining" },                     /* vt400? */
    {  42, "ISO Latin-2" },                             /* vt500 */
    {  44, "PC Term" },                                 /* vt500 */
    {  45, "soft key mapping" },                        /* vt500 */
    {  46, "ASCII Terminal emulation (WYSE,TVI,...)" }, /* vt500 */
    {  -1, "" }
};
/* *INDENT-ON* */

static int
legend(int n, const char *input, const char *word, const char *description)
{
  int i;
  size_t len = strlen(word);
  char buf[BUF_SIZE];

  for (i = 0; input[i] != 0; i++) {
    if ((i == 0 || !isalpha(CharOf(input[i - 1])))
        && !strncmp(word, input + i, len)) {
      sprintf(buf, "%-8s %-3s = %s", n ? "" : "Legend:", word, description);
      show_result("%s", buf);
      fakeio::_putchar('\n');
      return n + 1;
    }
  }
  return n;
}

static const char *
lookup(struct table t[], int k)
{
  int i;
  for (i = 0; t[i].key != -1; i++) {
    if (t[i].key == k)
      return (t[i].msg);
  }
  return ("BAD VALUE");
}

static void
report_is(const char *report, int row, int col)
{
  const char *tag = "Report is:";
  vt_move(row, col);
  vt_el(0);
  tprintf("%s", tag);
  chrprint2(report, row, col + (int) strlen(tag));
}

/******************************************************************************/

static int
tst_DA(MENU_ARGS)
{
  int found;
  const char *report, *cmp;
  /* *INDENT-OFF* */
  static const char *attributes[][2] = { /* after CSI */
    { "?1;0c",   "No options (vanilla VT100)" },
    { "?1;1c",   "VT100 with STP" },
    { "?1;2c",   "VT100 with AVO (could be a VT102)" },
    { "?1;3c",   "VT100 with STP and AVO" },
    { "?1;4c",   "VT100 with GPO" },
    { "?1;5c",   "VT100 with STP and GPO" },
    { "?1;6c",   "VT100 with AVO and GPO" },
    { "?1;7c",   "VT100 with STP, AVO and GPO" },
    { "?1;11c",  "VT100 with PP and AVO" },
    { "?1;15c",  "VT100 with PP, GPO and AVO" },
    { "?2c",     "VT102" },
    { "?4;2c",   "VT132 with AVO" },
    { "?4;3c",   "VT132 with AVO and STP" },
    { "?4;6c",   "VT132 with GPO and AVO" },
    { "?4;7c",   "VT132 with GPO, AVO, and STP" },
    { "?4;11c",  "VT132 with PP and AVO" },
    { "?4;15c",  "VT132 with PP, GPO and AVO" },
    { "?6c",     "VT102" },
    { "?7c",     "VT131" },
    { "?12;5c",  "VT125" },           /* VT125 also has ROM version */
    { "?12;7c",  "VT125 with AVO" },  /* number, so this won't work */
    { "?5;0c",   "VK100 (GIGI)" },
    { "?5c",     "VK100 (GIGI)" },
    { "?62;1;2;4;6;8;9;15c",       "VT220" },
    { "?63;1;2;8;9c",              "VT320" },
    { "?63;1;2;4;6;8;9;15c",       "VT320" },
    { "?63;1;3;4;6;8;9;15;16;29c", "DXterm" },
    { "", "" }
  };
  /* *INDENT-ON* */

  vt_move(1, 1);
  println("Test of Device Attributes report (what are you)");

  set_tty_raw(TRUE);
  da();
  report = get_reply();

  report_is(report, 3, 1);

  found = FALSE;

  if ((cmp = skip_csi_2(report)) != NULL) {
    int i;

    for (i = 0; *attributes[i][0] != '\0'; i++) {
      if (!strcmp(cmp, attributes[i][0])) {
        int n = 0;
        show_result(" -- means %s", attributes[i][1]);
        fakeio::_putchar('\n');
        n = legend(n, attributes[i][1], "STP", "Processor Option");
        n = legend(n, attributes[i][1], "AVO", "Advanced Video Option");
        n = legend(n, attributes[i][1], "GPO", "Graphics Processor Option");
        (void) legend(n, attributes[i][1], "PP", "Printer Port");
        found = TRUE;
        break;
      }
    }
  }

  if (!found) { /* this could be a vt200+ with some options disabled */
    if (cmp != NULL && *cmp == '?') {
      int reportpos = 1;
      int value = scan_DA(cmp, &reportpos);
      show_result("%s\n", lookup(operating_level, value));
      fakeio::_putchar('\n');
      if (value == 12) {
        if ((value = scan_DA(cmp, &reportpos)) >= 0) {
          fakeio::_fputs("   ", stdout);
          switch (value) {
          case 2:
            show_result("no STP, AVO, no GPO (ReGIS)");
            break;
          case 3:
            show_result("no STP, AVO, GPO (ReGIS)");
            break;
          case 5:
            show_result("no AVO");
            break;
          case 7:
            show_result("with AVO");
            break;
          default:
            printxx("unknown code %d", value);
            break;
          }
          fakeio::_putchar('\n');
        }
        if ((value = scan_DA(cmp, &reportpos)) >= 0) {
          fakeio::_fputs("   ", stdout);
          switch (value) {
          case 0:
            show_result("no printer");
            break;
          case 1:
            show_result("with printer");
            break;
          default:
            printxx("unknown code %d", value);
            break;
          }
          fakeio::_putchar('\n');
        }
        if ((value = scan_DA(cmp, &reportpos)) >= 0) {
          tprintf("    ROM version %d", value);
          fakeio::_putchar('\n');
        }
      } else {
        while ((value = scan_DA(cmp, &reportpos)) >= 0) {
          fakeio::_fputs("   ", stdout);
          show_result("%d = %s\n", value, lookup(extensions, value));
          fakeio::_putchar('\n');
        }
      }
      found = TRUE;
    }
  }
  if (!found)
    show_result(" -- Unknown response, refer to the manual");

  restore_ttymodes();

  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

/*
 * Applies to VT220 & up (probably no VT100's).
 * Expected reply (from VT420 manual):
 *      CSI > 4 1 ; Pv ; 0 c (Pv = firmware version)
 * From kermit:
 *      CSI > 2 4 ; Pv ; 0 c (Pv = "0 ; 0 ; 0", for "0.0")
 * I've seen also:
 *      CSI > 8 3 ; Pv ; 0 c  (e.g., Pv = "3 0 7 0 1")
 */
static int
tst_DA_2(MENU_ARGS)
{
  int row, col;
  /* *INDENT-OFF* */
  static const struct {
    int Pp;
    const char *name;
  } tbl[] = {
    {  1,  "VT220" },
    {  2,  "VT240" },
    { 18,  "VT330" },
    { 19,  "VT340" },
    { 24,  "VT320" },
    { 32,  "VT382" },   /* EK-VT382-RM-001, page 11-5 */
    { 48,  "VT382" },   /* EK-VT38T-UG-001, page C-40 */
    { 28,  "DECterm" },
    { 41,  "VT420" },
    { 61,  "VT510" },
    { 64,  "VT520" },
    { 65,  "VT525" },
  };
  /* *INDENT-ON* */

  char *report;
  int Pp, Pv, Pc;
  char ch;
  const char *show = SHOW_FAILURE;

  vt_move(1, 1);
  println("Testing Secondary Device Attributes (Firmware version)");

  set_tty_raw(TRUE);
  do_csi(">c"); /* or "CSI > 0 c" */
  report = get_reply();

  vt_move(row = 3, col = 10);
  chrprint2(report, row, col);

  if ((report = skip_csi(report)) != NULL) {
    if (sscanf(report, ">%d;%d;%d%c", &Pp, &Pv, &Pc, &ch) == 4
        && ch == 'c') {
      const char *name = "unknown";
      size_t n;

      show = SHOW_SUCCESS;
      for (n = 0; n < TABLESIZE(tbl); n++) {
        if (Pp == tbl[n].Pp) {
          name = tbl[n].name;
          break;
        }
      }
      vt_move(4, 10);
      printxx("Pp=%d (%s)", Pp, name);

      vt_move(5, 10);
      printxx("Pv=%d, firmware version %d.%d", Pv, Pv / 10, Pv % 10);

      vt_move(6, 10);
      switch (Pp) {
      case 64:
      case 65:
        printxx("Pc=%d, %s keyboard", Pc, Pc ? "PC" : "VT");
        break;
      default:
        printxx("Pc=%d, ROM cartridge registration number", Pc);
        break;
      }
    }
  }
  show_result("%s", show);

  restore_ttymodes();

  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

/*
 * VT400 (reply is a hexadecimal string)
 */
static int
tst_DA_3(MENU_ARGS)
{
  int row, col;
  char *report;
  const char *show;

  vt_move(1, 1);
  println("Testing Tertiary Device Attributes (unit ID)");

  set_tty_raw(TRUE);
  do_csi("=c"); /* or "CSI = 0 c" */
  report = get_reply();

  vt_move(row = 3, col = 10);
  chrprint2(report, row, col);
  show = SHOW_FAILURE;
  if (report == NULL || *report == '\0') {
    if (get_level() < 4)  /* vt420 and up support this */
      show = "not supported";
  } else {
    char *content;
    if ((content = skip_dcs(report)) != NULL
        && strip_terminator(content) != 0
        && *content++ == '!'
        && *content++ == '|'
        && strlen(content) != 0) {
      char *check;
      if (strlen(content) == 8) {
        (void) strtol(content, &check, 16);
        if (*check == '\0')
          show = SHOW_SUCCESS;
      }
    }
  }
  show_result("%s", show);

  restore_ttymodes();

  vt_move(max_lines - 1, 1);
  return MENU_HOLD;
}

/* Not supported above VT320 */
static int
tst_DECREQTPARM(MENU_ARGS)
{
  char *report, *report2, *cmp;

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);

  vt_move(2, 1);
  println("Test of the \"Request Terminal Parameters\" feature, argument 0.");

  vt_move(3, 1);
  decreqtparm(0);
  report = get_reply();

  report_is(report, 5, 1);

  if ((cmp = skip_csi(report)) != NULL)
    report = cmp;

  if (strlen(report) < 14
      || report[0] != '2'
      || report[1] != ';')
    println(" -- Bad format");
  else {
    int reportpos = 2;
    /* *INDENT-EQLS* */
    int parity = scanto(report, &reportpos, ';');
    int nbits  = scanto(report, &reportpos, ';');
    int xspeed = scanto(report, &reportpos, ';');
    int rspeed = scanto(report, &reportpos, ';');
    int clkmul = scanto(report, &reportpos, ';');
    int flags  = scanto(report, &reportpos, 'x');

    if (parity <= 0 || nbits <= 0 || clkmul <= 0)
      println(" -- Bad format");
    else
      println(" -- OK");

    show_result(
                 "This means: Parity %s, %s bits, xmitspeed %s, recvspeed %s.\n",
                 lookup(paritytable, parity),
                 lookup(nbitstable, nbits),
                 lookup(speedtable, xspeed),
                 lookup(speedtable, rspeed));
    show_result("(CLoCk MULtiplier = %d, STP option flags = %d)\n", clkmul, flags);
  }

  vt_move(10, 1);
  println("Test of the \"Request Terminal Parameters\" feature, argument 1.");

  vt_move(11, 1);
  decreqtparm(1);   /* Does the same as decreqtparm(0), reports "3" */
  report2 = get_reply();

  report_is(report2, 13, 1);

  if ((cmp = skip_csi(report2)) != NULL)
    report2 = cmp;

  if (strlen(report2) < 1
      || report2[0] != '3')
    println(" -- Bad format");
  else {
    report2[0] = '2';
    if (!strcmp(report, report2))
      println(" -- OK");
    else
      println(" -- Bad format");
  }
  vt_move(max_lines, 1);

  restore_ttymodes();
  return MENU_HOLD;
}

static int
tst_DSR(MENU_ARGS)
{
  int found;
  int origin;
  int row, col;
  char *report;
  const char *cmp;

  set_tty_raw(TRUE);

  vt_move(1, 1);
  printxx("Test of Device Status Report 5 (report terminal status).");

  vt_move(row = 2, col = 1);
  dsr(5);
  report = get_reply();

  report_is(report, row, col);

  if ((cmp = skip_csi(report)) != NULL)
    found = !strcmp(cmp, "0n") || !strcmp(cmp, "3n");
  else
    found = 0;

  if (found)
    show_result(" -- means \"TERMINAL OK\"");
  else
    show_result(" -- Unknown response!");

  vt_move(4, 1);
  println("Test of Device Status Report 6 (report cursor position).");

  for (origin = 0; origin < 2; ++origin) {
    if (origin) {
      sm("?6");
      decstbm(4, max_lines - 6);
    }
    vt_move(row = 5, col = 1);
    dsr(6);
    report = get_reply();

    report_is(report, row, col);

    if ((cmp = skip_csi(report)) != NULL) {
      found = (!strcmp(cmp, "5;1R")
               ? 1
               : ((!strcmp(cmp, "8;1R") && origin)
                  ? 2
                  : 0));
    } else {
      found = 0;
    }

    switch (found) {
    case 2:
      show_result(" -- Ignores origin mode");
      break;
    case 1:
      show_result(" -- OK");
      break;
    default:
      show_result(" -- Unknown response!");
      break;
    }
  }

  rm("?6");
  decstbm(0, 0);

  vt_move(max_lines - 1, 1);
  restore_ttymodes();
  return MENU_HOLD;
}

static int
tst_ENQ(MENU_ARGS)
{
  int row, col;
  const char *report;

  vt_move(5, 1);
  println("This is a test of the ANSWERBACK MESSAGE. (To load the A.B.M.");
  println("see the TEST KEYBOARD part of this program). Below here, the");
  println("current answerback message in your terminal should be");
  println("displayed. Finish this test with RETURN.");

  vt_move(10, 1);

  set_tty_raw(TRUE);
  set_tty_echo(FALSE);
  inflush();
  tprintf("%c", 5);   /* ENQ */
  report = get_reply();

  vt_move(row = 10, col = 1);
  chrprint2(report, row, col);

  vt_move(12, 1);

  restore_ttymodes();
  return MENU_HOLD;
}

static int
tst_NLM(MENU_ARGS)
{
  int row, col;
  const char *report;

  vt_move(1, 1);
  println("Test of LineFeed/NewLine mode.");

  vt_move(3, 1);
  sm("20");
  set_tty_crmod(FALSE);
  tprintf("NewLine mode set. Push the RETURN key: ");
  report = instr();

  vt_move(row = 4, col = 1);
  vt_el(0);
  chrprint2(report, row, col);

  if (!strcmp(report, "\015\012"))
    show_result(" -- OK");
  else
    show_result(" -- Not expected");

  vt_move(6, 1);
  rm("20");
  tprintf("NewLine mode reset. Push the RETURN key: ");
  report = instr();

  vt_move(row = 7, col = 1);
  vt_el(0);
  chrprint2(report, row, col);

  if (!strcmp(report, "\015"))
    show_result(" -- OK");
  else
    show_result(" -- Not expected");

  vt_move(9, 1);

  restore_ttymodes();
  return MENU_HOLD;
}

/******************************************************************************/
int
tst_reports(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static MENU my_menu[] = {
      { "Exit",                                                   NULL },
      { "<ENQ> (AnswerBack Message)",                             tst_ENQ },
      { "Set/Reset Mode - LineFeed / Newline",                    tst_NLM },
      { "Device Status Report (DSR)                 VT100 & up",  tst_DSR },
      { "Primary Device Attributes (DA)             VT100 & up",  tst_DA },
      { "Secondary Device Attributes (DA)           VT220 & up",  tst_DA_2 },
      { "Tertiary Device Attributes (DA)            VT420",       tst_DA_3 },
      { "Request Terminal Parameters (DECREQTPARM)  VT100",       tst_DECREQTPARM },
      { "",                                                       NULL }
    };
  /* *INDENT-ON* */

  do {
    vt_clear(2);
    __(title(0), printxx("Terminal Reports/Responses"));
    __(title(2), println("Choose test type:"));
  } while (menu(my_menu));
  return MENU_NOHOLD;
}
