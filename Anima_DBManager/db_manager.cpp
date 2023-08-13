#include "db_manager.h"

#include "aarray.h"
#include "abool.h"
#include "aenumerator.h"
#include "afloat.h"
#include "aint.h"
#include "ashortstring.h"
#include "astructure.h"
#include "areference.h"
#include "atablestring.h"

#include "aasset.h"
#include "aamesh.h"
#include "aaniagara.h"
#include "aasound.h"
#include "aatexture.h"

#include <fstream>

#include <QDir>
#include <qdebug.h>

DB_Manager::DB_Manager()
{}
DB_Manager::~DB_Manager()
{
    while (myStructures.count())
    {
        RemoveStructureDB(0);
    }
}
DB_Manager& DB_Manager::GetDB_Manager()
{
    static DB_Manager singleton = DB_Manager();
    return singleton;
}

bool DB_Manager::SetProjectContentFolderPath(const QString& _path)
{
    myProjectContentFolderPath = _path;
    myProjectPathIsValid = QDir(myProjectContentFolderPath).exists();
    return myProjectPathIsValid;
}
const QString& DB_Manager::GetProjectContentFolderPath(bool _homePathIfUnvalid) const {
    return myProjectPathIsValid || !_homePathIfUnvalid ? myProjectContentFolderPath : myHomePath;
}
bool DB_Manager::IsProjectContentFolderPathValid() const
{
    return myProjectPathIsValid;
}


