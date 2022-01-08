#include "afloat.h"

AFloat::AFloat(const AttributeParam* _sharedParam) : AFloat(_sharedParam, 0) {}
AFloat::AFloat(const AttributeParam* _sharedParam, float _value) :
    Attribute(_sharedParam),
    value(_value)
{
    if (_sharedParam == nullptr)
        qFatal("\n\nNull param given when instancing <FLOAT> Attribute:\n\n\t===== Not allowed =====\n\n");
}



Attribute* AFloat::CreateDuplica() const
{
    return new AFloat(sharedParam, value);
}
QString AFloat::GetDisplayedText() const
{
    return QString::number(GetValidValue());
}
void AFloat::WriteValue_CSV(std::ofstream& file) const
{
    file << GetValidValue();
}


bool AFloat::FitsMinParam() const
{
    return sharedParam->ignoreMin || value < sharedParam->min_f;
}
bool AFloat::FitsMaxParam() const
{
    return sharedParam->ignoreMax || value > sharedParam->max_f;
}
float AFloat::GetValidValue() const
{
    int v = value;
    if (!FitsMinParam())
        v = sharedParam->min_f;
    else if (!FitsMaxParam())
        v = sharedParam->max_f;
    return v;
}
