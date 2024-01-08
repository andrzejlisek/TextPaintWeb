#ifndef BINARYFILEITEM_H
#define BINARYFILEITEM_H

#include <memory>
#include "str.h"
#include "textcodec.h"
#include <iostream>

class BinaryFileItem
{
public:
    int EventId;
    int Type;
    Str Name;
    BinaryFileItem(Str Name_, int Type_, int EventId_, std::string Attrib_);

    bool Uninitialized = true;
    std::string Attrib;
    int Attrib_Codec;
    bool Attrib_Ansi;

    std::string AttribGet();
    void AttribSet();

    bool operator!= (BinaryFileItem &_) { return Name != _.Name; }
    bool operator>  (BinaryFileItem &_) { return Name >  _.Name; }
    bool operator>= (BinaryFileItem &_) { return Name >= _.Name; }
    bool operator== (BinaryFileItem &_) { return Name == _.Name; }
    bool operator<= (BinaryFileItem &_) { return Name <= _.Name; }
    bool operator<  (BinaryFileItem &_) { return Name <  _.Name; }

    std::string GetCodecName();
    void Save(Str &Text, Raw &Data);
    void Load(Raw &Data, Str &Text);
    void Browser(int Set);
private:
    int Type_;
};

#endif // BINARYFILEITEM_H
