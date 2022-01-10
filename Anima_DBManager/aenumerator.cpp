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
    int v = enumerator->GetValueIndex(text);
    if (v < 0 || v >= enumerator->GetValueCount())
        v = 0;
    value_index = v;
}
