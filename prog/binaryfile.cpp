#include "binaryfile.h"

BinaryFile::BinaryFile()
{
    TextCodec B64;

    ListName.Add(Str("blank"));
    ListData.Add(Raw());
    ListCodec.Add(std::make_shared<TextCodec>(65001));


    ListName.Add(Str("xmas0"));
    ListData.Add(Raw());
    FileXmas0();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas1"));
    ListData.Add(Raw());
    FileXmas1();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas2"));
    ListData.Add(Raw());
    FileXmas2();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas3"));
    ListData.Add(Raw());
    FileXmas3();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas4"));
    ListData.Add(Raw());
    FileXmas4();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas5"));
    ListData.Add(Raw());
    FileXmas5();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas6"));
    ListData.Add(Raw());
    FileXmas6();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas7"));
    ListData.Add(Raw());
    FileXmas7();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas8"));
    ListData.Add(Raw());
    FileXmas8();
    ListCodec.Add(std::make_shared<TextCodec>(0));

    ListName.Add(Str("xmas9"));
    ListData.Add(Raw());
    FileXmas9();
    ListCodec.Add(std::make_shared<TextCodec>(0));
}

void BinaryFile::Refresh()
{

}

void BinaryFile::Init(int CodecR, int CodecW)
{
    //Codec = std::make_shared<TextCodec>(CodecR);
    //Codec_ = CodecW;
}

void BinaryFile::SetCRLF(int CR, int LF)
{
    ListCodec[ListIndex].get()->CR = CR;
    ListCodec[ListIndex].get()->LF = LF;
}

void BinaryFile::Save(Str &Text)
{
    ListData[ListIndex].Clear();
    ListCodec[ListIndex].get()->Reset();
    ListCodec[ListIndex].get()->AddBOM();
    ListCodec[ListIndex].get()->EnqueueStr(Text);
    ListCodec[ListIndex].get()->DequeueRaw(ListData[ListIndex]);
}

void BinaryFile::Load(Str &Text)
{
    ListCodec[ListIndex].get()->Reset();
    ListCodec[ListIndex].get()->EnqueueRaw(ListData[ListIndex]);
    ListCodec[ListIndex].get()->RemoveBOM();
    ListCodec[ListIndex].get()->DequeueStr(Text);
}
