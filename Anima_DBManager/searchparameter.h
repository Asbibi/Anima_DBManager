#ifndef SEARCHPARAMETER_H
#define SEARCHPARAMETER_H

#include "attributetype.h"
#include "sstringhelper.h"
#include <QMap>
#include <QString>

struct SearchParameter
{
    QString mySearchedString;

    bool mySearchOnStructs;
    QMap<AttributeTypeHelper::Type, bool> myAttributeIgnoreSearchMap;
    bool mySearchOnStrings;
    QMap<SStringHelper::SStringLanguages, bool> myLanguageIgnoreSearchMap;
    bool mySearchOnEnum;
};

#endif // SEARCHPARAMETER_H
