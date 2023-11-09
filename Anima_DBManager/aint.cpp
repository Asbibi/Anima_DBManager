#include "aint.h"
#include "afloat.h"
#include "templateattribute.h"


AInt::AInt(TemplateAttribute& _template) :
    AInt(_template, 0)
{}
AInt::AInt(TemplateAttribute& _template, int _value) :
    Attribute(_template),
    value(_value)
{}


QString AInt::GetDisplayedText() const
{
    return QString::number(GetValue());
}
QString AInt::GetValueAsText() const
{
    return QString::number(value);
}
QString AInt::GetValue_CSV() const
{
    return QString::number(GetValue());
}
QJsonValue AInt::GetValue_JSON() const
{
    return QJsonValue(value);
}
void AInt::SetValueFromText(const QString& text)
{
    bool ok;
    int _value = 0;
    if (text.contains('.'))
        _value = text.toFloat(&ok);
    else
        _value = text.toInt(&ok);

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
void AInt::CopyValueFromOther(const Attribute* _other)
{
    const AInt* other_AI = dynamic_cast<const AInt*>(_other);
    int otherValue = 0;
    if (other_AI)
    {
        otherValue = other_AI->GetValue();
    }
    else
    {
        const AFloat* other_AF = dynamic_cast<const AFloat*>(_other);
        if (!other_AF)
            return;

        otherValue = other_AF->GetValue();
    }

    value = otherValue;
    if (!FitsMinParam())
        value = MY_SHARED_PARAM.min_i;
    else if (!FitsMaxParam())
            value = MY_SHARED_PARAM.max_i;
}
bool AInt::SetValue_JSON(const QJsonValue& _value)
{
    if (!_value.isDouble()) // Type Double is used for int in QJsonValue
    {
        return false;
    }

    value = _value.toInt();
    return true;
}


bool AInt::FitsMinParam() const
{
    return MY_SHARED_PARAM.ignoreMin || value >= MY_SHARED_PARAM.min_i;
}
bool AInt::FitsMaxParam() const
{
    return MY_SHARED_PARAM.ignoreMax || value <= MY_SHARED_PARAM.max_i;
}
int AInt::GetValue(bool _validated) const
{
    if (!_validated)
        return value;

    int v = value;
    if (!FitsMinParam())
        v = MY_SHARED_PARAM.min_i;
    else if (!FitsMaxParam())
        v = MY_SHARED_PARAM.max_i;
    return v;
}

int AInt::GetMax(bool& useIt) const
{
    useIt = !MY_SHARED_PARAM.ignoreMax;
    return MY_SHARED_PARAM.max_i;
}
int AInt::GetMin(bool& useIt) const
{
    useIt = !MY_SHARED_PARAM.ignoreMin;
    return MY_SHARED_PARAM.min_i;
}
