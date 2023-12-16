#ifndef XLIST_H
#define XLIST_H

#include <vector>
#include <stdarg.h>
#include <algorithm>

constexpr unsigned int _(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (_(str, h+1) * 33) ^ str[h];
}

template <class T>
class XList
{
public:
    XList()
    {
        Data.clear();
        Count = 0;
    }

    XList(int N, ...)
    {
        Data.clear();
        Count = N;
        va_list Items;
        va_start(Items, N);
        while (N > 0)
        {
            Data.push_back(va_arg(Items, T));
            N--;
        }
    }

    void Clear()
    {
        Data.clear();
        Count = 0;
    }

    void Add(T Val)
    {
        Data.push_back(Val);
        Count++;
    }

    void AddRange(T * Vals, unsigned int ValsC)
    {
        for (int I = 0; I < ValsC; I++)
        {
            Data.push_back(Vals[I]);
        }
        Count += ValsC;
    }

    void AddRange(XList<T> Vals)
    {
        for (int I = 0; I < Vals.Count; I++)
        {
            Data.push_back(Vals.Data[I]);
        }
        Count += Vals.Count;
    }

    void Remove(int Pos)
    {
        Data.erase(Data.begin() + Pos);
        Count--;
    }

    void RemoveRange(int Pos)
    {
        RemoveRange(Pos, Count - Pos);
    }

    void RemoveRange(int Pos, int Count_)
    {
        Data.erase(Data.begin() + Pos, Data.begin() + Pos + Count_);
        Count -= Count_;
    }

    void Insert(T Val)
    {
        Insert(0, Val);
    }

    void Insert(int Pos, T Val)
    {
        Data.insert(Data.begin() + Pos, Val);
        Count++;
    }

    void InsertRange(XList<T> Vals)
    {
        InsertRange(0, Vals);
    }

    void InsertRange(int Pos, XList<T> Vals)
    {
        Data.insert(Data.begin() + Pos, Vals.Data.begin(), Vals.Data.end());
        Count += Vals.Count;
    }

    void Set(int Pos, T Val)
    {
        Data[Pos] = Val;
    }

    T Get(int Pos)
    {
        return Data[Pos];
    }

    bool Contains(T Val)
    {
        for (int I = 0; I < Count; I++)
        {
            if (Data[I] == Val)
            {
                return true;
            }
        }
        return false;
    }

    int IndexOf(T Val)
    {
        for (int I = 0; I < Count; I++)
        {
            if (Data[I] == Val)
            {
                return I;
            }
        }
        return -1;
    }

    T& operator [](int Pos)
    {
        return Data[Pos];
    }

    XList<T> Concat(XList<T> S)
    {
        XList<T> L = Copy();
        L.AddRange(S);
        return L;
    }

    bool Equals(XList<T> S)
    {
        if (Count != S.Count)
        {
            return false;
        }
        for (int i = 0; i < Count; i++)
        {
            if (Data[i] != S[i])
            {
                return false;
            }
        }
        return true;
    }

    XList<T> Copy()
    {
        XList<T> L;
        for (int I = 0; I < Count; I++)
        {
            L.Data.push_back(Data[I]);
        }
        L.Count = Count;
        return L;
    }

    T PopLast()
    {
        T Temp = Get(Count - 1);
        Remove(Count - 1);
        return Temp;
    }

    T PopFirst()
    {
        T Temp = Get(0);
        Remove(0);
        return Temp;
    }

    void Sort()
    {
        std::sort(Data.begin(), Data.end());
    }

    int Count;

    XList<T> LPad(int I, T Val)
    {
        XList<T> L = Copy();
        while (L.Count < I)
        {
            L.Insert(0, Val);
        }
        return L;
    }

    XList<T> RPad(int I, T Val)
    {
        XList<T> L = Copy();
        while (L.Count < I)
        {
            L.Add(Val);
        }
        return L;
    }

    void InsertPad(int I, T Val)
    {
        while (Count < I)
        {
            Insert(0, Val);
        }
    }

    void AddPad(int I, T Val)
    {
        while (Count < I)
        {
            Add(Val);
        }
    }


protected:
    std::vector<T> Data;
};

#endif // XLIST_H
