#include "aenumerator.h"


AEnumerator::AEnumerator(const AttributeParam& _sharedParam, int _valueIndex) :
    Attribute(_sharedParam),
    value_index(_valueIndex)
{}


bool AEnumerator::CheckEnumIsValid() const
{
    return mySharedParam.GetEnum() != nullptr;
}



Attribute* AEnumerator::CreateDuplica() const
{
    return new AEnumerator(mySharedParam, value_index);
}
QString AEnumerator::GetDisplayedText(bool) const
{
    return CheckEnumIsValid() ? mySharedParam.GetEnum()->GetValue(value_index) : "<font color=\"darkred\">!!! NULL ENUM !!!</font>";
}
void AEnumerator::WriteValue_CSV(std::ofstream& file) const
{
    if (CheckEnumIsValid())
        file << GetDisplayedText().toStdString();
}
void AEnumerator::SetValueFromText(const QString& text)
{
    if (!CheckEnumIsValid())
    {
        SetEnumValue(-1);
        return;
    }

    int indexValue = mySharedParam.GetEnum()->GetValueIndex(text);
    if (indexValue < 0 || indexValue >= mySharedParam.GetEnum()->GetValueCount())
        indexValue = 0;
    SetEnumValue(indexValue);
}
void AEnumerator::CopyValueFromOther(const Attribute* _other)
{
    const AEnumerator* other_AE = dynamic_cast<const AEnumerator*>(_other);
    if (!other_AE || mySharedParam.enumeratorIndex != other_AE->mySharedParam.enumeratorIndex)
        return;

    value_index = other_AE->value_index;
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
