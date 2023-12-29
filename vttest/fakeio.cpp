#include "fakeio.h"

fakeio::fakeio()
{

}

void fakeio::io_clear()
{
    if (LogHandle != ((FILE*)0))
    {
        free(LogHandle);
    }
    LogHandle = 0;
    LogFlush(false);

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
    if (character == '\n')
    {
        o_push('\r');
    }
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
            _putchar('\r');
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
            if (Echo)
            {
                if (Chr == '\r')
                {
                    _putchar('\r');
                    _putchar('\n');
                }
                else
                {
                    _putchar(Chr);
                }
            }
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
    else
    {
        if ((LogHandle != (FILE*)0) && (LogHandle == stream))
        {
            if (character == '\n')
            {
                LogFlush(true);
            }
            else
            {
                if ((character >= 32) && (character <= 126))
                {
                    LogBuf.push_back(character);
                }
                else
                {
                    LogBuf.push_back('.');
                }
            }
        }
    }
    return 0;
}

int fakeio::_fputs(const char * str, FILE * stream)
{
    int I = 0;
    while (str[I] != 0)
    {
        _fputc(str[I], stream);
        I++;
    }
    return 0;
}

int fakeio::_fflush(FILE * stream)
{
    return 0;
}

int fakeio::_vfprintf(FILE * stream, const char * format, va_list arg)
{
    char buf[fake_array_size];
    vsnprintf(buf, fake_array_size, format, arg);
    _fputs(buf, stream);
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
    va_list valist;
    va_start(valist, format);
    _vfprintf(stream, format, valist);
    va_end(valist);
    return 0;
}

void fakeio::_sleep(int T)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(T));
}

FILE * fakeio::_fopen(const char * filename, const char * modes)
{
    char LogName[] = "vttest.log";

    bool IsLog = true;
    int I = 0;

    while (LogName[I] != 0)
    {
        if (filename[I] == 0)
        {
            IsLog = false;
            break;
        }
        if (LogName[I] != filename[I])
        {
            IsLog = false;
            break;
        }
        I++;
    }

    if (IsLog)
    {
        LogFlush(false);
        if (LogHandle == ((FILE*)0))
        {
            LogHandle = (FILE*)malloc(32);
        }
        std::cout << "VTTEST log open" << std::endl;
        return LogHandle;
    }
    else
    {
        return 0;
    }
}

int fakeio::_fclose(FILE * stream)
{
    if (stream == LogHandle)
    {
        LogFlush(false);
        free(LogHandle);
        LogHandle = 0;
        std::cout << "VTTEST log closed" << std::endl;
    }
    return 0;
}

void fakeio::LogFlush(bool Force)
{
    if (Force || (LogBuf.size() > 0))
    {
        std::cout << "VTTEST:" << LogBuf << std::endl;
    }
    LogBuf = "";
}

