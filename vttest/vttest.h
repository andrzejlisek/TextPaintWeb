/* $Id: vttest.h,v 1.128 2024/11/25 01:14:50 tom Exp $ */

#ifndef VTTEST_H
#define VTTEST_H 1

/* original version... */
#define VERSION "1.7b 1985-04-19"

/* the actual version is here */
#include "patchlev.h"
#include "fakedef.h"
/* Choose one of these */

#ifdef HAVE_CONFIG_H
#include <config.h>
#define UNIX
#else

/* Assume ANSI and (minimal) Posix */
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define RETSIGTYPE void

#endif

#ifndef GCC_UNUSED
#define GCC_UNUSED  /* ARGSUSED */
#endif

#define SIG_ARGS int sig

#include <cstdio>
#include <cstdarg>

#ifdef HAVE_STDLIB_H
//#include <stdlib.h>
#endif

#ifdef HAVE_UNISTD_H
//#include <cunistd>
#endif

#ifdef HAVE_STRING_H
#include <cstring>
#endif

#include <cctype>

#if defined(HAVE_TERMIOS_H) && defined(HAVE_TCGETATTR)
#  define USE_POSIX_TERMIOS 1
#elif defined(HAVE_TERMIO_H)
#  define USE_TERMIO 1
#elif defined(HAVE_SGTTY_H)
#  define USE_SGTTY 1
#  define USE_FIONREAD 1
#elif !defined(VMS)
//#  error please fix me
#endif

#ifndef USE_FIONREAD
#define USE_FIONREAD 0
#endif

#ifndef USE_POSIX_TERMIOS
#define USE_POSIX_TERMIOS 0
#endif

#ifndef USE_SGTTY
#define USE_SGTTY 0
#endif

#ifndef USE_TERMIO
#define USE_TERMIO 0
#endif

//#include <csignal>
//#include <setjmp.h>

#if USE_POSIX_TERMIOS
#  //include <termios.h>
#  //define TTY struct termios
#else
#  if USE_TERMIO
#    include <termio.h>
/*#    define TCSANOW TCSETA */
/*#    define TCSADRAIN TCSETAW */
/*#    define TCSAFLUSH TCSETAF */
#    define TTY struct termio
#    define tcsetattr(fd, cmd, arg) ioctl(fd, cmd, arg)
#    define tcgetattr(fd, arg) ioctl(fd, TCGETA, arg)
#    define VDISABLE (unsigned char)(-1)
#  else
#    if USE_SGTTY
#      include <sgtty.h>
#      define TTY struct sgttyb
#    endif
#  endif
#endif

#ifdef HAVE_SYS_FILIO_H
#  include <sys/filio.h>  /* FIONREAD */
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#if !defined(sun) || !defined(NL0)
#  ifdef HAVE_IOCTL_H
#   include <ioctl.h>
#  else
#   ifdef HAVE_SYS_IOCTL_H
#    include <sys/ioctl.h>
#   endif
#  endif
#endif

#include <cerrno>

#define LOG_ENABLED ((log_fp != (FILE *) 0) && !log_disabled)

#define CharOf(c) ((unsigned char)(c))

extern FILE *log_fp;
extern int allows_utf8;
extern int assume_utf8;
extern int brkrd;
extern int debug_level;
extern int decac_bg;
extern int decac_fg;
extern int do_colors;
extern int input_8bits;
extern int log_disabled;
extern int lrmm_flag;
extern int max_cols;
extern int max_lines;
extern int min_cols;
extern int origin_mode;
extern int output_8bits;
extern int parse_7bits;
extern int reading;
extern int slow_motion;
extern int tty_speed;
extern int use_decac;
extern int use_padding;
//extern //jmp_buf intrenv;

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#define SHOW_SUCCESS "ok"
#define SHOW_FAILURE "failed"

#undef __
#define __(a,b) (void)(a && b)

#define TABLESIZE(table) (int)(sizeof(table)/sizeof(table[0]))

#define DEFAULT_SPEED 9600
#define TABWIDTH 8

#define STR_ENABLE(flag)     ((flag) ? "Disable" : "Enable")
#define STR_ENABLED(flag)    ((flag) ? "enabled" : "disabled")
#define STR_START(flag)      ((flag) ? "Stop" : "Start")

#if !defined(__GNUC__) && !defined(__attribute__)
#define __attribute__(p)  /* nothing */
#endif

#ifndef GCC_NORETURN
#define GCC_NORETURN  /* nothing */
#endif

#ifndef GCC_PRINTFLIKE
#define GCC_PRINTFLIKE(fmt,var)   /* nothing */
#endif

#ifndef GCC_UNUSED
#define GCC_UNUSED  /* nothing */
#endif

#define MENU_DECL    const char *   the_title
#define MENU_ARGS    const char *   the_title GCC_UNUSED
#define PASS_ARGS /* const char * */the_title

