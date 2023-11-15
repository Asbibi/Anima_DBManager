#include "ashortstring.h"
#include "templateattribute.h"



AShortString::AShortString(TemplateAttribute& _template) :
    AShortString(_template, "")
{}
AShortString::AShortString(TemplateAttribute& _template, QString _value) :
    Attribute(_template),
    value(_value)
{}


QString AShortString::GetDisplayedText() const
{
    if (MY_SHARED_PARAM.ignoreMax)
        return value;
    else
        return value.left(MY_SHARED_PARAM.max_i);
}
QString AShortString::GetValue_CSV() const
{
    return GetDisplayedText();
}
QJsonValue AShortString::GetValue_JSON() const
{
    return QJsonValue(value);
}
void AShortString::CopyValueFromOther(const Attribute* _other)
{
    const AShortString* other_ASS = dynamic_cast<const AShortString*>(_other);
    if (!other_ASS)
        return;

    value = other_ASS->value;
}
bool AShortString::SetValue_JSON(const QJsonValue& _value)
{
    if (!_value.isString())
    {
        return false;
    }

    value = _value.toString();
    return true;
}
void AShortString::SetValue_CSV(const QString& _text)
{
    bool changed = value != _text;
    value = _text;

    if (changed)
    {
        EmitValueChanged();
    }
}


bool AShortString::HasValidSize() const
{
    if (MY_SHARED_PARAM.ignoreMax)
        return true;
    else
        return value.size() < MY_SHARED_PARAM.max_i;
}
QString AShortString::GetFullText() const
{
    return value;
}
