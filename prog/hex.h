#ifndef HEX_H
#define HEX_H

#include <string>

class Hex
{
public:
    Hex();
    static std::string IntToHex(int Data);
    static std::string IntToHex4(int Data);
    static std::string IntToHex8(int Data);
    static std::string IntToHex16(int Data);
    static std::string IntToHex32(int Data);
    static std::string IntToHex32(unsigned int Data);

    static std::string ToHex(char * Raw, unsigned int RawSize);
    static std::string ToHex(unsigned char * Raw, unsigned int RawSize);

    static int HexToInt(std::string Hex0);
};

#endif // HEX_H