typedef struct {
  const char *description;
  int (*dispatch) (MENU_ARGS);
} MENU;

typedef struct {
  int cur_level;
  int input_8bits;
  int output_8bits;
  int parse_7bits;
} VTLEVEL;

typedef struct {
  int mode;
  const char *name;
  int level;
} RQM_DATA;

#define MENU_NOHOLD 0
#define MENU_HOLD   1
#define MENU_MERGE  2

#define TITLE_LINE  3

#define BUF_SIZE 1024

/* normal logging-prefixes */
#define READ_STR "Read: "   /* use this for noting user-input */
#define NOTE_STR "Note: "   /* use this for making notes (no data involved) */
#define SEND_STR "Send: "   /* use this for control-sequences, sent */
#define DATA_STR "Data: "   /* use this for test-data, sent */
#define TELL_STR "Text: "   /* use this for test-instructions */

/* replay logging-prefixes */
#define WAIT_STR "Wait: "   /* use this for suspending replay */
#define DONE_STR "Done: "   /* use this for resuming replay */
#define SKIP_STR "Skip: "   /* use this for debugging replay */

#define WHITE_ON_BLUE   "0;37;44"
#define WHITE_ON_GREEN  "0;37;42"
#define YELLOW_ON_BLACK "0;33;40"
#define BLINK_REVERSE   "0;5;7"

extern char origin_mode_mesg[80];
extern char lrmm_mesg[80];
extern char lr_marg_mesg[80];
extern char tb_marg_mesg[80];
extern char txt_override_color[80];
extern int current_Gx[4];
extern int scs_national;

