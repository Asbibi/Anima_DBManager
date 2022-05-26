#include "aint.h"


AInt::AInt(const AttributeParam* _sharedParam) : AInt(_sharedParam, 0) {}
AInt::AInt(const AttributeParam* _sharedParam, int _value) :
    Attribute(_sharedParam),
    value(_value)
{
    if (_sharedParam == nullptr)
        qFatal("\n\nNull param given when instancing <INT> Attribute:\n\n\t===== Not allowed =====\n\n");
}



Attribute* AInt::CreateDuplica() const
{
    return new AInt(sharedParam, value);
}
QString AInt::GetDisplayedText(bool complete) const
{
    return QString::number(complete ? value : GetValidValue());
}
void AInt::WriteValue_CSV(std::ofstream& file) const
{
    file << GetValidValue();
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
    return sharedParam->ignoreMin || value < sharedParam->min_i;
}
bool AInt::FitsMaxParam() const
{
    return sharedParam->ignoreMax || value > sharedParam->max_i;
}
int AInt::GetValidValue() const
{
    int v = value;
    if (!FitsMinParam())
        v = sharedParam->min_i;
    else if (!FitsMaxParam())
        v = sharedParam->max_i;
    return v;
}
