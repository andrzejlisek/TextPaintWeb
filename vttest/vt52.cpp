/* $Id: vt52.c,v 1.23 2024/12/05 00:43:14 tom Exp $ */

#include "vttest.h"
#include "ttymodes.h"
#include "esc.h"

static int
testing(const char *name, int row)
{
  char buffer[1024];

  sprintf(buffer,
          "Testing %s. "
          "A real VT%d will not recognize %s at this point",
          name, terminal_id(), name);
  println(buffer);
  return row + 1;
}

static int
isreturn(const char *reply)
{
  return (*reply == '\r' || *reply == '\n');
}

int
tst_vt52(MENU_ARGS)
{
  /* *INDENT-OFF* */
  static struct {
      const char *rcode;
      const char *rmsg;
  } resptable[] = {
      { "\033/A", " -- OK (VT50)" },
      { "\033/C", " -- OK (VT55)" },
      { "\033/H", " -- OK (VT50H without copier)" },
      { "\033/J", " -- OK (VT50H with copier)" },
      { "\033/K", " -- OK (means Standard VT52)" },
      { "\033/L", " -- OK (VT52 with copier)" },
      { "\033/Z", " -- OK (means VT100 emulating VT52)" },
      { "",       " -- Unknown response"}
  };
  /* *INDENT-ON* */

  int i, j;
  char *response;
  VTLEVEL save;

  save_level(&save);
  set_level(0); /* Reset ANSI (VT100) mode, Set VT52 mode  */
  vt52home();   /* Cursor home     */
  vt52ed();     /* Erase to end of screen  */
  vt52home();   /* Cursor home     */
  for (i = 0; i <= max_lines - 1; i++) {
    for (j = 0; j <= 9; j++)
      tprintf("%s", "FooBar ");
    println("Bletch");
  }
  vt52home();   /* Cursor home     */
  vt52ed();     /* Erase to end of screen  */

  vt52cup(7, 47);
  tprintf("nothing more.");
  for (i = 1; i <= 10; i++)
    tprintf("THIS SHOULD GO AWAY! ");
  for (i = 1; i <= 5; i++) {
    vt52cup(1, 1);
    tprintf("%s", "Back scroll (this should go away)");
    vt52ri();   /* Reverse LineFeed (with backscroll!)  */
  }
  vt52cup(12, 60);
  vt52ed();     /* Erase to end of screen  */
  for (i = 2; i <= 6; i++) {
    vt52cup(i, 1);
    vt52el();   /* Erase to end of line */
  }
  /*
   * Draw some text that we do not want to display.
   */
  for (i = 2; i <= max_lines - 1; i++) {
    vt52cup(i, 70);
    tprintf("%s", "**Foobar");
  }
  /*
   * Draw the left side of the box, going from bottom to top.
   */
  vt52cup(max_lines - 1, 10);
  for (i = max_lines - 1; i >= 2; i--) {
    tprintf("%s", "*");
    tprintf("%c", 8);   /* BS */
    vt52ri();   /* Reverse LineFeed (LineStarve)        */
  }
  /*
   * Draw the top of the box, going from right to left.
   * Make the movement a little more complicated by using cursor-addressing
   * which is out of bounds vertically so that only the column is updated.
   */
  vt52cup(1, 70);
  for (i = 70; i >= 10; i--) {
    tprintf("%s", "*");
    vt52cub1();
    if (i % 2)
      vt52cup(max_lines + 3, i - 1);
    else
      vt52cub1();   /* Cursor Left */
  }
  /*
   * Draw the bottom of the box, going from left to right.
   */
  vt52cup(max_lines, 10);
  for (i = 10; i <= 70; i++) {
    tprintf("%s", "*");
    tprintf("%c", 8);   /* BS */
    vt52cuf1(); /* Cursor Right */
  }
  /*
   * Draw a column of "!" inside the left edge of the box.
   */
  vt52cup(2, 11);
  for (i = 2; i <= max_lines - 1; i++) {
    tprintf("%s", "!");
    tprintf("%c", 8);   /* BS */
    vt52cud1(); /* Cursor Down  */
  }
  /*
   * Draw a column of "!" inside the right edge of the box.
   */
  vt52cup(max_lines - 1, 69);
  for (i = max_lines - 1; i >= 2; i--) {
    tprintf("%s", "!");
    tprintf("%c", 8);   /* BS */
    vt52cuu1(); /* Cursor Up    */
  }
  /*
   * Erase the "*FooBar" on the right-side of the box, leaving just "*".
   */
  for (i = 2; i <= max_lines - 1; i++) {
    vt52cup(i, 71);
    vt52el();   /* Erase to end of line */
  }

  vt52cup(10, 16);
  printxx("%s", "The screen should be cleared, and have a centered");
  vt52cup(11, 16);
  printxx("%s", "rectangle of \"*\"s with \"!\"s on the inside to the");
  vt52cup(12, 16);
  printxx("%s", "left and right. Only this, and");
  vt52cup(13, 16);
  holdit();

  vt52home();   /* Cursor home     */
  vt52ed();     /* Erase to end of screen  */
  printxx("%s", "This is the normal character set:");
  for (j = 0; j <= 1; j++) {
    vt52cup(3 + j, 16);
    for (i = 0; i <= 47; i++)
      tprintf("%c", 32 + i + 48 * j);
  }
  vt52cup(6, 1);
  printxx("%s", "This is the special graphics character set:");
  esc("F");     /* Select Special Graphics character set        */
  for (j = 0; j <= 1; j++) {
    vt52cup(8 + j, 16);
    for (i = 0; i <= 47; i++)
      tprintf("%c", 32 + i + 48 * j);
  }
  esc("G");     /* Select ASCII character set   */
  vt52cup(12, 1);
  holdit();

  vt52home();   /* Cursor home     */
  vt52ed();     /* Erase to end of screen  */
  println("Test of terminal response to IDENTIFY command");

  /*
   * According to J.Altman, DECID isn't recognized by VT5xx terminals.  Real
   * DEC terminals through VT420 do, though it isn't recommended.  VT420's
   * emulation of VT52 does not recognize DA -- so we use DECID in this case.
   */
  set_tty_raw(TRUE);
  decid();      /* Identify     */
  response = get_reply();
  println("");

  restore_level(&save);
  restore_ttymodes();
  padding(10);  /* some terminals miss part of the response otherwise */

  printxx("Response was ");
  chrprint2(response, 2, 13);
  for (i = 0; resptable[i].rcode[0] != '\0'; i++) {
    if (!strcmp(response, resptable[i].rcode)) {
      show_result("%s", resptable[i].rmsg);
      break;
    }
  }
  println("");
  println("");

  /*
   * Verify whether returning to ANSI mode restores the previous operating
   * level.  If it was a VT220, we can check this by seeing if 8-bit controls
   * work; if a VT420 we can check the value of DECSCL.  A real VT420 goes to
   * VT100 mode.
   */
  if (terminal_id() >= 200) {
    int row, col;

    row = 8;
    set_level(0);   /* Reset ANSI (VT100) mode, Set VT52 mode  */
    println("Verify operating level after restoring ANSI mode");
    esc("<");   /* Enter ANSI mode (VT100 mode) */
    set_tty_raw(TRUE);
    if (save.cur_level >= 3) {  /* VT340 implements DECRQSS */
      vt_move(row, 1);
      row = testing("DECSCL", row);
      println("You should have to press return to continue:");
      println("");
      decrqss("\"p");
      response = instr();
      vt_move(++row, col = 10);
      printxx("Response was");
      chrprint2(response, row, col);
      if (isreturn(response)) {
        show_result(SHOW_SUCCESS);
      } else {
        if (parse_decrqss(response, "\"p") > 0)
          printxx("DECSCL recognized --");
        show_result(SHOW_FAILURE);
      }
      println("");
      row++;
    }

    if (save.cur_level >= 2) {
      const char *temp;

      vt_move(++row, 1);
      row = testing("S8C1T", row);
      s8c1t(1);
      cup(1, 1);
      dsr(6);
      response = instr();
      vt_move(row, col = 10);
      printxx("Response to CUP(1,1)/DSR(6)");
      chrprint2(response, row, col);
      if ((temp = skip_prefix(csi_input(), response)) != NULL) {
        if (!strcmp("1;1R", temp)) {
          printxx("S8C1T recognized --");
          show_result(SHOW_FAILURE);
        } else {
          printxx("unknown response --");
          show_result(SHOW_FAILURE);
        }
      } else {
        input_8bits = FALSE;  /* we expect this anyway */
        if ((temp = skip_prefix(csi_input(), response)) != NULL
            && !strcmp("1;1R", temp)) {
          show_result(SHOW_SUCCESS);
        } else {
          printxx("unknown response --");
          show_result(SHOW_FAILURE);
        }
      }
    }
    restore_level(&save);
    restore_ttymodes();
    println("");
    println("");
  }
  return MENU_HOLD;
}
