#include "raw.h"

Raw::Raw()
{

}

Raw Raw::Copy()
{
    Raw L;
    for (int I = 0; I < Count; I++)
    {
        L.Data.push_back(Data[I]);
    }
    L.Count += Count;
    return L;
}