void DB_Manager::Init()
{
#define testValues
#ifdef testValues

    // Project Folder
    SetProjectContentFolderPath("D:/Documents/Unreal/Anima_OLD/Content/");
    qDebug() << "Project path is valid : " << myProjectPathIsValid;

    // Enums (TODO : remove to instead add automatic creation from loading file)
    AddEnum(Enumerator("Type", {"SOUL", "FIRE", "WATER", "GRASS", "GROUND", "THUNDER", "WIND", "SNOW", "MYSTIC", "CORRUPTION", "TIME", "SPACE"},
                       {QColorConstants::Gray,
                        QColorConstants::Red, QColorConstants::Svg::mediumblue, QColorConstants::DarkGreen,
                        QColorConstants::Svg::saddlebrown, QColorConstants::Svg::goldenrod, QColorConstants::DarkCyan,
                        QColorConstants::Svg::midnightblue, QColorConstants::Svg::cornflowerblue, QColorConstants::Svg::dimgray,
                        QColorConstants::Svg::maroon, QColorConstants::Svg::darkorchid}));
    AddEnum(Enumerator("MoveCategory", {"PHYSIC","SPECIAL","SUPPORT"}));

    // String Tables;
    int strIndex = -1;
    AddStringTablePrivate("Ceribou", strIndex);
    AddStringTablePrivate("Shitty Stuff", strIndex);
    QString text1s[] = {"Ceribou (anglais : Cherubi ; japonais : チェリンボ Cherinbo) est un Pokémon de type Plante de la quatrième génération.",
                        "Cherubi (Japanese: チェリンボ Cherinbo) is a Grass-type Pokémon introduced in Generation IV. It evolves into Cherrim starting at level 25."};
    QString text2s[] = {"Palkia shiny de ses morts", ""};
    QString text3s[] = {"Les grandes apparitions de ce Pokémon remontent à son utilisation par Flo dans les DP031 et 37.\nOn peut noter l'apparition d'un Ceribou dans le film Pokémon : L'ascension de Darkrai.\nOn reverra très peu (ou très rapidement) ce Pokémon par la suite (DP066 et 88), jusqu'à son utilisation par Maryline dans le DP125 : en effet, cette dernière juge Ceribou faisant partie des Pokémon mignons.",
                        "Cherubi debuted in The Grass-type Is Always Greener!, under the ownership of Gardenia.\nIt was first used in a battle against Ash. It faced off against Turtwig, and during the battle, Gardenia was able to learn quite a bit about Turtwig.\nCherubi reappeared in The Grass Menagerie!, where it once again fought Turtwig during an official Gym battle.\nIts speed forced Ash to recall Turtwig for Staravia, after which Cherubi was defeated."};
    QString id = "Palkia";
    QString text4s[] = {"He ho hohohoh", "ah ahaha ahiiiii"};
    QString id2 = "7 nains";
    myStringTables[0].AddStringItemWithTexts(-1, text1s);
    myStringTables[0].AddStringItem(-1);
    myStringTables[0].AddStringItem(-1);
    myStringTables[0].AddStringItem(-1);
    myStringTables[0].AddStringItemWithTexts(-1, text2s, &id);
    myStringTables[0].AddStringItemWithTexts(-1,text3s);
    myStringTables[1].AddStringItem(-1);
    myStringTables[1].AddStringItem(-1,&id);
    myStringTables[1].AddStringItemWithTexts(-1,text4s, &id2);
    emit StringTableAdded(0);
    emit StringTableAdded(1);

    /*
    // Struct Tables;
    AddStructureDB({"BasicStruct", QColorConstants::DarkRed}, 0);
    AddAttributeTemplate(0, 0, false);
    AddAttributeTemplate(0, 1, false);
    AddAttributeTemplate(0, 2, false);
    AddAttributeTemplate(0, 3, false);
    AddAttributeTemplate(0, 4, false);
    ChangeAttributeTemplate(0, 0, AttributeTypeHelper::Type::Texture, AttributeParam(), true);
    ChangeAttributeTemplate(0, 1, AttributeTypeHelper::Type::Sound, AttributeParam(), true);
    ChangeAttributeTemplate(0, 2, AttributeTypeHelper::Type::Bool, AttributeParam(), true);
        AttributeParam tempEnumParam = AttributeParam();
        tempEnumParam.enumeratorIndex = 0;
    ChangeAttributeTemplate(0, 3, AttributeTypeHelper::Type::Enum, tempEnumParam, true);
    ChangeAttributeTemplate(0, 4, AttributeTypeHelper::Type::TableString, AttributeParam(), true);
    QString name = "TextureAttr";
    RenameStructureAttribute(0,0, name);
    name = "SoundAttr";
    RenameStructureAttribute(0,1, name);
    name = "BoolAttr";
    RenameStructureAttribute(0,2, name);
    name = "EnumAttr";
    RenameStructureAttribute(0,3, name);
    name = "SStringAttr";
    RenameStructureAttribute(0,4, name);

    AddStructureRow(0,0);
    AddStructureRow(0,0);
    AddStructureRow(0,0);


    AddStructureDB({"ConcreteStruct", QColorConstants::DarkBlue}, 1);
    AddAttributeTemplate(1, 0, false);
    AddAttributeTemplate(1, 1, false);
    AddAttributeTemplate(1, 2, false);
    AddAttributeTemplate(1, 3, false);
    AddAttributeTemplate(1, 4, false);
    AddAttributeTemplate(1, 5, false);
    AttributeParam tempStructParam = AttributeParam();
    tempStructParam.templateStruct = new TemplateStructure("", QColorConstants::Black);
    tempStructParam.templateStruct->AddAttributeTemplate(AttributeTypeHelper::Type::Int, "0", 0);
    tempStructParam.templateStruct->AddAttributeTemplate(AttributeTypeHelper::Type::ShortString, "1", 1);
    tempStructParam.templateStruct->AddAttributeTemplate(AttributeTypeHelper::Type::Bool, "2", 2);
    ChangeAttributeTemplate(1, 0, AttributeTypeHelper::Type::Structure, tempStructParam, true);
    ChangeAttributeTemplate(1, 1, AttributeTypeHelper::Type::Int, AttributeParam(), true);
    ChangeAttributeTemplate(1, 2, AttributeTypeHelper::Type::Float, AttributeParam(), true);
    ChangeAttributeTemplate(1, 3, AttributeTypeHelper::Type::ShortString, AttributeParam(), true);
    AttributeParam tempRefParam = AttributeParam();
    tempRefParam.structTable = GetStructureTable(0);
    ChangeAttributeTemplate(1, 4, AttributeTypeHelper::Type::Reference, tempRefParam, true);
    AttributeParam tempArrayParam = AttributeParam();
    tempArrayParam.templateAtt = new TemplateAttribute("", AttributeTypeHelper::Type::Int, AttributeParam());    
    AttributeParam tempArrayArrayParam = AttributeParam();
    tempArrayArrayParam.templateAtt = new TemplateAttribute("", AttributeTypeHelper::Type::Array, tempArrayParam);
    ChangeAttributeTemplate(1, 5, AttributeTypeHelper::Type::Array, tempArrayArrayParam, true);
    name = "StrcutAttr";
    RenameStructureAttribute(1,0, name);
    name = "IntAttr";
    RenameStructureAttribute(1,1, name);
    name = "FloatAttr";
    RenameStructureAttribute(1,2, name);
    name = "StrAttr";
    RenameStructureAttribute(1,3, name);
    name = "RefAttr";
    RenameStructureAttribute(1,4, name);
    name = "ArrayAttr";
    RenameStructureAttribute(1,5, name);

    AddStructureRow(1,0);
    AddStructureRow(1,0);

    */

    AddStructureDB({"ArrayStruct", QColorConstants::DarkCyan}, 0);
    AddAttributeTemplate(0, 0, false);
    AddStructureRow(0,0);

#else

#endif

}

