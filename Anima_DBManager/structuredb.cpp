#include "structuredb.h"

#include "aasset.h"
#include "iconmanager.h"


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
StructureDB::~StructureDB()
{
    ClearStructures();
}


bool StructureDB::CheckIndex(int& index) const
{
    return index >= 0 && index < GetStructureCount();
}

void StructureDB::AddStructureAt(int _index)
{
    if (!CheckIndex(_index))
        _index = GetStructureCount();

    qDebug("WIP -- Notify AReference attributes (?)");
    myStructures.insert(_index, new Structure(myTemplate));
}

void StructureDB::DuplicateStructureAt(int _index, int _originalIndex)
{
    if (!CheckIndex(_originalIndex))
    {
        AddStructureAt(_index);
        return;
    }


    if (!CheckIndex(_index))
        _index = GetStructureCount();

    myStructures.insert(_index, new Structure(*myStructures[_originalIndex], true));
}

void StructureDB::RemoveStructureAt(int _index)
{
    if (!CheckIndex(_index))
        return;

    qDebug("WIP -- Notify AReference attributes");
    auto* structToDelete = myStructures[_index];
    myStructures.removeAt(_index);
    delete structToDelete;
}

void StructureDB::MoveStructureAt(int _indexFrom, int& _indexTo)
{
    if (!CheckIndex(_indexFrom))
        return;
    if (!CheckIndex(_indexTo))
        _indexTo = GetStructureCount()-1;
    if (_indexTo == _indexFrom)
        return;

    auto* strct = myStructures.takeAt(_indexFrom);
    myStructures.insert(_indexTo, strct);
}

