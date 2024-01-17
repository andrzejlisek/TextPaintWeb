#ifndef ANSISAUCE_H
#define ANSISAUCE_H

#include <string>
#include "xlist.h"
#include "textwork.h"
#include "str.h"

class AnsiSauce
{
public:
    AnsiSauce();

    XList<std::string> Info;
    Raw InfoRaw;

    void NonSauceInfo(std::string Label, long Value);
    void NonSauceInfo(std::string Label, std::string Value);
    void CreateInfo();
    void LoadRaw(Raw &Raw__);

    int DataIdx = -1;

private:
    int SauceIdx = -1;
    int CommentIdx = -1;
    std::string NameDataType[10] { "None", "Character", "Bitmap", "Vector", "Audio", "BinaryText", "XBin", "Archive", "Executable", "_" };
    std::string NameFileType0[1] { "_" };
    std::string NameFileType1[10] { "ASCII", "ANSi", "ANSiMation", "RIP script", "PCBoard", "Avatar", "HTML", "Source", "TundraDraw", "_" };
    std::string NameFileType2[15] { "GIF", "PCX", "LBM/IFF", "TGA", "FLI", "FLC", "BMP", "GL", "DL", "WPG", "PNG", "JPG/JPeg", "MPG", "AVI", "_" };
    std::string NameFileType3[5] { "DXF", "DWG", "WPG", "3DS", "_" };
    std::string NameFileType4[26] { "MOD", "669", "STM", "S3M", "MTM", "FAR", "ULT", "AMF", "DMF", "OKT", "ROL", "CMF", "MID", "SADT", "VOC", "WAV", "SMP8", "SMP8S", "SMP16", "SMP16S", "PATCH8", "PATCH16", "XM", "HSC", "IT", "_" };
    std::string NameFileType5[1] { "_" };
    std::string NameFileType6[1] { "_" };
    std::string NameFileType7[11] { "ZIP", "ARJ", "LZH", "ARC", "TAR", "ZOO", "RAR", "UC2", "PAK", "SQZ", "_" };
    std::string NameFileType8[1] { "_" };

    std::string Field01Version = "";
    std::string Field02Title = "";
    std::string Field03Author = "";
    std::string Field04Group = "";
    std::string Field05Date = "";
    long Field06FileSize = 0;
    int Field07DataType = 0;
    int Field08FileType = 0;
    int Field09TInfo1 = 0;
    int Field10TInfo2 = 0;
    int Field11TInfo3 = 0;
    int Field12TInfo4 = 0;
    int Field13Comments = 0;
    int Field14TFlags = 0;
    std::string Field15TInfoS = "";
    XList<std::string> Comment;

    int GetRawNum(Str FileCtx, int Idx);
    std::string GetRawChar(Str FileCtx, int Idx);
    std::string GetChar(int Idx);
    int GetByte(int Idx);

    Raw Raw_;

    void ReadSAUCE();
    void ReadCOMNT();

    XList<std::string> NonSauceInfo1;
    XList<std::string> NonSauceInfo2;

};

#endif // ANSISAUCE_H
