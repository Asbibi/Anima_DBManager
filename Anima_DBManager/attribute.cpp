#include "attribute.h"

#include "qattribute.h"
#include "templateattribute.h"


Attribute::Attribute(TemplateAttribute& _template) :
    myTemplate(_template)
{
    myTemplate.RegisterAttribute(this);
}
Attribute* Attribute::CreateDuplica() const
{
    auto* duplica = AttributeTypeHelper::NewAttributeFromType(GetType(), myTemplate);
    duplica->SetValueFromText(GetValueAsText());
    return duplica;
}
void Attribute::PreManualDelete()
{
    myTemplate.UnregisterAttribute(this);
}
Attribute::~Attribute()
{}

void Attribute::EmitValueChanged()
{
    emit OnValueChanged(this);
}

void Attribute::SetValue_CSV(const QString& text)
{
    // By default simply use the standard set from text method
    SetValueFromText(text);
}
void Attribute::WriteValue_CSV(std::ofstream& file) const
{
    file << GetValue_CSV().toStdString();
}

const TemplateAttribute* Attribute::GetTemplate() const
{
    return &myTemplate;
}
const AttributeParam& Attribute::GetTemplateParam() const
{
    return GetTemplate()->GetSharedParam();
}
