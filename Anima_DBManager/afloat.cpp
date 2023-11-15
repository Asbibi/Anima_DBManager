#include "afloat.h"
#include "aint.h"
#include "templateattribute.h"

AFloat::AFloat(TemplateAttribute& _template) :
    AFloat(_template, 0)
{}
AFloat::AFloat(TemplateAttribute& _template, float _value) :
    Attribute(_template),
    value(_value)
{}


QString AFloat::GetDisplayedText() const
{
    return QString::number(GetValue());
}
QString AFloat::GetValue_CSV() const
{
    return QString::number(GetValue());
}
QJsonValue AFloat::GetValue_JSON() const
{
    return QJsonValue(value);
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
        value = MY_SHARED_PARAM.min_f;
    else if (!FitsMaxParam())
            value = MY_SHARED_PARAM.max_f;
}
bool AFloat::SetValue_JSON(const QJsonValue& _value)
{
    if (!_value.isDouble())
    {
        return false;
    }

    value = _value.toDouble();
    return true;
}
void AFloat::SetValue_CSV(const QString& _text)
{
    bool ok;
    float _value = _text.toFloat(&ok);
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
    return MY_SHARED_PARAM.ignoreMin || value >= MY_SHARED_PARAM.min_f;
}
bool AFloat::FitsMaxParam() const
{
    return MY_SHARED_PARAM.ignoreMax || value <= MY_SHARED_PARAM.max_f;
}
float AFloat::GetValue(bool _validated) const
{
    if (!_validated)
        return value;

    float v = value;
    if (!FitsMinParam())
        v = MY_SHARED_PARAM.min_f;
    else if (!FitsMaxParam())
        v = MY_SHARED_PARAM.max_f;
    return v;
}

float AFloat::GetMax(bool& useIt) const
{
    useIt = !MY_SHARED_PARAM.ignoreMax;
    return MY_SHARED_PARAM.max_f;
}
float AFloat::GetMin(bool& useIt) const
{
    useIt = !MY_SHARED_PARAM.ignoreMin;
    return MY_SHARED_PARAM.min_f;
}
