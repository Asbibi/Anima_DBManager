#include "aint.h"


AInt::AInt(const AttributeParam& _sharedParam) :
    AInt(_sharedParam, 0)
{}
AInt::AInt(const AttributeParam& _sharedParam, int _value) :
    Attribute(_sharedParam),
    value(_value)
{}



Attribute* AInt::CreateDuplica() const
{
    return new AInt(mySharedParam, value);
}
QString AInt::GetDisplayedText(bool complete) const
{
    return QString::number(complete ? value : GetValue());
}
void AInt::WriteValue_CSV(std::ofstream& file) const
{
    file << GetValue();
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

bool AInt::FitsMinParam() const
{
    return mySharedParam.ignoreMin || value < mySharedParam.min_i;
}
bool AInt::FitsMaxParam() const
{
    return mySharedParam.ignoreMax || value > mySharedParam.max_i;
}
int AInt::GetValue(bool _validated) const
{
    if (!_validated)
        return value;

    int v = value;
    if (!FitsMinParam())
        v = mySharedParam.min_i;
    else if (!FitsMaxParam())
        v = mySharedParam.max_i;
    return v;
}

int AInt::GetMax(bool& useIt) const
{
    useIt = !mySharedParam.ignoreMax;
    return mySharedParam.max_i;
}
int AInt::GetMin(bool& useIt) const
{
    useIt = !mySharedParam.ignoreMin;
    return mySharedParam.min_i;
}
