#include "binaryfileitem.h"

BinaryFileItem::BinaryFileItem(Str Name_, std::shared_ptr<TextCodec> Codec_, int Type_, int EventId_, bool Ansi_)
{
    Name = Name_;
    Codec = Codec_;
    Type = Type_;
    EventId = EventId_;
    Ansi = Ansi_;
}

void BinaryFileItem::Save(Str &Text, Raw &Data)
{
    Data.Clear();
    Codec.get()->Reset();
    Codec.get()->AddBOM();
    Codec.get()->EnqueueStr(Text);
    Codec.get()->DequeueRaw(Data);
}

void BinaryFileItem::Load(Raw &Data, Str &Text)
{
    Codec.get()->Reset();
    Codec.get()->EnqueueRaw(Data);
    Codec.get()->RemoveBOM();
    Codec.get()->DequeueStr(Text);
}
