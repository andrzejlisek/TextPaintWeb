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

    fake_i.clear();
    fake_o.clear();
}

void fakeio::i_magic_set(int param)
{
    if (InputMagicNumber != 0)
    {
        fakeio::i_push(fakeio::InputMagicNumber);
        fakeio::i_push(param);
        return;
    }
}

void fakeio::i_magic()
{
    if (InputMagicNumber == 0)
    {
        return;
    }
    bool work = fake_i.size() > 0;
    if (work) if (fake_i[0] != InputMagicNumber) work = false;
    while (work)
    {
        fake_i.pop_front();
        switch (fake_i[0])
        {
            case '1':
                EchoKeys = false;
                break;
            case '0':
                EchoKeys = true;
                break;
        }
        fake_i.pop_front();

        work = fake_i.size() > 0;
        if (work) if (fake_i[0] != InputMagicNumber) work = false;
    }
}

void fakeio::i_push(int Val)
{
    fake_i.push_back(Val);
}

int fakeio::i_pop()
{
    int Val = fake_i[0];
    fake_i.pop_front();
    return Val;
}

int fakeio::i_count()
{
    i_magic();
    return fake_i.size();
}

void fakeio::o_push(int Val)
{
    fake_o.push_back(Val);
}

int fakeio::o_pop()
{
    int Val = fake_o[0];
    fake_o.pop_front();
    return Val;
}

int fakeio::o_count()
{
    return fake_o.size();
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

int fakeio::_getchar(void)
{
    _fflush(stdout);
    while (i_count() < 1)
    {
        _sleep(20);
    }
    int Chr = i_pop();
    if (EchoKeys && Echo)
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
            if (EchoKeys && Echo)
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

int fakeio::_fgetc(FILE * stream)
{
    if (stream == stdin)
    {
        return _getchar();
    }
    else
    {
        return EOF;
    }
}

char * fakeio::_fgets(char * str, int num, FILE * stream)
{
    if (stream == stdin)
    {
        int i = 0;
        while (true)
        {
            int chr = _getchar();
            if ((chr == 13) || (chr == 10))
            {
                str[i] = 0;
                return str;
            }
            else
            {
                str[i] = chr;
                i++;
                if ((i + 1) == num)
                {
                    str[i] = 0;
                    return str;
                }
            }
        }
    }
    else
    {
        return NULL;
    }
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
    emscripten_sleep(T);
    ///###std::this_thread::sleep_for(std::chrono::milliseconds(T));
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
        //std::cout << "VTTEST log open" << std::endl;
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
        //std::cout << "VTTEST log closed" << std::endl;
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

