#ifndef SEARCHPARAMETER_H
#define SEARCHPARAMETER_H

#include "attributetype.h"
#include "sstringhelper.h"
#include <QMap>
#include <QString>

struct SearchParameter
{
    QString mySearchedString;

    bool mySearchOnStructs                  = false;
    QMap<AttributeTypeHelper::Type, bool> myAttributeIgnoreSearchMap;
    bool mySearchOnStrings                  = false;
    QMap<SStringHelper::SStringLanguages, bool> myLanguageIgnoreSearchMap;
    bool mySearchOnEnum                     = false;

    Qt::CaseSensitivity myCaseSensitivity   = Qt::CaseInsensitive;
    bool myWholeWord                        = false;
};

#endif // SEARCHPARAMETER_H
