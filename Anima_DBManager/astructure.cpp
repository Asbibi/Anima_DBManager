#include "astructure.h"
#include<QDebug>

AStructure::AStructure(const StructureTemplate& structureTemplate) :
    Attribute(nullptr),
    value(structureTemplate)
{}
AStructure::AStructure(const Structure& _value) :
    Attribute(nullptr),
    value(_value)
{}



Attribute* AStructure::CreateDuplica() const
{
    return new AStructure(value);
}
QString AStructure::GetDisplayedText() const
{
    return "{STRUCT}";
}
void AStructure::WriteValue_CSV(std::ofstream& file) const
{
    value.WriteValue_CSV(file);
}
std::vector<QString> AStructure::GetDisplayedTexts() const
{
    int _count = value.GetAttributeCount();
    std::vector<QString> strings = std::vector<QString>();
    strings.reserve(_count);
    for (int i = 0; i< _count; i++)
        strings.push_back(value.GetAttribute(i)->GetDisplayedText());

    return strings;
}
