#include "aenumerator.h"


AEnumerator::AEnumerator(const AttributeParam* _sharedParam, int _valueIndex) :
    Attribute(_sharedParam),
    value_index(_valueIndex)
{
    if (!_sharedParam)
    {
        qFatal("Trying to initialize <ENUM> attribute with a null shared param");
    }
}


bool AEnumerator::CheckEnumIsValid() const
{
    return (sharedParam != nullptr) && (sharedParam->GetEnum() != nullptr);
}



Attribute* AEnumerator::CreateDuplica() const
{
    return new AEnumerator(sharedParam, value_index);
}
QString AEnumerator::GetDisplayedText(bool) const
{
    return CheckEnumIsValid() ? sharedParam->GetEnum()->GetValue(value_index) : "<font color=\"darkred\">!!! NULL ENUM !!!</font>";
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

    int indexValue = sharedParam->GetEnum()->GetValueIndex(text);
    if (indexValue < 0 || indexValue >= sharedParam->GetEnum()->GetValueCount())
        indexValue = 0;
    SetEnumValue(indexValue);
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
