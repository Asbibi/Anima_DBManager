#include "sstringitem.h"

SStringItem::SStringItem(const QString& _identifier) :
    myIdentifier(_identifier)
{
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        myStrings[i] = "";
    }
}
SStringItem::SStringItem(const SStringItem& _another) :
    myIdentifier(_another.myIdentifier)
{
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
        myStrings[i] = _another.myStrings[i];
}
void SStringItem::operator=(const SStringItem& _another)
{
    myIdentifier = _another.myIdentifier;
    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
        myStrings[i] = _another.myStrings[i];
}
bool SStringItem::operator==(const SStringItem& _other) const
{
    return myIdentifier == _other.myIdentifier;
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

void SStringItem::SetIdentifier(const QString& _identifier)
{
    myIdentifier = _identifier;
}
void SStringItem::SetString(SStringHelper::SStringLanguages _language, const QString& _str)
{
    if (_language != SStringHelper::SStringLanguages::Count)
        myStrings[_language] = _str;
}

void SStringItem::WriteValue_CSV(std::ofstream& _file, SStringHelper::SStringLanguages _language) const
{
    QString stringCpy = myStrings[_language];
    _file << "\"" << myIdentifier.toStdString() << "\",\"" << stringCpy.replace('\n',"\\n").toStdString() << "\"";
}
