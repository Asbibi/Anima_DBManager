#include "abool.h"

ABool::ABool() :
    ABool(false)
{}
ABool::ABool(bool _value) :
    Attribute(nullptr),
    value(_value)
{}



Attribute* ABool::CreateDuplica() const
{
    return new ABool(value);
}
QString ABool::GetDisplayedText(bool) const
{
    if (value)
        return "TRUE";
    else
        return "FALSE";
}
void ABool::WriteValue_CSV(std::ofstream& file) const
{
    if (value)
        file << "True";
    else
        file << "False";
}
void ABool::SetValueFromText(const QString& text)
{
    value = QString::compare(text, "true", Qt::CaseInsensitive) == 0;
}
