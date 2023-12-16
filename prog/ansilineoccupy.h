#ifndef ANSILINEOCCUPY_H
#define ANSILINEOCCUPY_H

#include "ansilineoccupyitem.h"
#include "xlist.h"

class AnsiLineOccupy : public AnsiLineOccupyItem
{
public:
    int Factor = 6;
    AnsiLineOccupy();
    void Clear();
    void Append(int Y);
    void Insert(int Y, int X);
    void Delete(int Y, int X);
    void AppendLine();
    void InsertLine(int I);
    void AppendLine(AnsiLineOccupy X, int I);
    void InsertLine(AnsiLineOccupy X, int I, int II);
    void DeleteLine(int I);
    void Get(int Y, int X);
    void Set(int Y, int X);
    int CountLines();
    int CountItems(int I);
    static void Copy(AnsiLineOccupy &Src, AnsiLineOccupy &Dst);
    static void Swap(AnsiLineOccupy &Src, AnsiLineOccupy &Dst);
protected:
    XList<Str> Data;
};

#endif // ANSILINEOCCUPY_H
