#ifndef SSTRINGITEM_H
#define SSTRINGITEM_H

#include "sstringhelper.h"

#include <QString>
#include <fstream>

class SStringItem
{
private:
    QString myIdentifier;
    QString myStrings[SStringHelper::SStringLanguages::Count];

public:
    SStringItem(const QString& _identifier);
    SStringItem(const SStringItem& _another);
    void operator=(const SStringItem& _another);
    bool operator==(const SStringItem& _other) const;
    bool operator<(const SStringItem& _other) const;
    bool operator>(const SStringItem& _other) const;

    const QString& GetIdentifier() const;
    const QString& GetString(SStringHelper::SStringLanguages _language) const;

    void SetIdentifier(const QString& _identifier);
    void SetString(SStringHelper::SStringLanguages _language, const QString& _str);

    void WriteValue_CSV(std::ofstream& _file, SStringHelper::SStringLanguages _language, bool _withDictionaryReplacement) const;
};

#endif // SSTRINGITEM_H
