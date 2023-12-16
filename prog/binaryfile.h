#ifndef BINARYFILE_H
#define BINARYFILE_H

#include <memory>
#include "textcodec.h"
#include <fstream>
#include <iostream>
#include "str.h"

class BinaryFile
{
public:
    BinaryFile();
    int ListIndex = 0;
    int ListIndex_ = 0;
    int ListDispOffset = 0;
    int ListDispOffset_ = 0;
    XList<Str> ListName;
    XList<Raw> ListData;
    void Refresh();
    void Init(int CodecR, int CodecW);
    void SetCRLF(int CR, int LF);
    void Save(Str &Text);
    void Load(Str &Text);
private:
    XList<std::shared_ptr<TextCodec>> ListCodec;
    void FileXmas0();
    void FileXmas1();
    void FileXmas2();
    void FileXmas3();
    void FileXmas4();
    void FileXmas5();
    void FileXmas6();
    void FileXmas7();
    void FileXmas8();
    void FileXmas9();
    void FilePrepare(unsigned char * RawX, int RawC);
};

#endif // BINARYFILE_H
