#include "sstringitem.h"

#include "db_manager.h"


SStringItem::SStringItem(const QString& _identifier) :
    myIdentifier(_identifier)
{
    const int languagesCount = DB_Manager::GetLanguagesCount();
    myStrings.reserve(languagesCount);
    for (int i = 0; i < languagesCount; i++)
    {
        myStrings.push_back("");
    }
}
SStringItem::SStringItem(const SStringItem& _another) :
    myIdentifier(_another.myIdentifier)
{
    const int languagesCount = DB_Manager::GetLanguagesCount();
    myStrings.reserve(languagesCount);
    for (int i = 0; i < languagesCount; i++)
    {
        myStrings.push_back(_another.myStrings[i]);
    }
}
void SStringItem::operator=(const SStringItem& _another)
{
    const int languagesCount = DB_Manager::GetLanguagesCount();
    myIdentifier = _another.myIdentifier;
    Q_ASSERT(myStrings.size() == languagesCount);
    Q_ASSERT(_another.myStrings.size() == languagesCount);
    for (int i = 0; i < languagesCount; i++)
        myStrings[i] = _another.myStrings[i];
}
bool SStringItem::operator==(const SStringItem& _other) const
{
    return myIdentifier == _other.myIdentifier;
}
bool SStringItem::operator<(const SStringItem& _other) const
{
    return myIdentifier < _other.myIdentifier;
}



const QString& SStringItem::GetIdentifier() const
{
    return myIdentifier;
}
const QString& SStringItem::GetString(int _languageIndex) const
{
    if (_languageIndex == DB_Manager::GetLanguagesCount())
        return myIdentifier;

    return myStrings[_languageIndex];
}

void SStringItem::SetIdentifier(const QString& _identifier)
{
    myIdentifier = _identifier;
}
void SStringItem::SetString(int _languageIndex, const QString& _str)
{
    if (_languageIndex < 0 || _languageIndex >= DB_Manager::GetLanguagesCount())
        return;

    myStrings[_languageIndex] = _str;
}

void SStringItem::WriteValue_CSV(std::ofstream& _file, int _languageIndex, bool _withDictionaryReplacement) const
{
    QString stringCpy = myStrings[_languageIndex];
    if (_withDictionaryReplacement)
    {
        SStringHelper::ReplaceDictionaryReferenceInString(stringCpy, _languageIndex);
    }

    _file << "\"" << myIdentifier.toStdString() << "\",\"" << stringCpy.replace('\n',"\\n").toStdString() << "\"";
}
