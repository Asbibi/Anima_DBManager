#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QStringList>

struct SearchResult
{
    QStringList myDisplayString;    // 5 values expected

    int myTableIndex;               // Struct Table, String Table, Enum
    int myRowIndex;                 // structure, string item, enum value
    int myColIndex;                 // attribute, language, -
};

#endif // SEARCHRESULT_H
