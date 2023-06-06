#include "attribute.h"
#include "qattribute.h"


Attribute::Attribute(const AttributeParam& _sharedParam) :
    mySharedParam(_sharedParam)
{}

Attribute::~Attribute()
{}

void Attribute::EmitValueChanged()
{
    emit OnValueChanged(this);
}

void Attribute::ReadValue_CSV(const QString& text)
{
    // By default simply use the standard set from text method
    SetValueFromText(text);
}
void Attribute::WriteValue_CSV(std::ofstream& file) const
{
    file << GetAttributeAsCSV().toStdString();
}