void DB_Manager::Reset()
{
    blockSignals(true);

    int structTableCount = myStructures.count();
    for (int i = structTableCount -1; i > -1; i--)
    {
        RemoveStructureDB(i);
    }
    int stringTableCount = myStringTables.count();
    for (int i = stringTableCount -1; i > -1; i--)
    {
        RemoveStringTable(i);
    }
    int enumCount = enumerators.count();
    for (int i = enumCount -1; i > -1; i--)
    {
        RemoveEnum(i);
    }
    //myStructures.clear();
    //enumerators.clear();
    //myStringTables.clear();
    //myAttributeParamPtrs.clear();

    myProjectContentFolderPath = "";

    blockSignals(false);

    emit ResetView();
}



// ==============================================================
// ==============================================================




int DB_Manager::GetEnumCount() const
{
    return (int)enumerators.size();
}
const Enumerator* DB_Manager::GetEnum(int _index) const
{
    if (_index < 0 || _index >= (int)enumerators.size())
        return nullptr;

    return &enumerators[_index];
}
void DB_Manager::AddEnum(const Enumerator& _enum, int _index)
{
    const int enumCount = enumerators.size();
    if (_index < 0 || _index > enumCount)
        _index = enumCount;

    for (auto& attrParamPtr : myAttributeParamPtrs)
    {
        int& enumIndex = attrParamPtr->enumeratorIndex;
        if (_index <= enumIndex)
            enumIndex++;
    }
    enumerators.insert(_index, _enum);
}
void DB_Manager::MoveEnum(const int _indexFrom, const int _indexTo)
{
    if (_indexFrom == _indexTo)
        return;
    const int enumCount = enumerators.size();
    if (_indexFrom < 0 || _indexFrom >= enumCount)
        return;
    if (_indexTo < 0 || _indexTo >= enumCount)
        return;

    for (auto& attrParamPtr : myAttributeParamPtrs)
    {
        int& enumIndex = attrParamPtr->enumeratorIndex;
        if (enumIndex == -1)
            continue;

        else if (enumIndex == _indexFrom)
            enumIndex = _indexTo;
        else if (_indexTo <= enumIndex && enumIndex < _indexFrom)
            enumIndex++;
        else if (_indexFrom < enumIndex && enumIndex <= _indexTo)
            enumIndex--;
    }

    auto movedEnum = enumerators.takeAt(_indexFrom);
    enumerators.insert(_indexTo, movedEnum);
}
bool DB_Manager::CanSafelyRemoveEnum(int _index) const
{
    if (_index < 0 || _index >= (int)enumerators.size())
        return false;

    for (const auto& attrParamPtr : myAttributeParamPtrs)
    {
        if (attrParamPtr->enumeratorIndex == _index)
            return false;
    }
    return true;
}
void DB_Manager::RemoveEnum(int _index)
{
    if (_index < 0 || _index >= (int)enumerators.size())
        return;

    // Assume that CanSafelyRemoveEnum() has been called before and returned true
    for (auto& attrParamPtr : myAttributeParamPtrs)
    {
        int& enumIndex = attrParamPtr->enumeratorIndex;
        if (_index < enumIndex)
            enumIndex--;
    }

    enumerators.removeAt(_index);
}
void DB_Manager::UpdateEnum(int _index, const Enumerator& _another)
{
    if (_index < 0 || _index >= (int)enumerators.size())
        return;

    enumerators[_index].operator=(_another);
}
void DB_Manager::UpdateEnumName(int _index, const QString& _name)
{
    if (_index < 0 || _index >= (int)enumerators.size())
        return;

    enumerators[_index].SetName(_name);
}



