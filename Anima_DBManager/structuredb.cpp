#include "structuredb.h"

StructureDB::StructureDB(const StructureTemplate& _structureTemplate):
    templ(_structureTemplate)
{}


void StructureDB::BoundIndex(int& index) const
{
    if(index < 0)
        index = 0;
    else if (index > GetStructureCount())
        index = GetStructureCount();
}

void StructureDB::AddStructureAt(int index)
{
    BoundIndex(index);
    myStructures.emplace(myStructures.begin() + index, Structure(templ));
}

void StructureDB::RemoveStructureAt(int index)
{
    BoundIndex(index);
    myStructures.erase(myStructures.begin() + index);
}

void StructureDB::ClearStructures()
{
    myStructures.clear();
}



const StructureTemplate& StructureDB::GetTemplate() const
{
    return templ;
}
const Structure& StructureDB::GetStructureAt(int index) const
{
    BoundIndex(index);
    return myStructures[index];
}
int StructureDB::GetStructureCount() const
{
    return (int)myStructures.size();
}
const QString& StructureDB::GetTemplateName() const
{
    return templ.GetStructName();
}
