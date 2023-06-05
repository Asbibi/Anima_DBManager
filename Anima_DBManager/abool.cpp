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
    return value ? "TRUE" : "FALSE";
}
QString ABool::GetAttributeAsCSV() const
{
    return value ? "True" : "False";
}
void ABool::SetValueFromText(const QString& text)
{
    SetValue(QString::compare(text, "true", Qt::CaseInsensitive) == 0);
}
void ABool::CopyValueFromOther(const Attribute* _other)
{
    const ABool* other_AB = dynamic_cast<const ABool*>(_other);
    if (!other_AB)
        return;

    value = other_AB->value;
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
