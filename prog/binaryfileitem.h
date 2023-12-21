#ifndef BINARYFILEITEM_H
#define BINARYFILEITEM_H

#include <memory>
#include "str.h"
#include "textcodec.h"

class BinaryFileItem
{
public:
    int EventId;
    int Type;
    bool Ansi;
    Str Name;
    std::shared_ptr<TextCodec> Codec;
    BinaryFileItem(Str Name_, std::shared_ptr<TextCodec> Codec_, int Type_, int EventId_, bool Ansi_);

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
