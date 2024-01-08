#include "binaryfileitem.h"

BinaryFileItem::BinaryFileItem(Str Name_, int Type_, int EventId_, std::string Attrib_)
{
    Name = Name_;
    Type = Type_;
    EventId = EventId_;
    Attrib = Attrib_;
    AttribSet();
}

std::string BinaryFileItem::AttribGet()
{
    std::stringstream Attrib_;
    Attrib_ << std::to_string(Attrib_Codec);
    Attrib_ << "|";
    Attrib_ << (Attrib_Ansi ? "1" : "0");
    Attrib_ << "|";
    Attrib = Attrib_.str();
    return Attrib;
}

void BinaryFileItem::AttribSet()
{
    std::string Attrib_ = Attrib + "|";
    int Idx = 0;
    Str Buf;
    for (int I = 0; I < Attrib_.size(); I++)
    {
        if (Attrib_[I] != '|')
        {
            Buf.Add(Attrib_[I]);
        }
        else
        {
            switch (Idx)
            {
                case 0:
                    Attrib_Codec = std::stoi(Buf.ToString());
                    break;
                case 1:
                    if ((Buf.ToString() == "") || (Buf.ToString() == "0"))
                    {
                        Attrib_Ansi = false;
                    }
                    else
                    {
                        Attrib_Ansi = true;
                    }
                    break;
            }
            Buf.Clear();
            Idx++;
        }
    }
}

std::string BinaryFileItem::GetCodecName()
{
    std::unique_ptr<TextCodec> Codec = std::make_unique<TextCodec>(Attrib_Codec);
    return Codec.get()->FullName();
}

void BinaryFileItem::Save(Str &Text, Raw &Data)
{
    std::unique_ptr<TextCodec> Codec = std::make_unique<TextCodec>(Attrib_Codec);
    Data.Clear();
    Codec.get()->Reset();
    Codec.get()->AddBOM();
    Codec.get()->EnqueueStr(Text);
    Codec.get()->DequeueRaw(Data);
}

void BinaryFileItem::Load(Raw &Data, Str &Text)
{
    std::unique_ptr<TextCodec> Codec = std::make_unique<TextCodec>(Attrib_Codec);
    Codec.get()->Reset();
    Codec.get()->EnqueueRaw(Data);
    Codec.get()->RemoveBOM();
    Codec.get()->DequeueStr(Text);
}

void BinaryFileItem::Browser(int Set)
{
    switch (Set)
    {
        case 0:
            {
                Type = Type_;
            }
            break;
        case 1:
            {
                Type_ = Type;
                Type = 6;
            }
            break;
    }
}
