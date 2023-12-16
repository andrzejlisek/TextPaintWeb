#include "clipboard.h"

Clipboard::Clipboard()
{

}

void Clipboard::SetText(Str Text)
{
    TextCodec::Transcode(Text, TextCodec::UTF8, 0);
    SystemText = Text.ToString();
}

Str Clipboard::GetText()
{
    Str Temp = Str::FromString(SystemText);
    TextCodec::Transcode(Temp, 0, TextCodec::UTF8);
    return Temp;
}