extern RETSIGTYPE onbrk(SIG_ARGS);
extern GCC_NORETURN RETSIGTYPE onterm(SIG_ARGS);
extern char *chrformat(const char *s, int col, int first);
extern char *get_reply(void);
extern char *instr(void);
extern char *replay_string(void);
extern char *skip_csi(char *input);
extern char *skip_dcs(char *input);
extern char *skip_digits(char *src);
extern char *skip_osc(char *input);
extern char *skip_prefix(const char *prefix, char *input);
extern char *skip_ss3(char *input);
extern char inchar(void);
extern const char *charset_name(int g, int n);
extern const char *parse_Sdesig(const char *source, int *offset);
extern const char *parse_upss_name(const char *source, int size);
extern const char *skip_csi_2(const char *input);
extern const char *skip_dcs_2(const char *input);
extern const char *skip_digits_2(const char *src);
extern const char *skip_prefix_2(const char *prefix, const char *input);
extern const char *skip_ss3_2(const char *input);
extern const char *skip_xdigits(const char *src, int *value);
extern int any_DSR(MENU_ARGS, const char *text, void (*explain) (const char *report));
extern int any_RQM(MENU_ARGS, RQM_DATA * table, int tablesize, int private_);
extern int any_decrqpsr(MENU_ARGS, int Ps);
extern int any_decrqss(const char *msg, const char *func);
extern int any_decrqss2(const char *msg, const char *func, const char *expected);
extern int bug_a(MENU_ARGS);
extern int bug_b(MENU_ARGS);
extern int bug_c(MENU_ARGS);
extern int bug_d(MENU_ARGS);
extern int bug_e(MENU_ARGS);
extern int bug_f(MENU_ARGS);
extern int bug_l(MENU_ARGS);
extern int bug_s(MENU_ARGS);
extern int bug_w(MENU_ARGS);
extern int chrprint2(const char *s, int row, int col);
extern int conv_to_utf32(unsigned *target, const char *source, unsigned limit);
extern int conv_to_utf8(unsigned char *target, unsigned source, unsigned limit);
extern int do_scs(int g);
extern int get_bottom_margin(int n);
extern int get_char(void);
extern int get_left_margin(void);
extern int get_level(void);
extern int get_right_margin(void);
extern int get_top_margin(void);
extern int is_replaying(void);
extern int fake_main(int argc, char *argv[]);
extern int menu(MENU *table);
extern int menu2(MENU *table, int tp);
extern int not_impl(MENU_ARGS);
extern int parse_decrqss(char *report, const char *func);
extern int print_chr(int c);
extern int print_str(const char *s);
extern int reset_charset(MENU_ARGS);
extern int rpt_DECSTBM(MENU_ARGS);
extern int sane_cs(int g);
extern int scan_DA(const char *str, int *pos);
extern int scan_any(const char *str, int *pos, int toc);
extern int scanto(const char *str, int *pos, int toc);
extern int set_DECRPM(int level);
extern int set_level(int level);
extern int setup_terminal(MENU_ARGS);
extern int specify_G0(MENU_ARGS);
extern int specify_G1(MENU_ARGS);
extern int specify_G2(MENU_ARGS);
extern int specify_G3(MENU_ARGS);
extern int strip_suffix(char *src, const char *suffix);
extern int strip_terminator(char *src);
extern int terminal_id(void);
extern int title(int offset);
extern int toggle_DECOM(MENU_ARGS);
extern int toggle_LRMM(MENU_ARGS);
extern int toggle_SLRM(MENU_ARGS);
extern int toggle_STBM(MENU_ARGS);
extern int toggle_color_mode(MENU_ARGS);
extern int toggle_nrc(MENU_ARGS);
extern int tst_CBT(MENU_ARGS);
extern int tst_CHA(MENU_ARGS);
extern int tst_CHT(MENU_ARGS);
extern int tst_CNL(MENU_ARGS);
extern int tst_CPL(MENU_ARGS);
extern int tst_DECRPM(MENU_ARGS);
extern int tst_DECSTR(MENU_ARGS);
extern int tst_DSR_cursor(MENU_ARGS);
extern int tst_DSR_keyboard(MENU_ARGS);
extern int tst_DSR_printer(MENU_ARGS);
extern int tst_DSR_userkeys(MENU_ARGS);
extern int tst_HPA(MENU_ARGS);
extern int tst_HPR(MENU_ARGS);
extern int tst_SD(MENU_ARGS);
extern int tst_SRM(MENU_ARGS);
extern int tst_SU(MENU_ARGS);
extern int tst_VPA(MENU_ARGS);
extern int tst_VPR(MENU_ARGS);
extern int tst_bugs(MENU_ARGS);
extern int tst_characters(MENU_ARGS);
extern int tst_colors(MENU_ARGS);
extern int tst_doublesize(MENU_ARGS);
extern int tst_ecma48_curs(MENU_ARGS);
extern int tst_ecma48_misc(MENU_ARGS);
extern int tst_insdel(MENU_ARGS);
extern int tst_keyboard(MENU_ARGS);
extern int tst_keyboard_layout(const char *scs_params);
extern int tst_mouse(MENU_ARGS);
extern int tst_movements(MENU_ARGS);
extern int tst_nonvt100(MENU_ARGS);
extern int tst_printing(MENU_ARGS);
extern int tst_reports(MENU_ARGS);
extern int tst_rst(MENU_ARGS);
extern int tst_screen(MENU_ARGS);
extern int tst_setup(MENU_ARGS);
extern int tst_softchars(MENU_ARGS);
extern int tst_statusline(MENU_ARGS);
extern int tst_tek4014(MENU_ARGS);
extern int tst_upss(MENU_ARGS);
extern int tst_vt220(MENU_ARGS);
extern int tst_vt220_device_status(MENU_ARGS);
extern int tst_vt220_reports(MENU_ARGS);
extern int tst_vt220_screen(MENU_ARGS);
extern int tst_vt320(MENU_ARGS);
extern int tst_vt320_DECRQSS(MENU_ARGS);
extern int tst_vt320_cursor(MENU_ARGS);
extern int tst_vt320_device_status(MENU_ARGS);
extern int tst_vt320_report_presentation(MENU_ARGS);
extern int tst_vt320_reports(MENU_ARGS);
extern int tst_vt320_screen(MENU_ARGS);
extern int tst_vt420(MENU_ARGS);
extern int tst_vt420_DECRQSS(MENU_ARGS);
extern int tst_vt420_cursor(MENU_ARGS);
extern int tst_vt420_device_status(MENU_ARGS);
extern int tst_vt420_report_presentation(MENU_ARGS);
extern int tst_vt420_reports(MENU_ARGS);
extern int tst_vt52(MENU_ARGS);
extern int tst_vt520(MENU_ARGS);
extern int tst_vt520_DECRQSS(MENU_ARGS);
extern int tst_vt520_reports(MENU_ARGS);
extern int tst_xterm(MENU_ARGS);
extern int vt_move(int row, int col);
extern GCC_NORETURN void bye(void);
extern void close_replay(void);
extern void default_level(void);
extern void dirty_charset(int state);
extern void do_scrolling(void);
extern void enable_logging(const char *filename);
extern void finish_vt420_cursor(MENU_ARGS);
extern void initterminal(int pn);
extern void menus_vt420_cursor(void);
extern void pause_replay(void);
extern void reset_colors(void);
extern void reset_level(void);
extern void restore_level(const VTLEVEL *save);
extern void resume_replay(void);
extern void save_level(VTLEVEL *save);
extern void scs_graphics(void);
extern void scs_normal(void);
extern void set_colors(const char *value);
extern void setup_replay(const char *filename);
extern void setup_softchars(const char *filename);
extern void setup_vt420_cursor(MENU_ARGS);
extern void show_mousemodes(void);
extern void show_result(const char *fmt, ...) GCC_PRINTFLIKE(1,2);
extern void slowly(void);
extern void test_with_margins(int enable);
extern void vt_clear(int code);
extern void vt_el(int code);
extern void vt_hilite(int flag);

extern GCC_NORETURN void failed(const char *);
extern GCC_NORETURN void no_memory(void);

#endif /* VTTEST_H */
