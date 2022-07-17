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
QString AShortString::GetDisplayedText(bool complete) const
{
    if (mySharedParam.ignoreMax)
        return value;
    else
        return complete ? value : value.left(mySharedParam.max_i);
}
void AShortString::WriteValue_CSV(std::ofstream& file) const
{
    file << GetDisplayedText().toStdString();
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
