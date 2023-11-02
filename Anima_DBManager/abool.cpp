#include "abool.h"

ABool::ABool(TemplateAttribute& _template) :
    ABool(_template, false)
{}
ABool::ABool(TemplateAttribute& _template, bool _value) :
    Attribute(_template),
    value(_value)
{}


QString ABool::GetDisplayedText() const
{
    return value ? "TRUE" : "FALSE";
}
QString ABool::GetValueAsText() const
{
    return value ? "true" : "false";
}
QString ABool::GetAttributeAsCSV() const
{
    return value ? "True" : "False";
}
QJsonValue ABool::GetAttributeAsJSON() const
{
    return QJsonValue(value);
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
bool ABool::ReadValue_JSON(const QJsonValue& _value)
{
    if (!_value.isBool())
    {
        return false;
    }

    value = _value.toBool();
    return true;
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
