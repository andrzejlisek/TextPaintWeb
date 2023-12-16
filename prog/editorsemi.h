#ifndef EDITORSEMI_H
#define EDITORSEMI_H

#include "editordata.h"
#include "textwork.h"
#include "configfile.h"
#include <memory>

class EditorSemi
{
public:
    std::shared_ptr<EditorData> EditorData_;
    EditorSemi();
    void Init(std::shared_ptr<ConfigFile> CF);
    int DiamondType = 0;
    int FramePencil = 0;
    int FramePencilLastCross = 0;
    std::string GetFrameName(int F);
    void SetFrame(int F1, int F2);
    void SetFrameNext(int N);
    XList<XList<int>> Frame1C;
    XList<std::string> Frame1N;
    XList<XList<int>> Frame2C;
    XList<std::string> Frame2N;
    void FrameCharPut(int Dir, int FontW, int FontH, bool CharDbl);
    int CursorX;
    int CursorY;
    bool DoubleDrawMode();
    int FrameChar[22];
    void FrameCharPut1(int X, int Y, bool ForceT, bool ForceB, bool ForceL, bool ForceR, int FontW, int FontH, bool DoubleChar);
    void FrameCharPut2(int X, int Y, bool ForceTR, bool ForceBL, bool ForceTL, bool ForceBR, int FontW, int FontH, bool DoubleChar);
    void RectangleDraw(int X, int Y, int W__, int H__, int T, int FontW, int FontH);
    void DiamondDraw(int X, int Y, int W__, int H__, int T, int TT, int FontW, int FontH);
private:
    int CharGet(int X, int Y);
    void CharPut(int X, int Y, int C);
    void CharPutDbl(int X, int Y, int C, int Offset);
    int Frame1I;
    int Frame2I;
};

#endif // EDITORSEMI_H
