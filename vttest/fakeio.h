#ifndef FAKEIO_H
#define FAKEIO_H

#include <iostream>
#include <cstdarg>
#include <deque>
#include <mutex>
#include <chrono>
#include <thread>
#include "../prog/stopwatch.h"

#define fake_array_size 256

class fakeio
{
public:
    fakeio();

    static inline int ProgWork = 1;
    static inline bool Echo = true;

    static void io_clear();
    static void i_push(int Val);
    static int i_pop();
    static int i_count();
    static void o_push(int Val);
    static int o_pop();
    static int o_count();

    static int _putchar(int character);
    static int _getchar(void);
    static int _read(int fd, void * buf, int nbytes);

    static void _exit(int status);
    static void _kill(int pid, int sig);

    static int _fputc(int character, FILE * stream);
    static int _fputs(const char * str, FILE * stream);
    static int _fflush(FILE * stream);
    static int _vfprintf(FILE * stream, const char * format, va_list arg);
    static int _printf(const char * format, ...);
    static int _fprintf(FILE * stream, const char * format, ...);

    static void _sleep(int T);
private:
    static inline std::deque<int> fake_i;
    static inline std::deque<int> fake_o;
    static inline std::mutex fake_i_mtx;
    static inline std::mutex fake_o_mtx;
};

#endif // FAKEIO_H
