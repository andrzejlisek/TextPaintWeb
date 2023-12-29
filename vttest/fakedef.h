#ifndef FAKEDEF_H
#define FAKEDEF_H

#define HAVE_TERMIOS_H 0
#define HAVE_TCGETATTR 0
#define HAVE_VFPRINTF 1
#define HAVE_USLEEP 1

#define SIGTERM 0
#define getpid() 0


// termios.h

#define  B0     0000000
#define  B50    0000001
#define  B75    0000002
#define  B110   0000003
#define  B134   0000004
#define  B150   0000005
#define  B200   0000006
#define  B300   0000007
#define  B600   0000010
#define  B1200  0000011
#define  B1800  0000012
#define  B2400  0000013
#define  B4800  0000014
#define  B9600  0000015
#define  B19200 0000016
#define  B38400 0000017

#define EXTA B19200
#define EXTB B38400

#define NCCS 32
#define VTIME 5
#define VMIN 6

typedef struct
{
  int c_cc[NCCS];
} TTY;


#endif // FAKEDEF_H
