#include "sstringtable.h"


SStringTable::SStringTable(const QString& _tableName) : myTableName(_tableName) {}

int SStringTable::GetStringItemCount() const
{
    return (int)(myStrings.size());
}

const QString& SStringTable::GetTableName() const
{
    return myTableName;
}



int SStringTable::GetIndexFromIdentifier(const QString& _identifier) const
{
    const uint count = GetStringItemCount();
    for(uint i = 0; i < count; i++)
    {
        if (myStrings[i].GetIdentifier() == _identifier)
        {
            return i;
        }
    }
    return -1;
}

const QString* SStringTable::GetString(int _index, SStringHelper::SStringLanguages _language) const
{
    if (_index < 0 || _index >= GetStringItemCount())
        return nullptr;

    return &(myStrings[_index].GetString(_language));
}

const QString* SStringTable::GetString(const QString& _identifier, SStringHelper::SStringLanguages _language) const
{
    return GetString(GetIndexFromIdentifier(_identifier), _language);
}

const std::vector<SStringItem>& SStringTable::GetStringItems() const
{
    return myStrings;
}

const SStringItem* SStringTable::GetStringItem(int _index) const
{
    if (_index < 0 || _index >= GetStringItemCount())
        return nullptr;
    return &myStrings[_index];
}

const SStringItem* SStringTable::GetStringItem(const QString& _identifier) const
{
    return GetStringItem(GetIndexFromIdentifier(_identifier));
}




SStringItem* SStringTable::GetStringItemW(int _index)
{
    if (_index < 0 || _index >= GetStringItemCount())
        return nullptr;
    return &myStrings[_index];
}

SStringItem* SStringTable::GetStringItemW(const QString& _identifier)
{
    return GetStringItemW(GetIndexFromIdentifier(_identifier));
}

QString SStringTable::AddStringItem(const QString* _wantedIdentifier /*= nullptr*/)
{
    if (_wantedIdentifier && GetIndexFromIdentifier(*_wantedIdentifier) == -1)
    {
        myStrings.push_back(SStringItem(*_wantedIdentifier));
        return *_wantedIdentifier;
    }

    const QString baseIdentifier = _wantedIdentifier ? *_wantedIdentifier : myTableName;
    int addition = 0;
    QString identifier;
    do
    {
        addition++;
        identifier = baseIdentifier + '_' + QString::number(addition);
    }
    while (GetIndexFromIdentifier(identifier) != -1);

    myStrings.push_back(SStringItem(identifier));
    return identifier;

    //#include <cstdlib>
    //int aleat = (rand() * (int)(1000) / RAND_MAX);
}

void SStringTable::AddStringItemWithTexts(const QString _texts[], const QString* _wantedIdentifier)
{
    auto outId = AddStringItem(_wantedIdentifier);
    SStringItem* item = GetStringItemW(outId);
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        item->SetString((SStringHelper::SStringLanguages)i, _texts[i]);
    }
}

void SStringTable::RemoveStingItem(int _index)
{
    if (_index < 0 || _index >= GetStringItemCount())
        return;

    myStrings.erase(myStrings.begin() + _index);
}

void SStringTable::RemoveStingItem(const QString& _identifier)
{
    RemoveStingItem(GetIndexFromIdentifier(_identifier));
}
