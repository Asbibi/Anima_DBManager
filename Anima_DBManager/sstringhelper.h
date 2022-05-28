#ifndef SSTRINGHELPER_H
#define SSTRINGHELPER_H

#include <QString>

namespace SStringHelper {

enum SStringLanguages
{
    French = 0,
    English,

    Count
};

QString GetLanguageString(SStringLanguages _language);

}


#endif // SSTRINGHELPER_H
