#include "languageaction.h"

LanguageAction::LanguageAction(const Language& _languageValue) :
    myType{LanguageActionType::PRISTINE},
    myLanguageValue{_languageValue}
{}


LanguageAction::LanguageAction(const LanguageAction& _other) :
    myType{_other.myType},
    myLanguageValue{_other.myLanguageValue}
{}

void LanguageAction::operator=(const LanguageAction& _other)
{
    myType = _other.myType;
    myLanguageValue = _other.myLanguageValue;
}