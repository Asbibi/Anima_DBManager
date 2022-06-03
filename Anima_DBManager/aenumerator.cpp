#include "aenumerator.h"


AEnumerator::AEnumerator(const AttributeParam* _sharedParam, int _valueIndex) :
    Attribute(_sharedParam),
    value_index(_valueIndex)
{
    if (!_sharedParam || !CheckEnumIsAlive())
    {
        qFatal("Trying to initialize <ENUM> attribute with a null enumerator or shared param");
    }
}


bool AEnumerator::CheckEnumIsAlive() const
{
    return (sharedParam != nullptr) && (sharedParam->enumerator != nullptr);
}



Attribute* AEnumerator::CreateDuplica() const
{
    return new AEnumerator(sharedParam, value_index);
}
QString AEnumerator::GetDisplayedText(bool) const
{
    return CheckEnumIsAlive() ? sharedParam->enumerator->GetValue(value_index) : "<font color=\"darkred\">!!! NULL ENUM !!!</font>";
}
void AEnumerator::WriteValue_CSV(std::ofstream& file) const
{
    if (CheckEnumIsAlive())
        file << GetDisplayedText().toStdString();
}
void AEnumerator::SetValueFromText(const QString& text)
{
    if (!CheckEnumIsAlive())
    {
        SetEnumValue(-1);
    }

    int indexValue = sharedParam->enumerator->GetValueIndex(text);
    if (indexValue < 0 || indexValue >= sharedParam->enumerator->GetValueCount())
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
