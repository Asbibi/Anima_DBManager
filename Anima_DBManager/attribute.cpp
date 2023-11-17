#include "attribute.h"

#include "jsonhelper.h"
#include "qattribute.h"
#include "templateattribute.h"

#include <QJsonDocument>


Attribute::Attribute(TemplateAttribute& _template) :
    myTemplate(_template)
{
    myTemplate.RegisterAttribute(this);
}
Attribute* Attribute::CreateDuplica() const
{
    auto* duplica = AttributeTypeHelper::NewAttributeFromType(GetType(), myTemplate);
    duplica->SetValue_JSON(GetValue_JSON());
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
QString Attribute::GetValue_String() const
{
    return JsonHelper::JsonToString(GetValue_JSON());
}



