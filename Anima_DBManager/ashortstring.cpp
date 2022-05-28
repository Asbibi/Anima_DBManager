#include "ashortstring.h"

#include<QDebug>


AShortString::AShortString(const AttributeParam* _sharedParam) : AShortString(_sharedParam, "") {}
AShortString::AShortString(const AttributeParam* _sharedParam, QString _value) :
    Attribute(_sharedParam),
    value(_value)
{
    if (_sharedParam == nullptr)
        qFatal("\n\nNull param given when instancing <STRING> Attribute:\n\n\t===== Not allowed =====\n\n");
}


Attribute* AShortString::CreateDuplica() const
{
    return new AShortString(sharedParam, value);
}
QString AShortString::GetDisplayedText(bool complete) const
{
    if (sharedParam->max_i < 0)
        return value;
    else
        return complete ? value : value.left(sharedParam->max_i);
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


bool AShortString::HasValidSize() const
{
    if (sharedParam->max_i < 0)
        return true;
    else
        return value.size() < sharedParam->max_i;
}
QString AShortString::GetFullText() const
{
    return value;
}
