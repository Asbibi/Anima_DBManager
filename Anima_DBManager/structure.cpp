#include "structure.h"


Structure::Structure(const Structure& _structure) : templ(_structure.templ)
{
    for (const auto& _att : _structure.attributes)
        attributes.push_back(_att->CreateDuplica());
}
Structure::Structure(const StructureTemplate& _structureTemplate) : templ(_structureTemplate)
{
    const std::vector<const Attribute*>& _templates = _structureTemplate.GetAttributes();
    for (const auto& _templ : _templates)
        attributes.push_back(_templ->CreateDuplica());
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
    return GetAttribute(templ.GetAttributeIndex(_attName));
}
void Structure::ChangeAttribute(int _attIndex, Attribute* _newAttribute)
{
    // ... todo depending on what i give : a New Attribute(), a Attribute(), do I copy or replace ? ....
}


void Structure::WriteValue_CSV(std::ofstream& file) const
{
    file << "(";

    // === Add all attributes ===
    for (int i = 0; i < attributes.size(); i++)
    {
        if (0 < i)
            file << ",";
        file << templ.GetAttributeName(i).toStdString();
        file << "=";
        attributes[i]->WriteValue_CSV(file);
    }
    // ==========================

    file << ")";
}
void Structure::WriteValue_CSV_AsRow(std::ofstream& file, int index) const
{
    // Add row name
    file << templ.GetStructName().toStdString() << "_" << index;

    // Add all attribute values in between ""
    for (const auto& att : attributes)
    {
        file << ",\"";
        att->WriteValue_CSV(file);
        file << "\"";
    }
    file << "\n";
}
