#include "language.h"

Language::Language() : Language{"EN", "English"}
{}
Language::Language(const QString& _abbrev, const QString& _name) :
    QObject{nullptr},
    name{_name},
    abbrev{_abbrev}
{}
Language::Language(const Language& _other) :
    QObject{nullptr},
    name{_other.name},
    abbrev{_other.abbrev}
{}
void Language::operator=(const Language& _other)
{
    name = _other.name;
    abbrev = _other.abbrev;
}

const QString& Language::GetName() const
{
    return name;
}
const QString& Language::GetAbbrev() const
{
    return abbrev;
}