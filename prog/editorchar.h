#ifndef EDITORCHAR_H
#define EDITORCHAR_H

#include "editordata.h"
#include "configfile.h"
#include <iostream>
#include "coreansi.h"
#include "binaryfile.h"

class EditorChar
{
public:
    std::shared_ptr<BinaryFile> BinaryFile_;
    EditorChar();
    void Init(std::shared_ptr<ConfigFile> CF);
    std::shared_ptr<EditorData> EditorData_;
    std::shared_ptr<CoreAnsi> CoreAnsi_;
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    int RepaintDepth = 0;
    bool FavPage = false;
    int SelectChar = 32;
    int SelectColoB = -1;
    int SelectColoF = -1;
    int SelectColoA = 0;
    int SelectorState = 1;
    int SelectToFav = -1;
    int CharPosMode = 0;
    int SelectCharPart(int N);
    int SelectCharCode();

    int FavChar[256];

    XList<int> WinBitmapPage;
    bool WinBitmapEnabled = true;
    int WinBitmapNearest(int CharCode, bool Backward);

    void SelectCharFavSet(int N);
    int SelectCharFavGet();
private:

    int MaxCharCode = 0x110000;

    void SelectCharChange(int T);
};

#endif // EDITORCHAR_H
