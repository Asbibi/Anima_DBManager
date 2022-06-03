#include "structure.h"

#include <QList>

Structure::Structure(const Structure& _other) : templ(_other.templ)
{
    for (const auto& _att : _other.attributes)
        attributes.push_back(_att->CreateDuplica());
}
Structure::Structure(const TemplateStructure& _structureTemplate) : templ(&_structureTemplate)
{
    const QList<TemplateAttribute>& _templateAttributes = _structureTemplate.GetAttributes();
    for (const auto& _templ : _templateAttributes)
        attributes.push_back(_templ.GenerateAttribute());
}
Structure::~Structure()
{
    for(auto const& att : attributes)
        delete(att);
}



int Structure::GetAttributeCount() const
{
    return (int)attributes.size();
}
const Attribute* Structure::GetAttribute(int _attIndex) const
{
    if (_attIndex < 0 || _attIndex >= attributes.size())
        return nullptr;

    return attributes[_attIndex];
}
const Attribute* Structure::GetAttribute(const QString& _attName) const
{
    return GetAttribute(templ->GetAttributeIndex(_attName));
}
void Structure::SetAttributeValueFromText(int _attIndex, QString _valueText)
{
    if (_attIndex < 0 || _attIndex >= attributes.size())
        return;
    attributes[_attIndex]->SetValueFromText(_valueText);
}
void Structure::SetAttributeValueFromText(const QString& _attName, QString _valueText)
{
    SetAttributeValueFromText(templ->GetAttributeIndex(_attName), _valueText);
}
void Structure::ChangeAttribute(int _attIndex, Attribute* _newAttribute)
{
    // ... todo depending on what i give : a New Attribute(), a Attribute(), do I copy or replace ? ....
}



void Structure::GetAttributesDisplayedText(QString& _text) const
{
    for (int i = 0; i < attributes.size(); i++)
    {
        if (i > 0)
            _text.append(',');
        _text.append(attributes[i]->GetDisplayedText(true));
    }
}
void Structure::WriteValue_CSV(std::ofstream& file) const
{
    file << "(";

    // === Add all attributes ===
    for (int i = 0; i < attributes.size(); i++)
    {
        if (0 < i)
            file << ",";
        file << templ->GetAttributeName(i).toStdString();
        file << "=";
        attributes[i]->WriteValue_CSV(file);
    }
    // ==========================

    file << ")";
}
void Structure::WriteValue_CSV_AsRow(std::ofstream& file) const
{
    // Add all attribute values in between ""
    for (const auto& att : attributes)
    {
        file << ",\"";
        att->WriteValue_CSV(file);
        file << "\"";
    }
    file << "\n";
}
