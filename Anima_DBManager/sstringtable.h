#ifndef SSTRINGTABLE_H
#define SSTRINGTABLE_H

#include "sstringhelper.h"
#include "sstringitem.h"

#include <QString>
#include <vector>

class SStringTable
{
private:
    QString myTableName;
    std::vector<SStringItem> myStrings;

public:
    SStringTable(const QString& _tableName);

    const QString& GetTableName() const;
    int GetStringItemCount() const;
    int GetIndexFromIdentifier(const QString& _identifier) const;
    const QString* GetString(int _index, SStringHelper::SStringLanguages _language) const;
    const QString* GetString(const QString& _identifier, SStringHelper::SStringLanguages _language) const;
    const std::vector<SStringItem>& GetStringItems() const;
    const SStringItem* GetStringItem(int _index) const;
    const SStringItem* GetStringItem(const QString& _identifier) const;

    SStringItem* GetStringItemW(int _index);
    SStringItem* GetStringItemW(const QString& _identifier);
    QString AddStringItem(const QString* _wantedIdentifier = nullptr); // return the actual identifier
    void AddStringItemWithTexts(const QString _texts[], const QString* _wantedIdentifier = nullptr);
    void RemoveStingItem(int _index);
    void RemoveStingItem(const QString& _identifier);
};

#endif // SSTRINGTABLE_H
