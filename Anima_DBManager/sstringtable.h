#ifndef SSTRINGTABLE_H
#define SSTRINGTABLE_H

#include <QObject>

#include "sstringhelper.h"
#include "sstringitem.h"

#include <QString>
#include <QList>
#include <fstream>

class SStringTable : public QObject
{
    Q_OBJECT

private:
    QString myTableName;
    QList<SStringItem> myStrings;

public:
    SStringTable(const QString& _tableName);
    SStringTable(const SStringTable& _another);
    void operator=(const SStringTable& _another);

    const QString& GetTableName() const;
    int GetStringItemCount() const;
    int GetIndexFromIdentifier(const QString& _identifier) const;
    const QString* GetString(int _index, SStringHelper::SStringLanguages _language) const;
    const QString* GetString(const QString& _identifier, SStringHelper::SStringLanguages _language) const;
    const QList<SStringItem>& GetStringItems() const;
    const SStringItem* GetStringItem(int _index) const;
    const SStringItem* GetStringItem(const QString& _identifier) const;

    void SetTableName(QString& _name);
    void SetStringItemCount(int _count);
    SStringItem* GetStringItemW(int _index);
    SStringItem* GetStringItemW(const QString& _identifier);
    QString AddStringItem(int _index, const QString* _wantedIdentifier = nullptr); // return the actual identifier
    void AddStringItemWithTexts(int _index, const QString _texts[], const QString* _wantedIdentifier = nullptr);
    void AddStringItemFromCopy(int _index, const SStringItem& _item);
    void RemoveStringItem(int _index);
    void RemoveStringItem(const QString& _identifier);
    void SwapStringItems(int _indexFirst, int _indexSecond);
    void MoveStringItems(int _indexFrom, int _indexTo);
    void SortStringItems(bool _ascending);

    bool SetItemIdentifier(const int _index, const QString& _identifier);
    void SetItemString(int _row, SStringHelper::SStringLanguages _col, const QString& _text);
    void ImportString(SStringHelper::SStringLanguages _language, const QString& _identifier, const QString& _text, int _overwritePolicy);

    void WriteValue_CSV(std::ofstream& _file, SStringHelper::SStringLanguages _language, bool _withDictionaryReplacement) const;
};

#endif // SSTRINGTABLE_H
