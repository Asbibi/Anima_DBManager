#include "aenumerator.h"

#include "templateattribute.h"


AEnumerator::AEnumerator(TemplateAttribute& _template, int _valueIndex) :
    Attribute(_template),
    value_index(_valueIndex)
{}


bool AEnumerator::CheckEnumIsValid() const
{
    return MY_SHARED_PARAM.GetEnum() != nullptr;
}


QString AEnumerator::GetDisplayedText() const
{
    return CheckEnumIsValid() ? MY_SHARED_PARAM.GetEnum()->GetValue(value_index) : "<font color=\"darkred\">!!! NULL ENUM !!!</font>";
}
QString AEnumerator::GetValueAsText() const
{
    return CheckEnumIsValid() ? MY_SHARED_PARAM.GetEnum()->GetValue(value_index) : "Ø";
}
QString AEnumerator::GetValue_CSV() const
{
    return CheckEnumIsValid() ? GetDisplayedText() : "";
}
QJsonValue AEnumerator::GetValue_JSON() const
{
    return QJsonValue(GetValue_CSV());
}
void AEnumerator::SetValueFromText(const QString& text)
{
    if (!CheckEnumIsValid())
    {
        SetEnumValue(-1);
        return;
    }

    int indexValue = MY_SHARED_PARAM.GetEnum()->GetValueIndex(text);
    if (indexValue < 0 || indexValue >= MY_SHARED_PARAM.GetEnum()->GetValueCount())
        indexValue = 0;
    SetEnumValue(indexValue);
}
void AEnumerator::CopyValueFromOther(const Attribute* _other)
{
    const AEnumerator* other_AE = dynamic_cast<const AEnumerator*>(_other);
    if (!other_AE || MY_SHARED_PARAM.enumeratorIndex != other_AE->MY_SHARED_PARAM.enumeratorIndex)
        return;

    value_index = other_AE->value_index;
}
bool AEnumerator::SetValue_JSON(const QJsonValue& _value)
{
    if (!_value.isString())
    {
        return false;
    }

    SetValueFromText(_value.toString());
    return true;
}



const Enumerator* AEnumerator::GetEnum() const
{
    return MY_SHARED_PARAM.GetEnum();
}
int AEnumerator::GetEnumValue() const
{
    return value_index;
}
void AEnumerator::SetEnumValue(int _valueIndex)
{
    bool changed = value_index != _valueIndex;
    value_index = _valueIndex;

    if (changed)
    {
        EmitValueChanged();
    }
}
