#include "astring.h"

#include<QDebug>


AString::AString(const AttributeParam* _sharedParam) : AString(_sharedParam, "") {}
AString::AString(const AttributeParam* _sharedParam, QString _value) :
    Attribute(_sharedParam),
    value(_value)
{
    if (_sharedParam == nullptr)
        qFatal("\n\nNull param given when instancing <STRING> Attribute:\n\n\t===== Not allowed =====\n\n");
}


Attribute* AString::CreateDuplica() const
{
    return new AString(sharedParam, value);
}
QString AString::GetDisplayedText(bool complete) const
{
    return complete ? value : value.left(sharedParam->max_i);
}
void AString::WriteValue_CSV(std::ofstream& file) const
{
    file << GetDisplayedText().toStdString();
}
void AString::SetValueFromText(const QString& text)
{
    value = text;
}


bool AString::HasValidSize() const
{
    return value.size() < sharedParam->max_i;
}
QString AString::GetFullText() const
{
    return value;
}
