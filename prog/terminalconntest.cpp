#include "terminalconntest.h"

TerminalConnTest::TerminalConnTest()
{
    SW.Reset();
}

int TerminalConnTest::IsConnected()
{
    return 0;
}

void TerminalConnTest::Send(Raw &Data)
{
    Raw Answer;
    if (SW.Elapsed() > 1000)
    {
        Answer.Add(13);
        Answer.Add(10);
        DataSpecial = false;
    }
    for (int I = 0; I < Data.Count; I++)
    {
        if ((Data[I] >= 32) && (Data[I] < 127))
        {
            if (DataSpecial)
            {
                switch (Data[I])
                {
                    case SpecialChar:
                        Answer.Add(Data[I]);
                        break;
                    case '1':
                        Answer.Add(27);
                        Answer.Add('[');
                        Answer.Add('0');
                        Answer.Add('c');
                        break;
                    case '2':
                        Answer.Add(27);
                        Answer.Add('[');
                        Answer.Add('>');
                        Answer.Add('0');
                        Answer.Add('c');
                        break;
                    case '3':
                        Answer.Add(27);
                        Answer.Add('[');
                        Answer.Add('=');
                        Answer.Add('0');
                        Answer.Add('c');
                        break;
                }

                DataSpecial = false;
            }
            else
            {
                if (Data[I] == SpecialChar)
                {
                    DataSpecial = true;
                }
                else
                {
                    Answer.Add(Data[I]);
                }
            }
        }
        else
        {
            std::string Num = std::to_string((int)Data[I]);
            Answer.Add('<');
            for (int I = 0; I < Num.size(); I++)
            {
                Answer.Add(Num[I]);
            }
            Answer.Add('>');
        }
    }
    SW.Reset();
    Msgs.Add(Answer);
}
