#include "ashortstring.h"

#include<QDebug>


AShortString::AShortString(const AttributeParam& _sharedParam) :
    AShortString(_sharedParam, "")
{}
AShortString::AShortString(const AttributeParam& _sharedParam, QString _value) :
    Attribute(_sharedParam),
    value(_value)
{}


Attribute* AShortString::CreateDuplica() const
{
    return new AShortString(mySharedParam, value);
}
QString AShortString::GetDisplayedText() const
{
    if (mySharedParam.ignoreMax)
        return value;
    else
        return value.left(mySharedParam.max_i);
}
QString AShortString::GetValueAsText() const
{
    return value;
}
QString AShortString::GetAttributeAsCSV() const
{
    return GetDisplayedText();
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
    if (mySharedParam.ignoreMax)
        return true;
    else
        return value.size() < mySharedParam.max_i;
}
QString AShortString::GetFullText() const
{
    return value;
}
