#include "binaryfile.h"

BinaryFile::BinaryFile()
{
    TextCodec B64;

    ListName.Add(Str("Dump.txt"));
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







    /*ListName.Add(Str("Formaty"));
    ListName.Add(Str("Prosty1"));
    ListName.Add(Str("Prosty2"));*/
    /*ListName.Add(Str("xmas0.txt"));
    ListName.Add(Str("xmas2.txt"));
    ListName.Add(Str("xmas3.txt"));*/


    Str FormatTest;
    TextCodec TC;

    FormatTest.Clear();
    FormatTest.Add(27);
    FormatTest.AddString("[0m");
    FormatTest.AddString(" A Normalny X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[1m");
    FormatTest.AddString(" A Gruby    X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[1;3m");
    FormatTest.AddString(" A GruPoc   X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[3m");
    FormatTest.AddString(" A Pochyly  X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[0m");
    FormatTest.AddString(" A Normalny X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[7m");
    FormatTest.AddString(" A Inwersja X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[8m");
    FormatTest.AddString(" A Ukryty   X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[4m");
    FormatTest.AddString(" A Podkresk X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[9m");
    FormatTest.AddString(" A Przekre  X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    FormatTest.Add(27);
    FormatTest.AddString("[5m");
    FormatTest.AddString(" A Mruganie X");
    FormatTest.Add(27);
    FormatTest.AddString("[0m\n");

    TC.Reset();
    TC.EnqueueStr(FormatTest);
    Raw DD;
    TC.DequeueRaw(DD);


    ListData.Add(Raw());
    ListData[0].AddRange(DD);
    ListData[0].Clear();

    //!!!!!!!!!!!! Zaczytanie pliku do testowania
    /*for (int Idx = 0; Idx <= 1; Idx++)
    {
        std::fstream FS("/media/xxx/WORK1/__ToBackup/Develop/HTML/WebAssembly/TextPaint/" + ListName[Idx].ToString(), std::ios::binary | std::ios::in);
        if (FS.is_open())
        {
            FS.seekg(0, std::ios_base::end);
            int FileSize = FS.tellg();
            std::cout << FileSize << std::endl;
            //FileSize = 739;
            FS.seekg(0);

            unsigned char * FileRaw = new unsigned char[FileSize];
            FS.read((char*)FileRaw, FileSize);
            for (int I = 0; I < FileSize; I++)
            {
                if (FileRaw[I] == 0x1B)
                {
                    //FileRaw[I] = '_';
                }
                if (FileRaw[I] == '\n')
                {
                    //ListData[Idx].Add('\r');
                }
                ListData[Idx].Add(FileRaw[I]);
            }
            delete[] FileRaw;
            FS.close();
        }
    }*/
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
