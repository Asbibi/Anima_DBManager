#include "afloat.h"

AFloat::AFloat(const AttributeParam& _sharedParam) :
    AFloat(_sharedParam, 0)
{}
AFloat::AFloat(const AttributeParam& _sharedParam, float _value) :
    Attribute(_sharedParam),
    value(_value)
{}



Attribute* AFloat::CreateDuplica() const
{
    return new AFloat(mySharedParam, value);
}
QString AFloat::GetDisplayedText(bool complete) const
{
    return QString::number(complete ? value : GetValue());
}
void AFloat::WriteValue_CSV(std::ofstream& file) const
{
    file << GetValue();
}
void AFloat::SetValueFromText(const QString& text)
{
    bool ok;
    float _value = text.toFloat(&ok);
    if (!ok)
    {
       EmitValueChanged();
       return;
    }

    bool changed = value != _value;
    value = _value;
    if (changed)
    {
        EmitValueChanged();
    }
}


bool AFloat::FitsMinParam() const
{
    return mySharedParam.ignoreMin || value < mySharedParam.min_f;
}
bool AFloat::FitsMaxParam() const
{
    return mySharedParam.ignoreMax || value > mySharedParam.max_f;
}
float AFloat::GetValue(bool _validated) const
{
    if (!_validated)
        return value;

    float v = value;
    if (!FitsMinParam())
        v = mySharedParam.min_f;
    else if (!FitsMaxParam())
        v = mySharedParam.max_f;
    return v;
}

float AFloat::GetMax(bool& useIt) const
{
    useIt = !mySharedParam.ignoreMax;
    return mySharedParam.max_f;
}
float AFloat::GetMin(bool& useIt) const
{
    useIt = !mySharedParam.ignoreMin;
    return mySharedParam.min_f;
}
