#include "binaryfileitem.h"

BinaryFileItem::BinaryFileItem(Str Name_, int Type_, int EventId_, int Codec_, bool Ansi_)
{
    Name = Name_;
    Type = Type_;
    EventId = EventId_;
    Codec = std::make_shared<TextCodec>(Codec_);
    Ansi = Ansi_;
}

BinaryFileItem::BinaryFileItem(Str Name_, int Type_, int EventId_, std::string Attrib_)
{
    Name = Name_;
    Type = Type_;
    EventId = EventId_;
    AttribSet(Attrib_);
}

std::string BinaryFileItem::AttribGet()
{
    std::stringstream Attrib;;
    Attrib << std::to_string(Codec.get()->EncodingNumber);
    Attrib << "|";
    Attrib << (Ansi ? "1" : "0");
    Attrib << "|";
    return Attrib.str();
}

void BinaryFileItem::AttribSet(std::string Attrib)
{
    Attrib = Attrib + "|";
    int Idx = 0;
    Str Buf;
    for (int I = 0; I < Attrib.size(); I++)
    {
        if (Attrib[I] != '|')
        {
            Buf.Add(Attrib[I]);
        }
        else
        {
            switch (Idx)
            {
                case 0:
                    Codec = std::make_shared<TextCodec>(std::stoi(Buf.ToString()));
                    break;
                case 1:
                    if ((Buf.ToString() == "") || (Buf.ToString() == "0"))
                    {
                        Ansi = false;
                    }
                    else
                    {
                        Ansi = true;
                    }
                    break;
            }
            Buf.Clear();
            Idx++;
        }
    }
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
