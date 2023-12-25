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
    bool Ansi;
    Str Name;
    std::shared_ptr<TextCodec> Codec;
    BinaryFileItem(Str Name_, int Type_, int EventId_, int Codec_, bool Ansi_);
    BinaryFileItem(Str Name_, int Type_, int EventId_, std::string Attrib_);

    std::string AttribGet();
    void AttribSet(std::string Attrib);

    bool operator!= (BinaryFileItem &_) { return Name != _.Name; }
    bool operator>  (BinaryFileItem &_) { return Name >  _.Name; }
    bool operator>= (BinaryFileItem &_) { return Name >= _.Name; }
    bool operator== (BinaryFileItem &_) { return Name == _.Name; }
    bool operator<= (BinaryFileItem &_) { return Name <= _.Name; }
    bool operator<  (BinaryFileItem &_) { return Name <  _.Name; }

    void Save(Str &Text, Raw &Data);
    void Load(Raw &Data, Str &Text);
};

#endif // BINARYFILEITEM_H
