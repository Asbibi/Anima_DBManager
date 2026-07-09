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
};

#endif // LANGUAGEENUM_H
