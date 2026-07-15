#ifndef LANGUAGEACTION_H
#define LANGUAGEACTION_H

#include "language.h"


enum class LanguageActionType
{
    PRISTINE,

    ADD,
    REMOVE,
    MOVE
};


struct LanguageAction
{
public:
    LanguageActionType myType;
    Language myLanguageValue;

public:
    LanguageAction(const Language& _languageValue);
    LanguageAction(const LanguageAction& _other);
    void operator=(const LanguageAction& _other);
};

#endif // LANGUAGEACTION_H
