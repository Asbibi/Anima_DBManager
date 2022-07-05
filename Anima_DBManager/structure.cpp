#include "structure.h"

#include <QList>

Structure::Structure(TemplateStructure& _structureTemplate) :
    myTemplate(_structureTemplate)
{
    const QList<TemplateAttribute>& _templateAttributes = _structureTemplate.GetAttributes();
    for (const auto& _templ : _templateAttributes)
        myAttributes.push_back(_templ.GenerateAttribute());
}
Structure::Structure(const Structure& _other) :
    myTemplate(_other.myTemplate)
{
    for (auto* _att : _other.myAttributes)
        myAttributes.push_back(_att);
}
Structure::Structure(const Structure& _other, TemplateStructure& _newTemplate) :
    myTemplate(_newTemplate)
{
    const int myAttrCount = myTemplate.GetAttributesCount();
    for (int i = 0; i < myAttrCount; i++)
    {
        auto* newAttr = myTemplate.GetAttributeTemplate(i)->GenerateAttribute();
        newAttr->CopyValueFromOther(_other.GetAttribute(i));
        myAttributes.push_back(newAttr);
    }
}
void Structure::operator=(const Structure& _other)
{
    // Why it happens ? => do we want a shallow ptr copy or a deep data copy ?

    myAttributes.clear();
    myTemplate = _other.myTemplate;

    const int myAttrCount = myTemplate.GetAttributesCount();
    for (int i = 0; i < myAttrCount; i++)
    {
        auto* newAttr = myTemplate.GetAttributeTemplate(i)->GenerateAttribute();
        newAttr->CopyValueFromOther(_other.GetAttribute(i));
        myAttributes.push_back(newAttr);
    }
}
Structure::~Structure()
{
    for(auto const& att : myAttributes)
        delete(att);
}




int Structure::GetAttributeCount() const
{
    return (int)myAttributes.size();
}
const Attribute* Structure::GetAttribute(int _attIndex) const
{
    if (_attIndex < 0 || _attIndex >= myAttributes.size())
        return nullptr;

    return myAttributes[_attIndex];
}
const Attribute* Structure::GetAttribute(const QString& _attName) const
{
    return GetAttribute(myTemplate.GetAttributeIndex(_attName));
}
void Structure::SetAttributeValueFromText(int _attIndex, QString _valueText)
{
    if (_attIndex < 0 || _attIndex >= myAttributes.size())
        return;
    myAttributes[_attIndex]->SetValueFromText(_valueText);
}
void Structure::SetAttributeValueFromText(const QString& _attName, QString _valueText)
{
    SetAttributeValueFromText(myTemplate.GetAttributeIndex(_attName), _valueText);
}
void Structure::ChangeAttribute(int _attIndex, Attribute* _newAttribute)
{
    // ... todo depending on what i give : a New Attribute(), a Attribute(), do I copy or replace ? ....
}



void Structure::GetAttributesDisplayedText(QString& _text) const
{
    for (int i = 0; i < myAttributes.size(); i++)
    {
        if (i > 0)
            _text.append(',');
        _text.append(myAttributes[i]->GetDisplayedText(true));
    }
}
void Structure::WriteValue_CSV(std::ofstream& file) const
{
    file << "(";

    // === Add all attributes ===
    for (int i = 0; i < myAttributes.size(); i++)
    {
        if (0 < i)
            file << ",";
        file << myTemplate.GetAttributeName(i).toStdString();
        file << "=";
        myAttributes[i]->WriteValue_CSV(file);
    }
    // ==========================

    file << ")";
}
void Structure::WriteValue_CSV_AsRow(std::ofstream& file) const
{
    // Add all attribute values in between ""
    for (const auto& att : myAttributes)
    {
        file << ",\"";
        att->WriteValue_CSV(file);
        file << "\"";
    }
    file << "\n";
}
