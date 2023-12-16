#include "progcommon.cpp"

#include "prog/xlist.h"
#include "prog/configfile.h"
#include "prog/core0editor.h"
#include "prog/core1player.h"
#include "prog/core2terminal.h"
#include "prog/core3keys.h"
#include "prog/core4command.h"
#include "prog/core9menu.h"
#include "prog/binaryfile.h"

void RespondClear()
{
    BufClear();
    BufTxt("_TestOK2([");
}

void RespondFinish()
{
    BufTxt("0]);");
    BufChr(0);
    emscripten_worker_respond(IOBuf, IOBufPtr);
}

void RespondPartial()
{
    if (BufCheck())
    {
        BufTxt("0]);");
        BufChr(0);
        emscripten_worker_respond_provisionally(IOBuf, IOBufPtr);
        RespondClear();
    }
}

void ScreenChar(int X, int Y, int Chr, int Back, int Fore, int Attr, int FontW, int FontH)
{
    BufNum(101);
    BufNum(X);
    BufNum(Y);
    BufNum(Chr);
    BufNum(Back);
    BufNum(Fore);
    BufNum(Attr);
    BufNum(FontW);
    BufNum(FontH);
    RespondPartial();
}

void ScreenClear(int ColorBack, int ColorFore)
{
    BufNum(102);
    BufNum(ColorBack);
    BufNum(ColorFore);
    RespondPartial();
}

void ScreenResize(int NewW, int NewH)
{
    BufNum(103);
    BufNum(NewW);
    BufNum(NewH);
    RespondPartial();
}

void ScreenCursorMove(int X, int Y)
{
    BufNum(104);
    BufNum(X);
    BufNum(Y);
    RespondPartial();
}

void ScreenTextMove(int X1, int Y1, int X2, int Y2, int W, int H)
{
    BufNum(105);
    BufNum(X1);
    BufNum(Y1);
    BufNum(X2);
    BufNum(Y2);
    BufNum(W);
    BufNum(H);
    RespondPartial();
}

void ClipboardCopy(std::string X)
{
}

void ClipboardPaste()
{
}

void WorkerSend(int T, std::string X)
{
    BufNum(200 + T);
    BufTxt("\"");
    BufTxt(X.c_str());
    BufTxt("\",");
    RespondPartial();
}


int X0 = 0;
int Y0 = 0;

std::shared_ptr<ConfigFile> CF;
std::unique_ptr<CoreCommon> Core;
std::shared_ptr<BinaryFile> BinaryFile_;

