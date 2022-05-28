#ifndef SSTRINGITEM_H
#define SSTRINGITEM_H

#include "sstringhelper.h"

#include <QString>

class SStringItem
{
private:
    const QString myIdentifier;
    QString myStrings[SStringHelper::SStringLanguages::Count];

public:
    SStringItem(const QString& _identifier);

    const QString& GetIdentifier() const;
    const QString& GetString(SStringHelper::SStringLanguages _language) const;
    void SetString(SStringHelper::SStringLanguages _language, const QString& _str);

    bool operator=(const SStringItem& _other) const;
};

#endif // SSTRINGITEM_H
