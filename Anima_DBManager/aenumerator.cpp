#include "aenumerator.h"

AEnumerator::AEnumerator(const Enumerator* _enumerator, int _valueIndex) :
    Attribute(nullptr),
    enumerator(_enumerator),
    value_index(_valueIndex)
{}


Attribute* AEnumerator::CreateDuplica() const
{
    return new AEnumerator(enumerator, value_index);
}
QString AEnumerator::GetDisplayedText(bool) const
{
    return enumerator->GetValue(value_index);
}
void AEnumerator::WriteValue_CSV(std::ofstream& file) const
{
    file << GetDisplayedText().toStdString();
}
void AEnumerator::SetValueFromText(const QString& text)
{
    int indexValue = enumerator->GetValueIndex(text);
    if (indexValue < 0 || indexValue >= enumerator->GetValueCount())
        indexValue = 0;
    value_index = indexValue;
}
