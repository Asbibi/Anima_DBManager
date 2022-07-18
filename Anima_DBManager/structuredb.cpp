#include "structuredb.h"

StructureDB::StructureDB(const TemplateStructure& _structureTemplate):
    myTemplate(_structureTemplate)
{}
StructureDB::StructureDB(const StructureDB& _another) :
    StructureDB(_another.myTemplate)
{
    for (const auto& otherStruct : _another.myStructures)
    {
        myStructures.push_back(new Structure(*otherStruct, myTemplate));
    }
}
void StructureDB::operator=(const StructureDB& _another)
{
    myStructures.clear();
    myTemplate = _another.myTemplate;
    for (const auto& otherStruct : _another.myStructures)
    {
        myStructures.push_back(otherStruct);
    }
}
StructureDB::~StructureDB()
{
    ClearStructures();
}
void StructureDB::UpdateTemplate()
{
    for (const auto* strct : myStructures)
    {
        qDebug("WIP -- Todo: propagate the changes done on myTemplate (add/remove/move/edit attribute template");
    }
}


bool StructureDB::CheckIndex(int& index) const
{
    return index >= 0 && index < GetStructureCount();
}

void StructureDB::AddStructureAt(int index)
{
    if (!CheckIndex(index))
        index = GetStructureCount();

    qDebug("WIP -- Notify AReference attributes");
    myStructures.insert(index, new Structure(myTemplate));
}

void StructureDB::RemoveStructureAt(int index)
{
    if (!CheckIndex(index))
        return;

    qDebug("WIP -- Notify AReference attributes");
    auto* structToDelete = myStructures[index];
    myStructures.removeAt(index);
    delete structToDelete;
}

void StructureDB::ClearStructures()
{
    while (myStructures.count() > 0)
    {
        RemoveStructureAt(0);
    }
}

void StructureDB::MoveAttribute(int _indexFrom, int _indexTo)
{
    const int attrCount = myTemplate.GetAttributesCount();
    if (_indexFrom == _indexTo)
        return;
    if (_indexFrom < 0 || _indexFrom >= attrCount)
        return;
    if(_indexTo < 0 || _indexTo >= attrCount)
        return;

    myTemplate.MoveAttribute(_indexFrom, _indexTo);

    for (auto* structure : myStructures)
    {
        structure->MoveAttribute(_indexFrom, _indexTo);
    }
}
void StructureDB::ResetAttributeToDefault(int _attrIndex)
{
    for (auto* structure : myStructures)
    {
        structure->ResetAttributeToDefault(_attrIndex);
    }
}
void StructureDB::ChangeAttributeTemplate(int _attrIndex, AttributeTypeHelper::Type _newType, const AttributeParam& _param)
{
    myTemplate.GetAttributeTemplate(_attrIndex)->SetNewValues(_newType, _param);
}
void StructureDB::FixAttributesTypeToDefault(int _attIndex)
{
    for (auto* structure : myStructures)
    {
        structure->FixAttributeTypeToDefault(_attIndex);
    }
}




void StructureDB::SetTemplateName(const QString& _name)
{
    myTemplate.RenameStructureTemplate(_name);
}
void StructureDB::SetTemplateAttributeName(int _index, QString& _name)
{
    myTemplate.RenameAttributeTemplate(_index, _name);
}
const QString& StructureDB::GetTemplateName() const
{
    return myTemplate.GetStructName();
}
QString StructureDB::GetTemplateColorString() const
{
    return myTemplate.GetStructColor().name();
}
const TemplateStructure& StructureDB::GetTemplate() const
{
    return myTemplate;
}
AttributeTypeHelper::Type StructureDB::GetAttributeTemplateType(int _attributeIndex) const
{
    return myTemplate.GetAttributeType(_attributeIndex);
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
