#ifndef SSTRINGITEM_H
#define SSTRINGITEM_H

#include "sstringhelper.h"

#include <QString>
#include <QList>
#include <fstream>

class SStringItem
{
private:
    QString myIdentifier;
    QList<QString> myStrings;

public:
    SStringItem(const QString& _identifier);
    SStringItem(const SStringItem& _another);
    void operator=(const SStringItem& _another);
    bool operator==(const SStringItem& _other) const;
    bool operator<(const SStringItem& _other) const;
    bool operator>(const SStringItem& _other) const;

    const QString& GetIdentifier() const;
    const QString& GetString(int _languageIndex) const;

    void SetIdentifier(const QString& _identifier);
    void SetString(int _languageIndex, const QString& _str);

    void WriteValue_CSV(std::ofstream& _file, int _languageIndex, bool _withDictionaryReplacement) const;
};

#endif // SSTRINGITEM_H
