#include "templatestructure.h"

#include <QDebug>

TemplateStructure::TemplateStructure(QString _structName, QColor _structColor) :
    myStructColor(_structColor),
    myStructName(_structName)
{}
TemplateStructure::TemplateStructure(const TemplateStructure& _other) :    
    myStructColor(_other.myStructColor),
    myStructName(_other.myStructName)
{
    for (const auto& otherTemplAttr : _other.myAttributeTemplates)
        myAttributeTemplates.append(otherTemplAttr);
}



void TemplateStructure::AddAttributeTemplate(const Attribute::Type _type, const QString& att_Name, const AttributeParam& _attParam, int _index)
{
    TemplateAttribute templAttr = TemplateAttribute(att_Name, _type, _attParam);
    AddAttributeTemplate(templAttr, nullptr, _index);
}
void TemplateStructure::AddAttributeTemplate(const TemplateAttribute& _attTemplate, const QString* _newName, int _index)
{
    const int count = (int)(myAttributeTemplates.count());
    if (_index < 0 || _index > count)
        _index = count;

    myAttributeTemplates.insert(_index, _attTemplate);

    if (_newName)
        myAttributeTemplates[_index].myAttrName = *_newName;
}


void TemplateStructure::RemoveAttribute(const int& _index)
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return;

    myAttributeTemplates[_index].DeleteData();
    myAttributeTemplates.removeAt(_index);
}
void TemplateStructure::RemoveAttribute(const QString& _attName)
{
    RemoveAttribute(GetAttributeIndex(_attName));
}


void TemplateStructure::RenameAttributeTemplate(int _index, const QString& _newName)
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return;

    myAttributeTemplates[_index].myAttrName = _newName;
}
void TemplateStructure::SetAttributeDefaultValue(int _index, const QString& _value)
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return;

    myAttributeTemplates[_index].SetDefaultValue(_value);
}
void TemplateStructure::SetAttributeDefaultValue(const QString& _attName, const QString& _value)
{
    SetAttributeDefaultValue(GetAttributeIndex(_attName), _value);
}
int TemplateStructure::GetAttributeIndex(const QString& _attName) const
{
    int index = 0;
    for (const auto& attr : myAttributeTemplates)
    {
        if (attr.GetName() == _attName)
            return index;
        index++;
    }
    return -1;
}


Attribute::Type TemplateStructure::GetAttributeType(int _index) const
{
    const TemplateAttribute* attr = GetAttributeTemplate(_index);
    return attr ? attr->GetType() : Attribute::Type::Invalid;
}
const QString& TemplateStructure::GetAttributeName(int _index) const
{
    return myAttributeTemplates[_index].GetName();
}
const TemplateAttribute* TemplateStructure::GetAttributeTemplate(int _index) const
{
    if (_index < 0 || _index >= myAttributeTemplates.count())
        return nullptr;

    return &myAttributeTemplates[_index];
}
const TemplateAttribute* TemplateStructure::GetAttributeTemplate(const QString& att_Name) const
{
    return GetAttributeTemplate(GetAttributeIndex(att_Name));
}
