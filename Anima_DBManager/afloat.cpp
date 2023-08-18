#include "afloat.h"
#include "aint.h"

AFloat::AFloat(const AttributeParam& _sharedParam) :
    AFloat(_sharedParam, 0)
{}
AFloat::AFloat(const AttributeParam& _sharedParam, float _value) :
    Attribute(_sharedParam),
    value(_value)
{}


QString AFloat::GetDisplayedText() const
{
    return QString::number(GetValue());
}
QString AFloat::GetValueAsText() const
{
    return QString::number(value);
}
QString AFloat::GetAttributeAsCSV() const
{
    return QString::number(GetValue());
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
void AFloat::CopyValueFromOther(const Attribute* _other)
{
    const AFloat* other_AF = dynamic_cast<const AFloat*>(_other);
    float otherValue = 0;
    if (other_AF)
    {
        otherValue = other_AF->GetValue();
    }
    else
    {
        const AInt* other_AI = dynamic_cast<const AInt*>(_other);
        if (!other_AI)
            return;

        otherValue = other_AI->GetValue();
    }

    value = otherValue;
    if (!FitsMinParam())
        value = mySharedParam.min_f;
    else if (!FitsMaxParam())
            value = mySharedParam.max_f;
}



bool AFloat::FitsMinParam() const
{
    return mySharedParam.ignoreMin || value >= mySharedParam.min_f;
}
bool AFloat::FitsMaxParam() const
{
    return mySharedParam.ignoreMax || value <= mySharedParam.max_f;
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
