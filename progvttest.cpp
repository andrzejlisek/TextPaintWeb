#include "vttest/fakeio.h"
#include <emscripten/emscripten.h>
#include <thread>

int fake_main(int argc, char *argv[]);


int StrPrefixL = 7;
char StrPrefix[10] = "VTMsg(\"";
int StrSuffixL = 3;
char StrSuffix[10] = "\");";

#define Base64Limit 1024
#define Base64Limit_ 512

char Base64Chars[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
char Base64Str[Base64Limit * 4];
char Base64Raw[Base64Limit * 3];
int Base64StrL;
int Base64RawL;

void Base64Raw2Str()
{
    Base64StrL = StrPrefixL;
    for (int I = 0; I < Base64RawL; I += 3)
    {
        int Char0 = -260;
        int Char1 = -260;
        int Char2 = -260;
        if ((I + 0) < Base64RawL) Char0 = Base64Raw[I + 0];
        if ((I + 1) < Base64RawL) Char1 = Base64Raw[I + 1];
        if ((I + 2) < Base64RawL) Char2 = Base64Raw[I + 2];
        if (Char0 < 0) Char0 += 256;
        if (Char1 < 0) Char1 += 256;
        if (Char2 < 0) Char2 += 256;
        int Byte0 = 64;
        int Byte1 = 64;
        int Byte2 = 64;
        int Byte3 = 64;
        if (Char0 >= 0)
        {
            Byte0 = ((Char0 >> 2) & 63);
            Byte1 = ((Char0 << 4) & 63);
        }
        if (Char1 >= 0)
        {
            Byte1 = Byte1 + ((Char1 >> 4) & 63);
            Byte2 = ((Char1 << 2) & 63);
        }
        if (Char2 >= 0)
        {
            Byte2 = Byte2 + ((Char2 >> 6) & 63);
            Byte3 = ((Char2) & 63);
        }

        Base64Str[Base64StrL] = (Base64Chars[Byte0]);
        Base64StrL++;
        Base64Str[Base64StrL] = (Base64Chars[Byte1]);
        Base64StrL++;
        Base64Str[Base64StrL] = (Base64Chars[Byte2]);
        Base64StrL++;
        Base64Str[Base64StrL] = (Base64Chars[Byte3]);
        Base64StrL++;
    }
}

void Base64Str2Raw()
{
    Base64RawL = 0;
    for (int I = 0; I < Base64StrL; I += 4)
    {
        int Byte0 = -1;
        int Byte1 = -1;
        int Byte2 = -1;
        int Byte3 = -1;
        for (int II = 0; II < 64; II++)
        {
            if (Base64Chars[II] == Base64Str[I + 0]) { Byte0 = II; }
            if (Base64Chars[II] == Base64Str[I + 1]) { Byte1 = II; }
            if (Base64Chars[II] == Base64Str[I + 2]) { Byte2 = II; }
            if (Base64Chars[II] == Base64Str[I + 3]) { Byte3 = II; }
        }

        if ((Byte0 >= 0) && (Byte1 >= 0))
        {
            Base64Raw[Base64RawL] = (((Byte0 << 2) + (Byte1 >> 4)) & 255);
            Base64RawL++;
        }
        if ((Byte1 >= 0) && (Byte2 >= 0))
        {
            Base64Raw[Base64RawL] = (((Byte1 << 4) + (Byte2 >> 2)) & 255);
            Base64RawL++;
        }
        if ((Byte2 >= 0) && (Byte3 >= 0))
        {
            Base64Raw[Base64RawL] = (((Byte2 << 6) + (Byte3)) & 255);
            Base64RawL++;
        }
    }
}

//emscripten_wasm_worker_t Wrk;
std::thread * Wrk = NULL;

char * ProgArgs[2];

void main0()
{
    if (ProgArgs[1][0] == 0)
    {
        fake_main(1, ProgArgs);
    }
    else
    {
        fake_main(2, ProgArgs);
    }
}

void PrintStr(char * SS, int L, int StrType)
{
    char HexNum[17] = "0123456789ABCDEF";
    for (int I = 0; I < L; I++)
    {
        int T = (int)Base64Raw[I];
        
        switch (StrType)
        {
            case 0:
                if ((T < 32) || (T > 126))
                {
                    if (T < 0) T += 256;
                    std::cout << "<";
                    std::cout << T;
                    std::cout << ">";
                }
                else
                {
                    std::cout << (char)Base64Raw[I];
                }
                break;
            case 1:
                {
                    if (T < 0) T += 256;
                    std::cout << (char)HexNum[T >> 4];
                    std::cout << (char)HexNum[T & 15];
                }
                break;
        }
    }
}

extern "C"
{
    int DebugParams;

    EMSCRIPTEN_KEEPALIVE
    void EventData(char * data)
    {
        Base64StrL = strlen(data);
        if (Base64StrL > 0)
        {
            strcpy(Base64Str, data);
            
            Base64Str2Raw();
            
            if (data[0] == '`')
            {
            }
            
            for (int I = 0; I < Base64RawL; I++)
            {
                fakeio::i_push((char)Base64Raw[I]);
            }

            if (DebugParams > 0)
            {
                if ((DebugParams & 1) > 0)
                {
                    Base64Raw[Base64RawL] = 0;
                    std::cout << "Send: ";
                    PrintStr(Base64Raw, Base64RawL, 0);
                    std::cout << std::endl;
                }
                if ((DebugParams & 2) > 0)
                {
                    Base64Raw[Base64RawL] = 0;
                    std::cout << "Send: ";
                    PrintStr(Base64Raw, Base64RawL, 1);
                    std::cout << std::endl;
                }
                if ((DebugParams & 4) > 0)
                {
                    std::cout << "Send: " << Base64Str << std::endl;
                }
            }
        }
    
    
        while (fakeio::o_count() > 0)
        {
            Base64RawL = 0;
            int CountLimit = fakeio::o_count();
            if (CountLimit > (Base64Limit_))
            {
                CountLimit = (Base64Limit_);
            }
            while (CountLimit > 0)
            {
                int ChrX = fakeio::o_pop();
                if (ChrX < 0)
                {
                    ChrX += 256;
                }
                Base64Raw[Base64RawL] = (char)ChrX;
                Base64RawL++;
                CountLimit--;
            }
            if (Base64RawL > 0)
            {
                memcpy(Base64Str, StrPrefix, StrPrefixL);
                Base64Raw2Str();
                memcpy(Base64Str + Base64StrL, StrSuffix, StrSuffixL + 1);

                if (DebugParams > 0)
                {
                    if ((DebugParams & 1) > 0)
                    {
                        Base64Raw[Base64RawL] = 0;
                        std::cout << "Receive: ";
                        PrintStr(Base64Raw, Base64RawL, 0);
                        std::cout << std::endl;
                    }
                    if ((DebugParams & 2) > 0)
                    {
                        Base64Raw[Base64RawL] = 0;
                        std::cout << "Receive: ";
                        PrintStr(Base64Raw, Base64RawL, 1);
                        std::cout << std::endl;
                    }
                    if ((DebugParams & 4) > 0)
                    {
                        std::cout << "Receive: " << Base64Str << std::endl;
                    }
                }

                emscripten_run_script(Base64Str);
            }
        }
    }

    EMSCRIPTEN_KEEPALIVE
    void EventStart(char * arg, int DebugParams_)
    {
        DebugParams = DebugParams_;
        strcpy(ProgArgs[0], "/test/vttest");
        strcpy(ProgArgs[1], arg);
        fakeio::io_clear();
        Wrk = new std::thread(main0);
    }

    EMSCRIPTEN_KEEPALIVE
    void EventStop()
    {
        pthread_cancel(Wrk->native_handle());
        Wrk->join();
        delete Wrk;
        Wrk = NULL;
    }
}

int main()
{
    ProgArgs[0] = (char*)malloc(20);
    ProgArgs[1] = (char*)malloc(100);
    return 0;
}