// ==============================================================
// ==============================================================



void DB_Manager::RegisterAttributeParam(AttributeParam* _param)
{
    myAttributeParamPtrs.append(_param);
}
void DB_Manager::UnregisterAttributeParam(AttributeParam* _param)
{
    myAttributeParamPtrs.removeOne(_param);
}



// ==============================================================
// ==============================================================



int DB_Manager::GetStructureTableIndex(const QString& _structName) const
{
    const int count = GetStructuresCount();
    for (int i = 0; i < count; i++)
        if (myStructures[i]->GetTemplateName() == _structName)
            return i;

    return -1;
}
int DB_Manager::GetStructuresCount() const
{
    return (int)(myStructures.size());
}
const StructureDB* DB_Manager::GetStructureTable(int index) const
{
    if (index < 0)
        index = 0;
    else if (index >= GetStructuresCount())
        index = GetStructuresCount() -1;

    return myStructures[index];
}
StructureDB* DB_Manager::GetStructureTable(int index)
{
    if (myStructures.size() == 0)
        return nullptr;

    if (index < 0 || index >= GetStructuresCount())
        return nullptr;

    return myStructures[index];
}
const StructureDB* DB_Manager::GetStructureTable(const QString& _SDBName) const
{
    return GetStructureTable(GetStructureTableIndex(_SDBName));
}
StructureDB* DB_Manager::GetStructureTable(const QString& _SDBName)
{
    return GetStructureTable(GetStructureTableIndex(_SDBName));
}
void DB_Manager::AddStructureDB(const TemplateStructure& _structureTemplate, int _index)
{
    const int count = myStructures.count();
    if (_index < 0 || _index > count)
        _index = count;

    QString originalName = _structureTemplate.GetStructName();
    auto validate = [this](const QString& _identifier)->bool{ return (bool)(GetStructureTableIndex(_identifier) == -1); };
    const QString identifier = SStringHelper::GetUniqueIdentifier(originalName, validate, true);

    myStructures.insert(_index, new StructureDB(_structureTemplate));
    myStructures[_index]->SetTemplateName(identifier);
    emit StructTableAdded(_index);
}
void DB_Manager::DuplicateStructureDB(int _index, int _indexOriginal)
{
    const int count = myStructures.count();
    if (_indexOriginal < 0 || _indexOriginal > count)
        return;
     if (_index < 0 || _index > count)
         _index = count;

     StructureDB& original = *myStructures[_indexOriginal];
     QString originalName = original.GetTemplateName();
     auto validate = [this](const QString& _identifier)->bool{ return (bool)(GetStructureTableIndex(_identifier) == -1); };
     const QString identifier = SStringHelper::GetUniqueIdentifier(originalName, validate, true);

     myStructures.insert(_index, new StructureDB(original));
     myStructures[_index]->SetTemplateName(identifier);
     emit StructTableAdded(_index);
}
void DB_Manager::MoveStructureDB(int _indexFrom, int _indexTo)
{
    const int count = myStructures.count();
    if (_indexFrom < 0 || _indexFrom > count)
        return;
    if (_indexTo < 0 || _indexTo > count)
        _indexTo = count;

    myStructures.move(_indexFrom, _indexTo);
    emit StructTableMoved(_indexFrom, _indexTo);
}
void DB_Manager::RemoveStructureDB(int _index)
{
    const int count = myStructures.count();
    if (_index < 0 || _index > count)
        return;


    // Clean up attributeParam that reference the deleted Struct Table (for AReference)
    auto* structDB = myStructures.takeAt(_index);
    for (auto* attrParam : myAttributeParamPtrs)
    {
        if (attrParam->structTable == structDB)
            attrParam->structTable = nullptr;
    }

    emit StructTableRemoved(_index);

    delete structDB;
}
void DB_Manager::RemoveStructureDB(const QString& _tableName)
{
    const int count = myStructures.count();
    int index = -1;
    for (int i = 0; i < count; i++)
    {
        if (myStructures[i]->GetTemplateName() == _tableName)
        {
            index = i;
            break;
        }
    }
    RemoveStructureDB(index);
}
void DB_Manager::RenameStructureDB(int _index, const QString& _tableName)
{
    const int count = myStructures.count();
    if (_index < 0 || _index > count)
        return;

    myStructures[_index]->SetTemplateName(_tableName);
    emit StructTableRenamed(_index, _tableName);
}
void DB_Manager::MoveStructureAttribute(int _tableIndex, int _indexFrom, int _indexTo)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    auto* structDB = myStructures[_tableIndex];
    Q_ASSERT(structDB);
    structDB->MoveAttribute(_indexFrom, _indexTo);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::MoveStructureAttribute(const QString& _tableName, int _indexFrom, int _indexTo)
{
    MoveStructureAttribute(GetStructureTableIndex(_tableName), _indexFrom, _indexTo);
}
void DB_Manager::RenameStructureAttribute(int _tableIndex, int _attributeIndex, QString& _attributeName)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->SetTemplateAttributeName(_attributeIndex, _attributeName);

    emit StructAttributeNameChanged(_tableIndex);
}
void DB_Manager::RenameStructureAttribute(const QString& _tableName, int _attributeIndex, QString& _attributeName)
{
    RenameStructureAttribute(GetStructureTableIndex(_tableName), _attributeIndex, _attributeName);
}
void DB_Manager::ResetAttributesToDefaultValue(int _tableIndex, int _attributeIndex)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->ResetAttributeToDefault(_attributeIndex);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::ResetAttributesToDefaultValue(const QString& _tableName, int _attributeIndex)
{
    ResetAttributesToDefaultValue(GetStructureTableIndex(_tableName), _attributeIndex);
}
void DB_Manager::ChangeAttributeTemplate(int _tableIndex, int _attrIndex, AttributeTypeHelper::Type _newType, const AttributeParam& _param, bool _needResetValue)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    AttributeTypeHelper::Type previousType = myStructures[_tableIndex]->GetAttributeTemplateType(_attrIndex);
    myStructures[_tableIndex]->ChangeAttributeTemplate(_attrIndex, _newType, _param);
    if (previousType != _newType)
    {
        myStructures[_tableIndex]->FixAttributesTypeToDefault(_attrIndex);
        emit StructItemChanged(_tableIndex);
    }
    else if (_needResetValue)
    {
        ResetAttributesToDefaultValue(_tableIndex, _attrIndex);
    }
    else
    {
        emit StructItemChanged(_tableIndex);
    }
}
void DB_Manager::ChangeAttributeTemplate(const QString& _tableName, int _attrIndex, AttributeTypeHelper::Type _newType, const AttributeParam& _param, bool _needResetValue)
{
    ChangeAttributeTemplate(GetStructureTableIndex(_tableName), _attrIndex, _newType, _param, _needResetValue);
}
void DB_Manager::AddAttributeTemplate(int _tableIndex, int _attrIndex, bool _copyFromPrevious)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->AddAttribute(_attrIndex, _copyFromPrevious);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::AddAttributeTemplate(const QString& _tableName, int _attrIndex, bool _copyFromPrevious)
{
    AddAttributeTemplate(GetStructureTableIndex(_tableName), _attrIndex, _copyFromPrevious);
}
void DB_Manager::RemoveAttributeTemplate(int _tableIndex, int _attrIndex)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->RemoveAttribute(_attrIndex);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::RemoveAttributeTemplate(const QString& _tableName, int _attrIndex)
{
    RemoveAttributeTemplate(GetStructureTableIndex(_tableName), _attrIndex);
}
void DB_Manager::SetAttributeTemplatesFromStringList(int _tableIndex, const QList<QString>& _stringList, QHash<AReference*, QString>& _outRefMap)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->SetAttributesFromList(_stringList, _outRefMap);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::AddStructureRow(const int _tableIndex, const int _position)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->AddStructureAt(_position);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::DuplicateStructureRow(const int _tableIndex, const int _position, const int _originalIndex)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->DuplicateStructureAt(_position, _originalIndex);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::RemoveStructureRow(const int _tableIndex, const int _position)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->RemoveStructureAt(_position);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::MoveStructureRow(const int _tableIndex, const int _positionFrom, int& _positionTo)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->MoveStructureAt(_positionFrom, _positionTo);
    if (_positionFrom != _positionTo)
        emit StructItemChanged(_tableIndex);
}
void DB_Manager::UpdateAAssetIsDirty()
{
    const int structDbCount = myStructures.count();
    for (int i = 0; i < structDbCount; i++)
    {
        if (myStructures[i]->UpdateMyAAssetIsDirty())
        {
            emit StructItemChanged(i);
        }
    }
}




