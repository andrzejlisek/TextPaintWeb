#ifndef RAW_H
#define RAW_H

#include "xlist.h"

class Raw : public XList<unsigned char>
{
public:
    Raw();
    Raw Copy();
};

#endif // RAW_H
