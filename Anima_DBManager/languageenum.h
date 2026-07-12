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

    const QList<Language>& GetLanguageList() const;
    int GetLanguageCount() const;
    const Language& GetLanguage(int _index) const;
    int GetLanguageIndexFromAbbrev(const QString& _languageAbbrev) const;
    bool HasDoubles() const;

    bool AddLanguage(const Language& _language, int _index);
    void RemoveLanguage(int _languageIndex);
    void MoveLanguage(int _fromIndex, int _targetIndex);
    void ReplaceLanguage(int _index, const Language& _language);
    void Reset();
};

#endif // LANGUAGEENUM_H
