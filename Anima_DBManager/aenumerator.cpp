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
QString AEnumerator::GetDisplayedText() const
{
    return enumerator->GetValue(value_index);
}
void AEnumerator::WriteValue_CSV(std::ofstream& file) const
{
    file << GetDisplayedText().toStdString();
}
