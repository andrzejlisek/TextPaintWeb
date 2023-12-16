#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "str.h"
#include "textcodec.h"
#include <string>

class Clipboard
{
public:
    Clipboard();
    void SetText(Str Text);
    Str GetText();
    std::string SystemText;
};

#endif // CLIPBOARD_H
