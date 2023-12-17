#include "fakeio.h"

fakeio::fakeio()
{

}

void fakeio::io_clear()
{
    fake_i_mtx.lock();
    fake_o_mtx.lock();
    fake_i.clear();
    fake_o.clear();
    fake_o_mtx.unlock();
    fake_i_mtx.unlock();
}

void fakeio::i_push(int Val)
{
    fake_i_mtx.lock();
    fake_i.push_back(Val);
    fake_i_mtx.unlock();
}

int fakeio::i_pop()
{
    fake_i_mtx.lock();
    int Val = fake_i[0];
    fake_i.pop_front();
    fake_i_mtx.unlock();
    return Val;
}

int fakeio::i_count()
{
    fake_i_mtx.lock();
    int Val = fake_i.size();
    fake_i_mtx.unlock();
    return Val;
}

void fakeio::o_push(int Val)
{
    fake_o_mtx.lock();
    fake_o.push_back(Val);
    fake_o_mtx.unlock();
}

int fakeio::o_pop()
{
    fake_o_mtx.lock();
    int Val = fake_o[0];
    fake_o.pop_front();
    fake_o_mtx.unlock();
    return Val;
}

int fakeio::o_count()
{
    fake_o_mtx.lock();
    int Val = fake_o.size();
    fake_o_mtx.unlock();
    return Val;
}


int fakeio::_putchar(int character)
{
    o_push(character);
    return 0;
}

int fakeio::_getchar( void )
{
    _fflush(stdout);
    while (i_count() < 1)
    {
        _sleep(20);
    }
    int Chr = i_pop();
    if (Echo)
    {
        if (Chr == '\r')
        {
            _putchar('\n');
        }
        else
        {
            _putchar(Chr);
        }
    }
    return Chr;
}

int fakeio::_read(int fd, void * buf, int nbytes)
{
    _fflush(stdout);
    if ((fd == 0) || (fd == 2))
    {
        int N = 0;
        if (fd == 0)
        {
            while (i_count() == 0)
            {
                _sleep(20);
            }
        }
        while ((N < nbytes) && (i_count() > 0))
        {
            int Chr = i_pop();
            /*if (Echo)
            {
                if (Chr == '\r')
                {
                    _putchar('\n');
                }
                else
                {
                    _putchar(Chr);
                }
            }*/
            ((char*)buf)[N] = (char)(Chr);
            N++;
        }
        return N;
    }
    return 0;
}


void fakeio::_exit(int status)
{
    ProgWork = 2;
}


void fakeio::_kill(int pid, int sig)
{
    ProgWork = 2;
}

int fakeio::_fputc(int character, FILE * stream)
{
    if ((stream == stdout) || (stream == stderr))
    {
        _putchar(character);
    }
    return 0;
}

int fakeio::_fputs(const char * str, FILE * stream)
{
    if ((stream == stdout) || (stream == stderr))
    {
        int I = 0;
        while (str[I] != 0)
        {
            _putchar(str[I]);
            I++;
        }
    }
    return 0;
}

int fakeio::_fflush(FILE * stream)
{
    return 0;
}

int fakeio::_vfprintf(FILE * stream, const char * format, va_list arg)
{
    if ((stream == stdout) || (stream == stderr))
    {
        char buf[fake_array_size];
        vsnprintf(buf, fake_array_size, format, arg);
        _fputs(buf, stdout);
    }
    return 0;
}

int fakeio::_printf(const char * format, ...)
{
    va_list valist;
    va_start(valist, format);
    _vfprintf(stdout, format, valist);
    va_end(valist);
    return 0;
}

int fakeio::_fprintf(FILE * stream, const char * format, ...)
{
    if ((stream == stdout) || (stream == stderr))
    {
        va_list valist;
        va_start(valist, format);
        _vfprintf(stdout, format, valist);
        va_end(valist);
    }
    return 0;
}

void fakeio::_sleep(int T)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(T));
}
