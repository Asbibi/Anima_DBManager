#include "abool.h"

ABool::ABool(const AttributeParam& _sharedParam) :
    ABool(_sharedParam, false)
{}
ABool::ABool(const AttributeParam& _sharedParam, bool _value) :
    Attribute(_sharedParam),
    value(_value)
{}



Attribute* ABool::CreateDuplica() const
{
    return new ABool(mySharedParam, value);
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
    SetValue(QString::compare(text, "true", Qt::CaseInsensitive) == 0);
}

void ABool::SetValue(bool _value)
{
    bool changed = _value != value;
    value = _value;

    if (changed)
    {
        EmitValueChanged();
    }
}
