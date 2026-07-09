#include "languageenum.h"

LanguageEnum::LanguageEnum() : QObject{nullptr}
{
    languages.push_back(Language{"EN", "English"});
    // vvv debug vvv
    languages.push_back(Language{"FR", "Fran"});
    languages.push_back(Language{"NO", "Norg"});
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