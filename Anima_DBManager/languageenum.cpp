#include "languageenum.h"
#include "savemanager.h"
#include <QDebug>

LanguageEnum::LanguageEnum() : QObject{nullptr}
{
    Reset();
}

const QList<Language>& LanguageEnum::GetLanguageList() const
{
    return languages;
}

int LanguageEnum::GetLanguageCount() const
{
    return (int)languages.size();
}

const Language& LanguageEnum::GetLanguage(int _index) const
{
    if (_index < 0 || _index >= GetLanguageCount())
    {
        Q_ASSERT(languages.size() > 0);
        return languages[0];
    }

    return languages[_index];
}
int LanguageEnum::GetLanguageIndexFromAbbrev(const QString& _languageAbbrev) const
{
    const int count = GetLanguageCount();
    for (int i = 0; i < count; i++)
    {
        if (languages[i].GetAbbrev() == _languageAbbrev)
        {
            return i;
        }
    }

    return -1;
}


bool LanguageEnum::AddLanguage(const Language& _language, int _index)
{
    // If currently opening a save file, we tolerate doubles temporary because of the default english language
    if (!SaveManager::IsOpeningFile())
    {
        if (GetLanguageIndexFromAbbrev(_language.GetAbbrev()) != -1)
        {
            qDebug() << "Language with code " << _language.GetAbbrev() << " already exists, can't add";
            return false;
        }
    }

    if (_index < 0 || _index >= languages.size())
    {
        languages.push_back(_language);
    }
    else
    {
        languages.insert(_index, _language);
    }
    return true;
}
void LanguageEnum::RemoveLanguage(int _languageIndex)
{
    if (_languageIndex < 0 || _languageIndex >= languages.size())
        return;

    languages.removeAt(_languageIndex);
}
void LanguageEnum::MoveLanguage(int _fromIndex, int _targetIndex)
{
    if (_fromIndex == _targetIndex ||
        (_fromIndex < 0 || _fromIndex >= languages.size())||
        (_targetIndex < 0 || _targetIndex >= languages.size()))
    {
        return;
    }

    languages.move(_fromIndex, _targetIndex);
}
void LanguageEnum::ReplaceLanguage(int _index, const Language& _language)
{
    if (_index < 0 || _index >= languages.size())
        return;
    languages[_index] = _language;
}
bool LanguageEnum::HasDoubles() const
{
    const int languageCount = GetLanguageCount();
    for (int i = 0; i < languageCount; i++)
    {
        if (i != GetLanguageIndexFromAbbrev(languages[i].GetAbbrev()))
            return true;
    }

    return false;
}
void LanguageEnum::Reset()
{
    languages.clear();
    languages.push_back(Language{});
}