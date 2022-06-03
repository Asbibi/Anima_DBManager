#include "structuredb.h"

StructureDB::StructureDB(const TemplateStructure& _structureTemplate):
    myTemplate(_structureTemplate)
{}
StructureDB::~StructureDB()
{
    ClearStructures();
}

bool StructureDB::CheckIndex(int& index) const
{
    return index >= 0 && index < GetStructureCount();
}

void StructureDB::AddStructureAt(int index)
{
    if (!CheckIndex(index))
        index = GetStructureCount();

    myStructures.emplace(myStructures.begin() + index, new Structure(myTemplate));
}

void StructureDB::RemoveStructureAt(int index)
{
    if (!CheckIndex(index))
        return;

    auto structPtr = myStructures[index];
    myStructures.erase(myStructures.begin() + index);
    delete structPtr;
}

void StructureDB::ClearStructures()
{
    for (auto ptr : myStructures)
    {
        delete ptr;
    }

    myStructures.clear();
}



const TemplateStructure& StructureDB::GetTemplate() const
{
    return myTemplate;
}
const Structure* StructureDB::GetStructureAt(int index) const
{
    if (!CheckIndex(index))
        return nullptr;

    return myStructures[index];
}
int StructureDB::GetStructureCount() const
{
    return (int)myStructures.size();
}
const QString& StructureDB::GetTemplateName() const
{
    return myTemplate.GetStructName();
}
const QString StructureDB::GetStructureRowName(int index) const
{
    if (!CheckIndex(index))
        return "";

    return GetTemplateName() + "_" + QString::number(index);
}
const QString StructureDB::GetStructureRowName(const Structure* _structure) const
{
    if (!_structure)
        return "";

    const int count = GetStructureCount();
    int i = 0;
    for (; i < count; i++)
    {
        if (_structure == myStructures[i])
            break;
    }
    return GetStructureRowName(i);
}

void StructureDB::WriteValue_CSV_Table(std::ofstream& file) const
{
    file << "---";
    const auto& templAttributes = myTemplate.GetAttributes();
    for (const auto& attr : templAttributes)
    {
        file << ",";
        file << attr.GetName().toStdString();
    }
    file << '\n';

    const int count = GetStructureCount();
    for (int i = 0; i < count; i++)
    {
        file << GetStructureRowName(i).toStdString();
        myStructures[i]->WriteValue_CSV_AsRow(file);
    }
}
