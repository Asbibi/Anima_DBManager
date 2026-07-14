#include "languageaction.h"

LanguageAction::LanguageAction(const Language& _languageValue) :
    myType{LanguageActionType::PRISTINE},
    myLanguageFinalValue{_languageValue}
{}


LanguageAction::LanguageAction(const LanguageAction& _other) :
    myType{_other.myType},
    myLanguageFinalValue{_other.myLanguageFinalValue}
{}

void LanguageAction::operator=(const LanguageAction& _other)
{
    myType = _other.myType;
    myLanguageFinalValue = _other.myLanguageFinalValue;
}