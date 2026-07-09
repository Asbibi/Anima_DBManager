#include "language.h"

Language::Language(const QString& _abbrev, const QString& _name) :
    QObject{parent},
    name{_name},
    abbrev{_abbrev}
{}

const QString& Language::GetName() const
{
    return name;
}
const QString& Language::GetAbbrev() const
{
    return abbrev;
}