void DB_Manager::AskFocusOnStructPanel(const int _tableIndex, const int _itemIndex)
{
    emit StructTableFocus(_tableIndex, _itemIndex);
}
void DB_Manager::AskFocusOnStructPanel(const QString& _tableName, const int _itemIndex)
{
    AskFocusOnStructPanel(GetStructureTableIndex(_tableName), _itemIndex);
}



// ==============================================================
// ==============================================================




int DB_Manager::GetStringTableIndex(const QString& _tableName) const
{
    const int count = GetStringTableCount();
    for (int i = 0; i < count; i++)
        if (myStringTables[i].GetTableName() == _tableName)
            return i;

    return -1;
}
int DB_Manager::GetStringTableCount() const
{
    return (int)(myStringTables.size());
}
const SStringTable* DB_Manager::GetStringTable(int _index) const
{
    if (_index < 0 || _index >= GetStringTableCount())
        return nullptr;

    return &myStringTables[_index];
}
const SStringTable* DB_Manager::GetStringTable(const QString& _tableName) const
{
    return GetStringTable(GetStringTableIndex(_tableName));
}
SStringTable* DB_Manager::GetStringTable(int _index)
{
    if (_index < 0 || _index >= GetStringTableCount())
        return nullptr;

    return &myStringTables[_index];
}
SStringTable* DB_Manager::GetStringTable(const QString& _tableName)
{
    return GetStringTable(GetStringTableIndex(_tableName));
}
void DB_Manager::AddStringTablePrivate(const QString& _newTableName, int& _index)
{
    const int count = myStringTables.count();
    if (_index < 0 || _index > count)
        _index = count;

    QString baseIdentifier = _newTableName;
    auto validate = [this](const QString& _identifier)->bool{ return (bool)(GetStringTableIndex(_identifier) == -1); };
    const QString identifier = SStringHelper::GetUniqueIdentifier(baseIdentifier, validate, true);

    myStringTables.insert(_index, SStringTable(identifier));
}
void DB_Manager::AddStringTable(const QString& _newTableName, int _index)
{
    AddStringTablePrivate(_newTableName, _index);
    myStringTables[_index].AddStringItem(-1);
    emit StringTableAdded(_index);
}
void DB_Manager::DuplicateStringTable(int _index, int _indexOriginal)
{
    if (_indexOriginal < 0 || _indexOriginal >= myStringTables.count())
        return;

    const QString originalName = myStringTables[_indexOriginal].GetTableName();
    AddStringTablePrivate(originalName, _index);

    if (_index <= _indexOriginal)
        _indexOriginal++;
    const SStringTable& originalTable = myStringTables[_indexOriginal];
    const auto& strings = originalTable.GetStringItems();
    for (const auto& str : strings)
    {
        myStringTables[_index].AddStringItemFromCopy(-1, str);
    }
    emit StringTableAdded(_index);
}
void DB_Manager::MoveStringTable(int _indexFrom, int _indexTo)
{
    if (_indexFrom == _indexTo)
        return;

    const int count = GetStringTableCount();
    if (_indexFrom < 0 || _indexFrom >= count)
        return;
    if (_indexTo < 0 || _indexTo >= count)
        _indexTo = count - 1;

    auto item = myStringTables.takeAt(_indexFrom);
    myStringTables.insert(_indexTo, item);
    emit StringTableMoved(_indexFrom, _indexTo);
}
void DB_Manager::RemoveStringTable(int _index)
{
    if (_index < 0 || _index >= GetStringTableCount())
        return;

    myStringTables.removeAt(_index);
    emit StringTableRemoved(_index);
}
void DB_Manager::RemoveStringTable(const QString& _tableName)
{
    RemoveStringTable(GetStringTableIndex(_tableName));
}
void DB_Manager::RenameStringTable(int _index, const QString& _tableName)
{
    if (_index < 0 || _index >= GetStringTableCount())
        return;

     myStringTables[_index].SetTableName(_tableName);
     emit StringTableRenamed(_index, _tableName);

     // TODO (?) : update all attributes using this table
}

