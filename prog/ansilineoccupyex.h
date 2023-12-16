#ifndef ANSILINEOCCUPYEX_H
#define ANSILINEOCCUPYEX_H

#include "ansilineoccupy.h"

class AnsiLineOccupyEx : public AnsiLineOccupy
{
public:
    using AnsiLineOccupy::Append;
    using AnsiLineOccupy::Insert;
    using AnsiLineOccupy::Delete;

    AnsiLineOccupyEx();
    int GetDataChar(int Y, int X);
    int GetDataColo(int Y, int X);
    int GetDataAttr(int Y, int X);
    int GetDataFont(int Y, int X);
    void Set_(int Y, int X);
    bool Get_(int Y, int X);
    int CountItemsTrim(int Y);
    void Trim(int Y);
    void TrimLines();
    void DeleteAll();
    Str GetLineString(int Y);
    void SetLineString(int Y, Str Text);
    void SetLineString(int Y, std::string Text);
    void ClearLine(int Y);
    void PadRightTab(int Y, int Size);
    void PadRightSpace(int Y, int Size);
    void PadRight(int Y, int Size);
    void DeleteLeft(int Y, int Size);
    void Crop(int Y, int Start, int Size);
    void LineCopy(AnsiLineOccupyEx Src, int SrcY, int DstY);
    void AppendLineCopy(AnsiLineOccupyEx Src, int SrcY);
    void Insert(int Y, int X, int L);
    void Insert(int Y, int X, AnsiLineOccupyEx Obj, int ObjY);
    void Append(int Y, int L);
    void Append(int Y, AnsiLineOccupyEx Obj, int ObjY);
    void Delete(int Y, int X, int L);
    AnsiLineOccupyEx CloneData();
    void MergeColor(AnsiLineOccupyEx ColorObj);
private:
    XList<int> ItemTemp;
    void ItemTempSave();
    void ItemTempLoad();
};

#endif // ANSILINEOCCUPYEX_H
