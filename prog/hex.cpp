#include "hex.h"

Hex::Hex()
{

}

std::string Hex::IntToHex(int Data)
{
    if (Data == 0)
    {
        return "0";
    }
    std::string H = IntToHex32(Data);
    while (H[0] == '0')
    {
        H = H.substr(1);
    }
    return H;
}

///
/// \brief Eden::IntToHex4 - Changes number to 4-bit hex (one character)
/// \param Data
/// \return
///
std::string Hex::IntToHex4(int Data)
{
    switch (Data)
    {
        case 0: return "0";
        case 1: return "1";
        case 2: return "2";
        case 3: return "3";
        case 4: return "4";
        case 5: return "5";
        case 6: return "6";
        case 7: return "7";
        case 8: return "8";
        case 9: return "9";
        case 10: return "A";
        case 11: return "B";
        case 12: return "C";
        case 13: return "D";
        case 14: return "E";
        case 15: return "F";
    }
    return "?";
}

///
/// \brief Eden::IntToHex8 - Changes number to 8-bit hex (two characters)
/// \param Data
/// \return
///
std::string Hex::IntToHex8(int Data)
{
    int D1 = Data >> 4;
    int D2 = Data & 15;
    return IntToHex4(D1) + IntToHex4(D2);
}

///
/// \brief Eden::IntToHex16 - Changes number to 16-bit hex (four characters)
/// \param Data
/// \return
///
std::string Hex::IntToHex16(int Data)
{
    int D1 = Data >> 8;
    int D2 = Data & 255;
    return IntToHex8(D1) + IntToHex8(D2);
}

///
/// \brief Eden::IntToHex32 - Changes number to 32-bit hex (eight characters)
/// \param Data
/// \return
///
std::string Hex::IntToHex32(int Data)
{
    int D1 = Data >> 16;
    int D2 = Data & 65535;
    return IntToHex16(D1) + IntToHex16(D2);
}

///
/// \brief Eden::IntToHex32 - Changes unsigned number to 32-bit hex (eight characters)
/// \param Data
/// \return
///
std::string Hex::IntToHex32(unsigned int Data)
{
    unsigned int D1 = Data >> 16;
    unsigned int D2 = Data & 65535;
    return IntToHex16((int)D1) + IntToHex16((int)D2);
}

///
/// \brief Eden::ToHex - Converts byte array to hex representation
/// \param Raw
/// \param RawSize
/// \return
///
std::string Hex::ToHex(char *Raw, unsigned int RawSize)
{
    return ToHex((unsigned char*)Raw, RawSize);
}

///
/// \brief Eden::ToHex - Converts byte array to hex representation
/// \param Raw
/// \param RawSize
/// \return
///
std::string Hex::ToHex(unsigned char *Raw, unsigned int RawSize)
{
    std::string X = "";
    for (unsigned int I = 0; I < RawSize; I++)
    {
        X = X + Hex::IntToHex8(Raw[I]);
    }
    return X;
}

///
/// \brief Eden::HexToInt - Changes 8-bit or 16-bit hex number to integer
/// \param Hex0
/// \return
///
int Hex::HexToInt(std::string Hex0)
{
    int L = Hex0.length();
    std::string Hex = "";
    for (int i = 0; i < L; i++)
    {
        Hex = Hex0[i] + Hex;
    }
    int D = 0;
    int N = 0;
    for (int i = L - 1; i >= 0; i--)
    {
        D = 0;
        if (Hex[i] == '0') { D = 0; }
        if (Hex[i] == '1') { D = 1; }
        if (Hex[i] == '2') { D = 2; }
        if (Hex[i] == '3') { D = 3; }
        if (Hex[i] == '4') { D = 4; }
        if (Hex[i] == '5') { D = 5; }
        if (Hex[i] == '6') { D = 6; }
        if (Hex[i] == '7') { D = 7; }
        if (Hex[i] == '8') { D = 8; }
        if (Hex[i] == '9') { D = 9; }
        if (Hex[i] == 'A') { D = 10; }
        if (Hex[i] == 'B') { D = 11; }
        if (Hex[i] == 'C') { D = 12; }
        if (Hex[i] == 'D') { D = 13; }
        if (Hex[i] == 'E') { D = 14; }
        if (Hex[i] == 'F') { D = 15; }
        if (Hex[i] == 'a') { D = 10; }
        if (Hex[i] == 'b') { D = 11; }
        if (Hex[i] == 'c') { D = 12; }
        if (Hex[i] == 'd') { D = 13; }
        if (Hex[i] == 'e') { D = 14; }
        if (Hex[i] == 'f') { D = 15; }
        if (i == 0) { N = N + (D); }
        if (i == 1) { N = N + (D * 16); }
        if (i == 2) { N = N + (D * 256); }
        if (i == 3) { N = N + (D * 4096); }
        if (i == 4) { N = N + (D * 65536); }
        if (i == 5) { N = N + (D * 1048576); }
        if (i == 6) { N = N + (D * 16777216); }
        if (i == 7) { N = N + (D * 268435456); }
    }
    return N;
}

///
/// \brief Eden::HexToInt - Changes 8-bit or 16-bit hex number to integer
/// \param Hex0
/// \return
///
int Hex::HexToInt(char Hex0)
{
    switch (Hex0)
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        case 'a': return 10;
        case 'b': return 11;
        case 'c': return 12;
        case 'd': return 13;
        case 'e': return 14;
        case 'f': return 15;
    }
    return 0;
}
