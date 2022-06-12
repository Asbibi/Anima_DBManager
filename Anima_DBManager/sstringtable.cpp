#include "sstringtable.h"


SStringTable::SStringTable(const QString& _tableName) :
    QObject(),
    myTableName(_tableName)
{}

SStringTable::SStringTable(const SStringTable& _another) :
    QObject(),
    myTableName(_another.myTableName),
    myStrings(_another.myStrings)
{}
void SStringTable::operator=(const SStringTable& _another)
{
    myTableName = _another.myTableName;
    myStrings.clear();

    for (const auto& item : _another.myStrings)
        myStrings.push_back(item);
}


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

const QList<SStringItem>& SStringTable::GetStringItems() const
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


QString SStringTable::AddStringItem(int _index, const QString* _wantedIdentifier /*= nullptr*/)
{
    if (_index < 0 || _index > GetStringItemCount())
        _index = GetStringItemCount();

    if (_wantedIdentifier && GetIndexFromIdentifier(*_wantedIdentifier) == -1)
    {
        myStrings.push_back(SStringItem(*_wantedIdentifier));
        return *_wantedIdentifier;
    }

    QString baseIdentifier = _wantedIdentifier ? *_wantedIdentifier : myTableName;
    auto validate = [this](const QString& _identifier)->bool{ return (bool)(GetIndexFromIdentifier(_identifier) == -1); };
    const QString identifier = SStringHelper::GetUniqueIdentifier(baseIdentifier, validate, false);


    myStrings.insert(_index, SStringItem(identifier));
    return identifier;
}

void SStringTable::AddStringItemWithTexts(int _index, const QString _texts[], const QString* _wantedIdentifier)
{
    auto outId = AddStringItem(_index, _wantedIdentifier);
    SStringItem* item = GetStringItemW(outId);
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        item->SetString((SStringHelper::SStringLanguages)i, _texts[i]);
    }
}

void SStringTable::RemoveStringItem(int _index)
{
    if (_index < 0 || _index >= GetStringItemCount())
        return;

    myStrings.removeAt(_index);
}

void SStringTable::RemoveStringItem(const QString& _identifier)
{
    RemoveStringItem(GetIndexFromIdentifier(_identifier));
}

void SStringTable::SwapStringItems(int _indexFirst, int _indexSecond)
{
    if (_indexFirst == _indexSecond)
        return;
    const int count = GetStringItemCount();
    if (_indexFirst < 0 || _indexFirst >= count)
        return;
    if (_indexSecond < 0 || _indexSecond >= count)
        return;

    myStrings.swapItemsAt(_indexFirst, _indexSecond);
}

void SStringTable::MoveStringItems(int _indexFrom, int _indexTo)
{
    if (_indexFrom == _indexTo)
        return;
    const int count = GetStringItemCount();
    if (_indexFrom < 0 || _indexFrom >= count)
        return;
    if (_indexTo < 0 || _indexTo >= count)
        return;

    myStrings.move(_indexFrom, _indexTo);
}




bool SStringTable::SetItemIdentifier(const int _index, const QString& _identifier)
{
    if (_identifier.isEmpty())
        return false;

    const int count = GetStringItemCount();
    if (_index < 0 || _index >= count)
        return false;

    if (myStrings[_index].GetIdentifier() == _identifier)
        return true;

    bool ok = true;
    for (int i = 0; i < count; i++)
    {
        if (myStrings[i].GetIdentifier() == _identifier && i != _index)
        {
            ok = false;
            break;
        }
    }
    if (ok)
        myStrings[_index].SetIdentifier(_identifier);
    return ok;
}

void SStringTable::SetItemString(int _row, SStringHelper::SStringLanguages _col, const QString& _text)
{
    if (_col < 0 || SStringHelper::SStringLanguages::Count <= _col)
    {
        qFatal("UpdateString slot triggered with invalid language col %d", _col);
        return;
    }

    SStringItem* stringItem = GetStringItemW(_row);
    if (!stringItem)
    {
        qFatal("UpdateString slot triggered with invalid item row %d", _row);
        return;
    }

    stringItem->SetString(_col, _text);
}



void SStringTable::WriteValue_CSV(std::ofstream& _file, SStringHelper::SStringLanguages _language) const
{
    for (const auto& stringItem : myStrings)
    {
        stringItem.WriteValue_CSV(_file, _language);
        _file << '\n';
    }
}
