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
QString AShortString::GetValueAsText() const
{
    return value;
}
QString AShortString::GetAttributeAsCSV() const
{
    return GetDisplayedText();
}
QJsonValue AShortString::GetAttributeAsJSON() const
{
    return QJsonValue(value);
}
void AShortString::SetValueFromText(const QString& text)
{
    bool changed = value != text;
    value = text;

    if (changed)
    {
        EmitValueChanged();
    }
}
void AShortString::CopyValueFromOther(const Attribute* _other)
{
    const AShortString* other_ASS = dynamic_cast<const AShortString*>(_other);
    if (!other_ASS)
        return;

    value = other_ASS->value;
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
