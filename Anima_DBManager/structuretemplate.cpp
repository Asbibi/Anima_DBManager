#include "structuretemplate.h"

StructureTemplate::StructureTemplate(QString _structName, QColor _structColor) :
    structName(_structName),
    structColor(_structColor)
{}
StructureTemplate::StructureTemplate(const StructureTemplate& other) :
    structName(other.structName),
    structColor(other.structColor)
{
    for(const auto& str : other.attributesNames)
        attributesNames.push_back(str);

    for(const auto& attr : other.attributeTemplates)
        attributeTemplates.push_back(attr->CreateDuplica());
}
StructureTemplate::~StructureTemplate()
{
    for(auto const& att : attributeTemplates)
        delete(att);
}



int StructureTemplate::GetAttributeIndex(const QString& att_Name) const
{
    int index = -1;
    for (int i = 0; i < attributesNames.size(); i++)
    {
        if(att_Name == attributesNames[i])
        {
            index = i;
            break;
        }
    }
    return index;
}
void StructureTemplate::AddAttribute(QString att_Name, Attribute* att)
{
    if (att == nullptr || GetAttributeIndex(att_Name) != -1)    // Attribute already exists
        return;

    attributesNames.push_back(att_Name);
    attributeTemplates.push_back(att);
}
void StructureTemplate::RemoveAttribute(const int& att_Index)
{
    if (att_Index < 0 || att_Index >= attributeTemplates.size())
        return;

    attributesNames.erase(attributesNames.begin() + att_Index);
    attributeTemplates.erase(attributeTemplates.begin() + att_Index);
}
void StructureTemplate::RemoveAttribute(const QString& att_Name)
{
    RemoveAttribute(GetAttributeIndex(att_Name));
}



Attribute::Type StructureTemplate::GetAttributeType(int _attIndex) const
{
    const auto* attr = GetAttribute(_attIndex);
    return attr ? attr->GetType() : Attribute::Type::Invalid;
}
const QString& StructureTemplate::GetAttributeName(int _attIndex) const
{
    return attributesNames[_attIndex];
}
const Attribute* StructureTemplate::GetAttribute(int _attIndex) const
{
    if (_attIndex < 0 || _attIndex >= attributeTemplates.size())
        return nullptr;

    return attributeTemplates[_attIndex];
}
const Attribute* StructureTemplate::GetAttribute(const QString& att_Name) const
{
    return GetAttribute(GetAttributeIndex(att_Name));
}
const std::vector<const Attribute*>& StructureTemplate::GetAttributes() const
{
    return attributeTemplates;
}
const QString& StructureTemplate::GetStructName() const
{
    return structName;
}
