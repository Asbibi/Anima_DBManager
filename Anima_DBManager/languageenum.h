#ifndef LANGUAGEENUM_H
#define LANGUAGEENUM_H

#include <QObject>
#include <language.h>

class LanguageEnum : public QObject
{
protected:
    QList<Language> languages;

public:
    LanguageEnum();

    int GetLanguageCount() const;
    const Language& GetLanguage(int _index) const;
    int GetLanguageIndexFromAbbrev(const QString& _languageAbbrev) const;
    bool HasDoubles() const;

    bool AddLanguage(const Language& _language);
    void RemoveLanguage(int _languageIndex);
    void Reset();
};

#endif // LANGUAGEENUM_H