void StructureDB::SetStructureCount(int _count)
{
    const int originalCount = GetStructureCount();
    if (_count < originalCount)
    {
        while (_count < GetStructureCount()) {
            RemoveStructureAt(_count);
        }
    }
    else if (_count > originalCount)
    {
        while (_count > GetStructureCount()) {
            AddStructureAt(_count);
        }
    }

    Q_ASSERT(_count == GetStructureCount());
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
bool StructureDB::ChangeAttributeTemplate(int _attrIndex, const TemplateAttribute& _templateToCopy)
{
    return myTemplate.ChangeAttribute(_attrIndex, _templateToCopy);
}
void StructureDB::FixAttributesTypeToDefault(int _attIndex)
{
    for (auto* structure : myStructures)
    {
        structure->FixAttributeTypeToDefault(_attIndex);
    }
}
void StructureDB::AddAttribute(int _position, bool _copyFromPrevious)
{
    const int attrCount = myTemplate.GetAttributesCount();
    if (_position < 0 || _position > attrCount)
        _position = attrCount;

    if (_copyFromPrevious && _position == 0)
        return;

    if (_copyFromPrevious)
    {
        QString nameToUse = "";
        const auto* previous = myTemplate.GetAttributeTemplate(_position - 1);
        myTemplate.AddAttributeTemplate(*previous, &nameToUse, _position);

        nameToUse = myTemplate.GetAttributeTemplate(_position -1)->GetName();
        myTemplate.RenameAttributeTemplate(_position, nameToUse);
    }
    else
    {
        myTemplate.AddAttributeTemplate(_position);
        QString name = "New Attribute";
        myTemplate.RenameAttributeTemplate(_position, name);
    }


    for (auto* str : myStructures)
    {
        str->AddAttribute(_position, _copyFromPrevious);
    }
}
void StructureDB::RemoveAttribute(int _position)
{
    if (_position < 0 || _position >= myTemplate.GetAttributesCount())
        return;

    for (auto* str : myStructures)
    {
        str->RemoveAttribute(_position);
    }
    myTemplate.RemoveAttribute(_position);
}
void StructureDB::SetAttributesFromList(const QList<QString>& _stringList, QHash<AReference*, QString>& _outRefMap)
{
    // no use for this method outside of the Open action, if nec adapt it later to be usable at any state
    Q_ASSERT(myStructures.count() == 0);
    myTemplate.SetAttributeFromList(_stringList, _outRefMap);
}
bool StructureDB::UpdateMyAAssetIsDirty()
{
    QList<int> aassetIndexes = QList<int>();

    const int attrCount = myTemplate.GetAttributesCount();
    for (int i = 0; i < attrCount; i++)
    {
        if (AttributeTypeHelper::IsAssetType(myTemplate.GetAttributeType(i)))
        {
            aassetIndexes.push_back(i);
        }
    }

    if (aassetIndexes.isEmpty())
    {
        return false;
    }

    bool hasChanged = false;
    const int rowCount = myStructures.count();
    for (int row = 0; row < rowCount; row++)
    {
        auto* structure = myStructures[row];
        for (const int& i : aassetIndexes)
        {
            AAsset* attr = dynamic_cast<AAsset*>(structure->GetAttribute(i));
            Q_ASSERT(attr != nullptr);
            hasChanged = attr->UpdateIsDirty() || hasChanged;
        }
    }

    return hasChanged;
}




void StructureDB::SetTemplateName(const QString& _name)
{
    myTemplate.RenameStructureTemplate(_name);
}
void StructureDB::SetTemplateAbbrev(const QString& _abbrev)
{
    myTemplate.ReabbrevStructureTemplate(_abbrev);
}
void StructureDB::SetTemplateIconType(IconManager::IconType _iconType)
{
    myTemplate.SetStructureIcon(_iconType);
}
void StructureDB::SetTemplateColor(const QColor& _color)
{
    myTemplate.SetStructureColor(_color);
}
void StructureDB::SetTemplateAttributeName(int _index, QString& _name)
{
    myTemplate.RenameAttributeTemplate(_index, _name);
}
const QString& StructureDB::GetTemplateName() const
{
    return myTemplate.GetStructName();
}
const QString& StructureDB::GetTemplateAbbrev() const
{
    return myTemplate.GetStructAbbrev();
}
QIcon StructureDB::GetIcon() const
{
    return IconManager::GetSimpleIcon(myTemplate.GetStructIcon(), myTemplate.GetStructColor());
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

    return GetTemplateAbbrev() + QString("%1").arg(index, 3, 10, QChar('0')); //QString::number(index);
}
const QString StructureDB::GetStructureRowName(const Structure* _structure) const
{
    const int i = GetStructureIndex(_structure);
    return GetStructureRowName(i);
}
int StructureDB::GetStructureIndex(const Structure* _structure) const
{
    if (!_structure)
        return -1;

    const int count = GetStructureCount();
    int i = 0;
    for (; i < count; i++)
    {
        if (_structure == myStructures[i])
            break;
    }
    return i;
}

void StructureDB::WriteValue_CSV_Table(std::ofstream& file) const
{
    file << "---";
    const auto& templAttributes = myTemplate.GetAttributes();
    for (const auto* attr : templAttributes)
    {
        file << ",";
        file << attr->GetName().toStdString();
    }
    file << '\n';

    const int count = GetStructureCount();
    for (int i = 0; i < count; i++)
    {
        file << GetStructureRowName(i).toStdString();
        myStructures[i]->WriteValue_CSV_AsRow(file);
    }
}
void StructureDB::ReadValue_CSV_Table(int _index, const QStringList& fields, int _overwritePolicy)
{
    int rowCount = GetStructureCount();
    bool exists = _index < rowCount;

    if (exists && _overwritePolicy == 1)
    {
        // Keep existing
        // nothing to do
        return;
    }
    else if (!exists || _overwritePolicy == 2)
    {
        // Write in a new row
        _index = rowCount;
        AddStructureAt(-1);
    }
    // else (exists && policy=3): overwrite existing

    int attributeCount = fields.count();
    for (int i = 0; i < attributeCount; i++)
    {
        myStructures[_index]->ReadAttributeValue_CSV(i, fields[i]);
    }
}
void StructureDB::AddValue_CSV_TableWithDelayedReference(const QStringList& fields, QHash<AReference*, QString>& referenceMap)
{
    int newIndex = GetStructureCount();
    AddStructureAt(-1);

    int attributeCount = fields.count();
    for (int i = 0; i < attributeCount; i++)
    {
        if (myStructures[newIndex]->GetAttribute(i)->GetType() == AttributeTypeHelper::Type::Reference)
        {
            AReference* aref = dynamic_cast<AReference*>(myStructures[newIndex]->GetAttribute(i));
            Q_ASSERT(aref != nullptr);
            referenceMap.insert(aref, fields[i]);
            continue;
        }

        myStructures[newIndex]->ReadAttributeValue_CSV(i, fields[i]);
    }
}