bool DB_Manager::AreValidIdentifiers(const QString& _tableId, const QString& _stringId) const
{
    const auto* stringTable = GetStringTable(_tableId);
    if (!stringTable)
        return false;

    const auto* string = stringTable->GetStringItem(_stringId);
    return string != nullptr;
}
QString DB_Manager::GetStringForDisplay(const QString& _tableId, const QString& _stringId, bool _complete) const
{
    if (!AreValidIdentifiers(_tableId, _stringId))
        return _complete ? "Ø" : "<font color=\"darkred\">INVALID</font>";

    const QString* myStr = GetStringTable(_tableId)->GetString(_stringId, SStringHelper::SStringLanguages::French);
    if (!myStr)
        return _complete ? "-" : "<font color=\"darkyellow\">INVALID</font>";

    if (_complete)
        return *myStr;

    return (myStr->length() > 15) ? (myStr->left(15) + "<font color=\"blue\">[...]</font>") : *myStr;  //<font color=\"blue\">Hello</font> //"[...]"
}

void DB_Manager::AskFocusOnStringItem(const int _tableIndex, const int _stringIndex)
{
    emit StringItemFocus(_tableIndex, _stringIndex);
}
void DB_Manager::AskUpdateOnStringTable(const int _tableIndex)
{
    emit StringItemChanged(_tableIndex);
}
void DB_Manager::AskUpdateOnStringPanel(const int _tableIndex)
{
    emit StringTableChanged(_tableIndex);
}