extern "C"
{
    int XXX = 0;
    int YYY = 0;
    bool ProgMenu = true;

    void Init0()
    {
        // Creating environment
        ProgMenu = false;
        switch (CF.get()->ParamGetI("WorkMode"))
        {
            case 0:
                Core = std::make_unique<Core0Editor>();
                break;
            case 1:
                Core = std::make_unique<Core1Player>();
                break;
            case 2:
                Core = std::make_unique<Core2Terminal>();
                break;
            case 3:
                Core = std::make_unique<Core3Keys>();
                break;
            case 4:
                Core = std::make_unique<Core4Command>();
                break;
            case 9:
                Core = std::make_unique<Core9Menu>();
                ProgMenu = true;
                break;
        }
        Core.get()->BinaryFile_ = BinaryFile_;
        Core.get()->CF = CF;
        Core.get()->InitCommon();
        Core.get()->Init();
        Core.get()->EventOther("Resize", "", CF.get()->ParamGetI("WinW"), CF.get()->ParamGetI("WinH"), 0, 0);


        /*Core.get()->EventKey("ArrowRight", 0, false, false, false);
        Core.get()->EventKey("ArrowRight", 0, false, false, false);
        Core.get()->EventKey("ArrowRight", 0, false, false, false);
        Core.get()->EventKey("ArrowRight", 0, false, false, false);
        Core.get()->EventKey("ArrowRight", 0, false, false, false);
        Core.get()->EventKey("ArrowRight", 0, false, false, false);
        Core.get()->EventKey("F3", 0, false, false, false);
        for (int I = 0; I < 8; I++)
        {
            Core.get()->EventKey("KeyD", 'd', false, false, false);
        }
        for (int I = 0; I < 3; I++)
        {
            Core.get()->EventKey("KeyS", 's', false, false, false);
        }*/
    }

    EMSCRIPTEN_KEEPALIVE
    void Init(char * data, int size)
    {
        BinaryFile_ = std::make_shared<BinaryFile>();
    
        BufInit();
        RespondClear();
        CF = std::make_shared<ConfigFile>();

        // Default configuration
        CF.get()->ParamClear();

        CF.get()->ParamSet("WorkMode", "9");

        CF.get()->ParamSet("WinUse", "1");

        CF.get()->ParamSet("WinTimer100", "0");

        CF.get()->ParamSet("Bell", "1");

        CF.get()->ParamSet("ConCursorDisplay", "1");
        CF.get()->ParamSet("ConUseMemo", "2");
        CF.get()->ParamSet("ConInputEncoding", "65001");
        CF.get()->ParamSet("ConOutputEncoding", "65001");
        CF.get()->ParamSet("ConUseEscapeCodes", "0");

        CF.get()->ParamSet("WinCellW", "8");
        CF.get()->ParamSet("WinCellH", "16");
        CF.get()->ParamSet("WinFontName", "Fonts/Dos/Dos08x16.png");
        CF.get()->ParamSet("WinFontSize", "10");
        CF.get()->ParamSet("WinCharRender", "1");
        CF.get()->ParamSet("WinFixed", "2");
        CF.get()->ParamSet("WinSteadyCursor", "0");

        CF.get()->ParamSet("WinW", "80");
        CF.get()->ParamSet("WinH", "24");
        CF.get()->ParamSet("WinColorBlending", "1");

        CF.get()->ParamSet("WinColorBlending_1", "2591,2588,3,1,0");
        CF.get()->ParamSet("WinColorBlending_2", "2592,2588,1,1,0");
        CF.get()->ParamSet("WinColorBlending_3", "2593,2588,1,3,0");
        CF.get()->ParamSet("WinColorBlending_4", "1FB8C,258C,1,1,0");
        CF.get()->ParamSet("WinColorBlending_5", "1FB8D,2590,1,1,0");
        CF.get()->ParamSet("WinColorBlending_6", "1FB8E,2580,1,1,0");
        CF.get()->ParamSet("WinColorBlending_7", "1FB8F,2584,1,1,0");
        CF.get()->ParamSet("WinColorBlending_8", "1FB9C,25E4,1,1,0");
        CF.get()->ParamSet("WinColorBlending_9", "1FB9D,25E5,1,1,0");
        CF.get()->ParamSet("WinColorBlending_10", "1FB9E,25E2,1,1,0");
        CF.get()->ParamSet("WinColorBlending_11", "1FB9F,25E3,1,1,0");
        CF.get()->ParamSet("WinColorBlending_12", "1FB90,20,1,1,1");
        CF.get()->ParamSet("WinColorBlending_13", "1FB91,2580,1,1,1");
        CF.get()->ParamSet("WinColorBlending_14", "1FB92,2584,1,1,1");
        CF.get()->ParamSet("WinColorBlending_15", "1FB93,258C,1,1,1");
        CF.get()->ParamSet("WinColorBlending_16", "1FB94,2590,1,1,1");

        CF.get()->ParamSet("WinPaletteR", "00AA00AA00AA00AA55FF55FF55FF55FF");
        CF.get()->ParamSet("WinPaletteG", "0000AAAA0000AAAA5555FFFF5555FFFF");
        CF.get()->ParamSet("WinPaletteB", "00000000AAAAAAAA55555555FFFFFFFF");

        CF.get()->ParamSet("WinPaletteBlinkR", "00AA00AA00AA00AA55FF55FF55FF55FF");
        CF.get()->ParamSet("WinPaletteBlinkG", "0000AAAA0000AAAA5555FFFF5555FFFF");
        CF.get()->ParamSet("WinPaletteBlinkB", "00000000AAAAAAAA55555555FFFFFFFF");

        CF.get()->ParamSet("WinPaletteFile", "Palettes/DOS.txt");

        CF.get()->ParamSet("DisplayBlink", "1");
        CF.get()->ParamSet("DisplayAttrib", "15");

        CF.get()->ParamSet("Space", "20,A0,2800");

        CF.get()->ParamSet("FileReadEncoding", "65001");
        CF.get()->ParamSet("FileWriteEncoding", "65001");
        CF.get()->ParamSet("FileReadSteps", "0");

        CF.get()->ParamSet("CipherMode", "0");
        CF.get()->ParamSet("CipherBegin", "005B");
        CF.get()->ParamSet("CipherEnd", "005D");
        CF.get()->ParamSet("CipherAlphabet", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
        CF.get()->ParamSet("CipherAutoKey", "0");
        CF.get()->ParamSet("CipherPassword", "");

        CF.get()->ParamSet("Frame1_0", "Simple,2D,7C,2B,2B,2B,2B,2B,2B,2B,2B,2B");
        CF.get()->ParamSet("Frame1_1", "Single,2500,2502,250C,252C,2510,251C,253C,2524,2514,2534,2518");
        CF.get()->ParamSet("Frame1_2", "Double,2550,2551,2554,2566,2557,2560,256C,2563,255A,2569,255D");
        CF.get()->ParamSet("Frame1_3", "SinDou,2500,2551,2553,2565,2556,255F,256B,2562,2559,2568,255C");
        CF.get()->ParamSet("Frame1_4", "DouSin,2550,2502,2552,2564,2555,255E,256A,2561,2558,2567,255B");
        CF.get()->ParamSet("Frame1_5", "ThinBold,2500,2503,250E,2530,2512,2520,2542,2528,2516,2538,251A");
        CF.get()->ParamSet("Frame1_6", "BoldThin,2501,2502,250D,252F,2511,251D,253F,2525,2515,2537,2519");
        CF.get()->ParamSet("Frame1_7", "Bold,2501,2503,250F,2533,2513,2523,254B,252B,2517,253B,251B");
        CF.get()->ParamSet("Frame1_8", "Test,2D,7C,31,32,33,34,35,36,37,38,39");

        CF.get()->ParamSet("Frame2_0", "Simple,5C,2F,5E,2B,3E,2B,58,2B,3C,2B,76");
        CF.get()->ParamSet("Frame2_1", "Line,2572,2571,2573,2573,2573,2573,2573,2573,2573,2573,2573");
        CF.get()->ParamSet("Frame2_2", "Test,5C,2F,31,32,33,34,35,36,37,38,39");

        CF.get()->ParamSet("PixelChar", "20,23");

        CF.get()->ParamSet("Pixel_1", "HalfH,1,2");
        CF.get()->ParamSet("Pixel_1_0", "0020,2580,2584,2588");

        CF.get()->ParamSet("Pixel_2", "HalfW,2,1");
        CF.get()->ParamSet("Pixel_2_0", "0020,258C,2590,2588");

        CF.get()->ParamSet("Pixel_3", "Quadrant,2,2");
        CF.get()->ParamSet("Pixel_3_0", "0020,2598,259D,2580");
        CF.get()->ParamSet("Pixel_3_1", "2596,258C,259E,259B");
        CF.get()->ParamSet("Pixel_3_2", "2597,259A,2590,259C");
        CF.get()->ParamSet("Pixel_3_3", "2584,2599,259F,2588");

        CF.get()->ParamSet("Pixel_4", "Teletext,2,3");
        CF.get()->ParamSet("Pixel_4_0", "00020,1FB00,1FB01,1FB02,1FB03,1FB04,1FB05,1FB06,1FB07,1FB08,1FB09,1FB0A,1FB0B,1FB0C,1FB0D,1FB0E");
        CF.get()->ParamSet("Pixel_4_1", "1FB0F,1FB10,1FB11,1FB12,1FB13,0258C,1FB14,1FB15,1FB16,1FB17,1FB18,1FB19,1FB1A,1FB1B,1FB1C,1FB1D");
        CF.get()->ParamSet("Pixel_4_2", "1FB1E,1FB1F,1FB20,1FB21,1FB22,1FB23,1FB24,1FB25,1FB26,1FB27,02590,1FB28,1FB29,1FB2A,1FB2B,1FB2C");
        CF.get()->ParamSet("Pixel_4_3", "1FB2D,1FB2E,1FB2F,1FB30,1FB31,1FB32,1FB33,1FB34,1FB35,1FB36,1FB37,1FB38,1FB39,1FB3A,1FB3B,02588");

        CF.get()->ParamSet("Pixel_5", "Braille,2,4");
        CF.get()->ParamSet("Pixel_5_0", "2800,2801,2808,2809,2802,2803,280A,280B,2810,2811,2818,2819,2812,2813,281A,281B");
        CF.get()->ParamSet("Pixel_5_1", "2804,2805,280C,280D,2806,2807,280E,280F,2814,2815,281C,281D,2816,2817,281E,281F");
        CF.get()->ParamSet("Pixel_5_2", "2820,2821,2828,2829,2822,2823,282A,282B,2830,2831,2838,2839,2832,2833,283A,283B");
        CF.get()->ParamSet("Pixel_5_3", "2824,2825,282C,282D,2826,2827,282E,282F,2834,2835,283C,283D,2836,2837,283E,283F");
        CF.get()->ParamSet("Pixel_5_4", "2840,2841,2848,2849,2842,2843,284A,284B,2850,2851,2858,2859,2852,2853,285A,285B");
        CF.get()->ParamSet("Pixel_5_5", "2844,2845,284C,284D,2846,2847,284E,284F,2854,2855,285C,285D,2856,2857,285E,285F");
        CF.get()->ParamSet("Pixel_5_6", "2860,2861,2868,2869,2862,2863,286A,286B,2870,2871,2878,2879,2872,2873,287A,287B");
        CF.get()->ParamSet("Pixel_5_7", "2864,2865,286C,286D,2866,2867,286E,286F,2874,2875,287C,287D,2876,2877,287E,287F");
        CF.get()->ParamSet("Pixel_5_8", "2880,2881,2888,2889,2882,2883,288A,288B,2890,2891,2898,2899,2892,2893,289A,289B");
        CF.get()->ParamSet("Pixel_5_9", "2884,2885,288C,288D,2886,2887,288E,288F,2894,2895,289C,289D,2896,2897,289E,289F");
        CF.get()->ParamSet("Pixel_5_10", "28A0,28A1,28A8,28A9,28A2,28A3,28AA,28AB,28B0,28B1,28B8,28B9,28B2,28B3,28BA,28BB");
        CF.get()->ParamSet("Pixel_5_11", "28A4,28A5,28AC,28AD,28A6,28A7,28AE,28AF,28B4,28B5,28BC,28BD,28B6,28B7,28BE,28BF");
        CF.get()->ParamSet("Pixel_5_12", "28C0,28C1,28C8,28C9,28C2,28C3,28CA,28CB,28D0,28D1,28D8,28D9,28D2,28D3,28DA,28DB");
        CF.get()->ParamSet("Pixel_5_13", "28C4,28C5,28CC,28CD,28C6,28C7,28CE,28CF,28D4,28D5,28DC,28DD,28D6,28D7,28DE,28DF");
        CF.get()->ParamSet("Pixel_5_14", "28E0,28E1,28E8,28E9,28E2,28E3,28EA,28EB,28F0,28F1,28F8,28F9,28F2,28F3,28FA,28FB");
        CF.get()->ParamSet("Pixel_5_15", "28E4,28E5,28EC,28ED,28E6,28E7,28EE,28EF,28F4,28F5,28FC,28FD,28F6,28F7,28FE,28FF");

        CF.get()->ParamSet("ColorNormal", "07");
        CF.get()->ParamSet("ColorBeyondLine", "88");
        CF.get()->ParamSet("ColorBeyondEnd", "77");
        CF.get()->ParamSet("ColorCursor", "F0");
        CF.get()->ParamSet("ColorStatus", "F0");
        CF.get()->ParamSet("ColorPopup", "0F");
        CF.get()->ParamSet("BeyondLineMargin", "-1");

        CF.get()->ParamSet("ANSIRead", "0");
        CF.get()->ParamSet("ANSIReadCR", "0");
        CF.get()->ParamSet("ANSIReadLF", "0");
        CF.get()->ParamSet("ANSIWidth", "0");
        CF.get()->ParamSet("ANSIHeight", "0");
        CF.get()->ParamSet("ANSIDOS", "0");
        CF.get()->ParamSet("ANSI8bit", "0");
        CF.get()->ParamSet("ANSIColors", "1");
        CF.get()->ParamSet("ANSIReverseMode", "2");
        CF.get()->ParamSet("ANSIColorBlink", "0");
        CF.get()->ParamSet("ANSIColorBold", "0");
        CF.get()->ParamSet("ANSIIgnoreConcealed", "0");
        CF.get()->ParamSet("ANSIPrintBackspace", "0");
        CF.get()->ParamSet("ANSIPrintTab", "0");
        CF.get()->ParamSet("ANSIWrite", "0");
        CF.get()->ParamSet("ANSIColorScheme", "0");
        CF.get()->ParamSet("ANSIScrollChars", "1000");
        CF.get()->ParamSet("ANSIScrollSmooth", "4");
        CF.get()->ParamSet("ANSIScrollBuffer", "100");
        CF.get()->ParamSet("ANSIBufferAbove", "1");
        CF.get()->ParamSet("ANSIBufferBelow", "1");

        CF.get()->ParamSet("ANSICharsDOS", "0020,263A,263B,2665,2666,2663,2660,2022,25D8,25CB,25D9,2642,2640,266A,266B,263C,25BA,25C4,2195,203C,00B6,00A7,25AC,21A8,2191,2193,2192,2190,221F,2194,25B2,25BC");
        CF.get()->ParamSet("ANSICharsVT52", "0020,0020,2588,00B9,00B3,2075,2077,00B0,00B1,2192,2026,00F7,2193,2594,1FB76,1FB77,1FB78,1FB79,1FB7A,1FB7B,2581,2080,2081,2082,2083,2084,2085,2086,2087,2088,2089,00B6");

        CF.get()->ParamSet("ANSIColorThresholdBlackWhite", "48");
        CF.get()->ParamSet("ANSIColorThresholdGray", "20");

        CF.get()->ParamSet("FileDelayStep", "100");
        CF.get()->ParamSet("FileDelayOffset", "1000000000");
        CF.get()->ParamSet("FileDelayTime", "40");
        CF.get()->ParamSet("FileDelayFrame", "200");
        CF.get()->ParamSet("FileBrowseWildcard", "*.txt;*.TXT;*.ans;*.ANS;*.vt;*.VT;*.nfo;*.NFO;*.diz;*.DIZ");
        CF.get()->ParamSet("ServerPort", "0");
        CF.get()->ParamSet("ServerEncoding", "65001");
        CF.get()->ParamSet("ServerTelnet", "1");

        CF.get()->ParamSet("TerminalEncoding", "65001");
        CF.get()->ParamSet("TerminalName", "xterm-256color");
        CF.get()->ParamSet("TerminalType", "1");
        CF.get()->ParamSet("TerminalVTFuncKeys", "0");
        CF.get()->ParamSet("TerminalVTArrowKeys", "0");
        CF.get()->ParamSet("TerminalFile", "");
        CF.get()->ParamSet("TerminalTimeResolution", "80");
        CF.get()->ParamSet("TerminalStep", "200");
        CF.get()->ParamSet("TerminalKeys", "1101000");
        CF.get()->ParamSet("TerminalAnswerBack", "TextPaint");

        CF.get()->ParamSet("RenderType", "PNG");
        CF.get()->ParamSet("RenderFile", "File.png");
        CF.get()->ParamSet("RenderStep", "50");
        CF.get()->ParamSet("RenderOffset", "0");
        CF.get()->ParamSet("RenderCursor", "1");
        CF.get()->ParamSet("RenderFrame", "200");
        CF.get()->ParamSet("RenderSliceX", "0");
        CF.get()->ParamSet("RenderSliceY", "0");
        CF.get()->ParamSet("RenderSliceW", "0");
        CF.get()->ParamSet("RenderSliceH", "0");
        CF.get()->ParamSet("RenderLeading", "0");
        CF.get()->ParamSet("RenderTrailing", "0");
        CF.get()->ParamSet("RenderBlinkPeriod", "10");
        CF.get()->ParamSet("RenderBlinkOffset", "0");


        CF.get()->ParamSet("_WinBitmapFontPage0", 0x00);
        CF.get()->ParamSet("_WinBitmapFontPage1", 0x01);
        CF.get()->ParamSet("_WinBitmapFontPage2", 0x02);
        CF.get()->ParamSet("_WinBitmapFontPage3", 0x03);
        CF.get()->ParamSet("_WinBitmapFontPage4", 0x04);
        CF.get()->ParamSet("_WinBitmapFontPage5", 0x05);
        CF.get()->ParamSet("_WinBitmapFontPage6", 0x1d);
        CF.get()->ParamSet("_WinBitmapFontPage7", 0x1e);
        CF.get()->ParamSet("_WinBitmapFontPage8", 0x20);
        CF.get()->ParamSet("_WinBitmapFontPage9", 0x21);
        CF.get()->ParamSet("_WinBitmapFontPage10", 0x22);
        CF.get()->ParamSet("_WinBitmapFontPage11", 0x23);
        CF.get()->ParamSet("_WinBitmapFontPage12", 0x25);
        CF.get()->ParamSet("_WinBitmapFontPage13", 0x26);
        CF.get()->ParamSet("_WinBitmapFontPage14", 0x27);
        CF.get()->ParamSet("_WinBitmapFontPage15", 0x28);
        CF.get()->ParamSet("_WinBitmapFontPage16", 0x2e);
        CF.get()->ParamSet("_WinBitmapFontPage17", 0xfb);
        CF.get()->ParamSet("_WinBitmapFontPage18", 0xff);
        CF.get()->ParamSet("_WinBitmapFontPage19", 0x1fb);
        
        CF.get()->ParamSet("_FavChar65", 0x2591);
        CF.get()->ParamSet("_FavChar66", 0x2592);
        CF.get()->ParamSet("_FavChar67", 0x2593);
        CF.get()->ParamSet("_FavChar49", 0x25E2);
        CF.get()->ParamSet("_FavChar50", 0x25E3);
        CF.get()->ParamSet("_FavChar51", 0x25E4);
        CF.get()->ParamSet("_FavChar52", 0x25E5);


        //CF.get()->ParamSet("TerminalKeys", "1101000");


        //CF.get()->ParamSet("TerminalKeys", "1101200");
        CF.get()->ParamSet("ANSIReadCR", "0");
        CF.get()->ParamSet("ANSIReadLF", "1");
        CF.get()->ParamSet("TerminalType", "4");


        // Assigning screen functions
        Screen::ScreenChar = ScreenChar;
        Screen::ScreenClear = ScreenClear;
        Screen::ScreenResize_ = ScreenResize;
        Screen::ScreenCursorMove_ = ScreenCursorMove;
        Screen::ScreenTextMove = ScreenTextMove;
        Screen::ClipboardCopy = ClipboardCopy;
        Screen::ClipboardPaste = ClipboardPaste;
        Screen::WorkerSend = WorkerSend;

        // Creating double font table
        Screen::AddDoubleRange(-1, -1);
        Screen::StaticInit();

        Init0();
   
        BufNum(100);
        RespondFinish();
    }

    EMSCRIPTEN_KEEPALIVE
    void EventTick(char * data, int size)
    {
        RespondClear();
        Core.get()->EventTick();
        RespondFinish();
    }

    EMSCRIPTEN_KEEPALIVE
    void EventKey(char * data, int size)
    {
        BufSet(data);
    
        std::string KeyName = BufGetStr();
        int KeyChar = BufGetNum();
        bool ModShift = (BufGetNum() == 1);
        bool ModCtrl = (BufGetNum() == 1);
        bool ModAlt = (BufGetNum() == 1);

        RespondClear();
    
        if (1)
        {
            Core.get()->EventKey(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            if (Core.get()->AppExit)
            {
                CF.get()->ParamSet("WorkMode", 9);
                Init0();
            }
            else
            {
                if (ProgMenu)
                {
                    int Opt = ((Core9Menu*)Core.get())->Opt;
                    CF.get()->ParamSet("WorkMode", Opt);
                    Init0();
                }
            }
        }
        else
        {
            std::cout << "testowanie klawisz >>>  " << KeyName << " " << KeyChar << std::endl;
            int TT = 'X';
            while (TT <= 'Z')
            {
                ScreenChar(XXX, YYY, TT, 0, 7, 0, 0, 0);
                TT++;
                XXX++;
                if (XXX == 80)
                {
                    XXX = 0;
                    YYY++;
                }
                ScreenCursorMove(XXX, YYY);
            }
        }

        RespondFinish();
    }

    EMSCRIPTEN_KEEPALIVE
    void EventOther(char * data, int size)
    {
        BufSet(data);
        std::string EvtName = BufGetStr();
        std::string EvtParam0 = BufGetStr();
        int EvtParam1 = BufGetNum();
        int EvtParam2 = BufGetNum();
        int EvtParam3 = BufGetNum();
        int EvtParam4 = BufGetNum();
        RespondClear();
        Core.get()->EventOther(EvtName, EvtParam0, EvtParam1, EvtParam2, EvtParam3, EvtParam4);
        RespondFinish();
    }
}
