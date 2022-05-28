#include "sstringitem.h"

SStringItem::SStringItem(const QString& _identifier) :
    myIdentifier(_identifier)
{
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        myStrings[i] = "";
    }
}


const QString& SStringItem::GetIdentifier() const
{
    return myIdentifier;
}
const QString& SStringItem::GetString(SStringHelper::SStringLanguages _language) const
{
    if (_language == SStringHelper::SStringLanguages::Count)
        return myIdentifier;

    return myStrings[_language];
}
void SStringItem::SetString(SStringHelper::SStringLanguages _language, const QString& _str)
{
    if (_language != SStringHelper::SStringLanguages::Count)
        myStrings[_language] = _str;
}

bool SStringItem::operator=(const SStringItem& _other) const
{
    return myIdentifier == _other.myIdentifier;
}
