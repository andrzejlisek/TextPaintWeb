#ifndef TEXTCODEC_H
#define TEXTCODEC_H

#include "str.h"
#include "raw.h"
#include "xlist.h"
#include <string>
#include <map>

class TextCodec
{
public:
    int CR = 0;
    int LF = 0;
    TextCodec();
    TextCodec(int CodePage);
    static constexpr inline int BOM = 0xFEFF;
    static constexpr inline int NONE = 0;
    static constexpr inline int DOS = 1;
    static constexpr inline int BASE64 = 2;
    static constexpr inline int HEX = 3;
    static constexpr inline int UTF8 = 65001;
    static constexpr inline int UTF16LE = 1200;
    static constexpr inline int UTF16BE = 1201;
    static constexpr inline int UTF32LE = 12000;
    static constexpr inline int UTF32BE = 12001;
    void Reset();
    void AddBOM();
    void RemoveBOM();
    void EnqueueRaw(Raw &Data);
    void DequeueRaw(Raw &Data);
    void EnqueueStr(Str &Text);
    void DequeueStr(Str &Text);
    void Encode(Str &Text, Raw &Data);
    void Decode(Raw &Data, Str &Text);
    bool EncodingOneByte;
    int EncodingNumber;
    std::string EncodingName;
    int MapByteToText[256];
    std::map<int, unsigned char> MapTextToByte;
    static void Transcode(Str &S, int CodecSrc, int CodecDst);
    static void Transcode(Raw &S, int CodecSrc, int CodecDst);
    bool Unicode = false;

    static inline XList<int> CodecListNumber;
    static inline XList<std::string> CodecListName;
    static void CodecListCreateItem(int Num);
    static void CodecListCreate();
private:
    char Base64Chars[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    char HexChars[33] = "0123456789ABCDEF0123456789abcdef";
    Raw RawData;
    std::string Map0;
    std::string Map1;
    std::string Map2;
    std::string Map3;
    int DequeueTextState = 0;
    unsigned int Temp[10];
    unsigned int BOM__[10];
    void BOMPrepare(unsigned int N, unsigned int B1, unsigned int B2, unsigned int B3, unsigned int B4);
};

#endif // TEXTCODEC_H
