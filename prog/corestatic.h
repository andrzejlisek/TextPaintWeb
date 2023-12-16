#ifndef CORESTATIC_H
#define CORESTATIC_H

#include <string>
#include <vector>
#include <iostream>

class CoreStatic
{
public:
    CoreStatic();
    static int FontCounter(int CurrentValue);
    static int FontSizeCode(int S, int N);
    static bool GetAttribBit(int Attrib, int Bit);
    static int SetAttribBit(int Attrib, int Bit, bool Val);
    static std::string GetAttribText(int Attrib);
    static int ColorNegative(int C);

    template<typename T>
    static void Array2(std::vector<std::vector<T>> &V, int Dim1, int Dim2)
    {
        if (Dim1 > Dim2)
        {
            Dim1 = Dim2;
        }
        if (Dim2 < Dim1)
        {
            Dim2 = Dim1;
        }
        while (V.size() < Dim1)
        {
            std::vector<T> VV;
            while (VV.size() < Dim2)
            {
                VV.push_back(T());
            }
            V.push_back(VV);
        }
        /*V.resize(Dim1);
        for (int I = 0; I < Dim2; I++)
        {
            V[I].resize(Dim2);
        }*/
    }

    static inline int FontMaxSize = 32;
    static inline int FontMaxSizeCode = 527;

};

#endif // CORESTATIC_H
