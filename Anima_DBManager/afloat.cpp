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
    return sharedParam->ignoreMin || value < sharedParam->min_f;
}
bool AFloat::FitsMaxParam() const
{
    return sharedParam->ignoreMax || value > sharedParam->max_f;
}
float AFloat::GetValue(bool _validated) const
{
    if (!_validated)
        return value;

    float v = value;
    if (!FitsMinParam())
        v = sharedParam->min_f;
    else if (!FitsMaxParam())
        v = sharedParam->max_f;
    return v;
}

float AFloat::GetMax(bool& useIt) const
{
    useIt = !sharedParam->ignoreMax;
    return sharedParam->max_f;
}
float AFloat::GetMin(bool& useIt) const
{
    useIt = !sharedParam->ignoreMin;
    return sharedParam->min_f;
}
