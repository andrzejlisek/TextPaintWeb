#ifndef XLIST_H
#define XLIST_H

#include <vector>
#include <deque>
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

    XList<T> GetRange(int Pos, int Count_)
    {
        XList<T> X;
        for (int I = Pos; I < (Count_ + Pos); I++)
        {
            X.Add(Data[I]);
        }
        return X;
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

    int AddSort(T Val)
    {
        if (Count == 0)
        {
            Data.push_back(Val);
            Count++;
            return (Count - 1);
        }

        int Idx = std::abs(IndexOfBin(Val));

        if (Idx > Count)
        {
            Data.insert(Data.begin(), Val);
            Count++;
            return 0;
        }

        if (Idx == Count)
        {
            Data.push_back(Val);
            Count++;
            return (Count - 1);
        }

        Data.insert(Data.begin() + Idx, Val);
        Count++;
        return Idx;
    }

    int AddSortByIndex(int Idx, T Val)
    {
        if (Count == 0)
        {
            Data.push_back(Val);
            Count++;
            return (Count - 1);
        }

        Idx = std::abs(Idx);

        if (Idx > Count)
        {
            Data.insert(Data.begin(), Val);
            Count++;
            return 0;
        }

        if (Idx == Count)
        {
            Data.push_back(Val);
            Count++;
            return (Count - 1);
        }

        Data.insert(Data.begin() + Idx, Val);
        Count++;
        return Idx;
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

    int IndexOf(T Val, int StartPos)
    {
        if (StartPos >= 0)
        {
            if (StartPos >= Count)
            {
                return -1;
            }
            for (int I = StartPos; I < Count; I++)
            {
                if (Data[I] == Val)
                {
                    return I;
                }
            }
        }
        else
        {
            if ((0 - StartPos) >= Count)
            {
                StartPos = 0 - Count + 1;
            }
            for (int I = (0 - StartPos); I >= 0; I--)
            {
                if (Data[I] == Val)
                {
                    return I;
                }
            }
        }
        return -1;
    }

    int IndexOf(T Val)
    {
        return IndexOf(Val, 0);
    }

    int IndexOfBin(T Val, int StartPos, int EndPos)
    {
        if (Count == 0)
        {
            return -1;
        }
        if (Val < Data[0])
        {
            return 0 - 1 - Count;
        }
        int MidPos = 0;
        while (StartPos <= EndPos)
        {
            MidPos = (StartPos + EndPos) >> 1;
            if (Data[MidPos] == Val)
            {
                return MidPos;
            }
            if (Data[MidPos] > Val)
            {
                EndPos = MidPos - 1;
            }
            else
            {
                StartPos = MidPos + 1;
            }
        }
        return 0 - StartPos;
    }

    int IndexOfBin(T Val)
    {
        return IndexOfBin(Val, 0, Count - 1);
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

    int Compare(XList<T> &X)
    {
        int L = Count;
        if (L > X.Count)
        {
            L = X.Count;
        }
        for (int I = 0; I < L; I++)
        {
            T _1 = Data[I];
            T _2 = X[I];
            if (_1 < _2)
            {
                return -1;
            }
            if (_1 > _2)
            {
                return 1;
            }
        }
        if (Count > L) { return 1; }
        if (X.Count > L) { return -1; }
        return 0;
    }

    bool operator== (XList<T> _) { return Compare(_) == 0; }
    bool operator!= (XList<T> _) { return Compare(_) != 0; }
    bool operator>  (XList<T> _) { return Compare(_) >  0; }
    bool operator<  (XList<T> _) { return Compare(_) <  0; }
    bool operator<= (XList<T> _) { return Compare(_) <= 0; }
    bool operator>= (XList<T> _) { return Compare(_) >= 0; }

protected:
    std::vector<T> Data;
};

#endif // XLIST_H
