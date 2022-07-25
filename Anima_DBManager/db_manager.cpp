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

#include "aaaniminstance.h"
#include "aamesh.h"
#include "aaniagara.h"
#include "aasound.h"
#include "aatexture.h"

#include <fstream>

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
void DB_Manager::Init()
{
    // Project Folder
    myProjectContentFolderPath = "D:/Documents/Unreal/Anima_OLD/Content/";

    // Enums (TODO : remove to instead add automatic creation from loading file)
    AddEnum(Enumerator("Type", {"SOUL", "FIRE", "WATER", "GRASS", "GROUND","THUNDER","WIND"},
                       {QColorConstants::Gray, QColorConstants::Red, QColorConstants::Blue,
                       QColorConstants::DarkGreen, QColorConstants::DarkMagenta, QColorConstants::DarkYellow, QColorConstants::DarkCyan}));
    AddEnum(Enumerator("MoveCategory", {"PHYSIC","SPECIAL","SUPPORT"}));

    // String Tables;
    AddStringTablePrivate("Ceribou");
    AddStringTablePrivate("Shitty Stuff");
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

    // Setup template
    TemplateStructure templ1 = TemplateStructure("Struct Test", QColorConstants::DarkRed);
    templ1.AddAttributeTemplate(AttributeTypeHelper::Type::Texture, "Texture", AttributeParam());
    templ1.AddAttributeTemplate(AttributeTypeHelper::Type::Sound, "Sound", AttributeParam());
    templ1.AddAttributeTemplate(AttributeTypeHelper::Type::Bool, "Bool", AttributeParam());
    AttributeParam tempEnumParam = AttributeParam();
    tempEnumParam.enumeratorIndex = 0;
    templ1.AddAttributeTemplate(AttributeTypeHelper::Type::Enum, "Enum", tempEnumParam);          // Enum requires that the param has a non null enum ptr
    templ1.SetAttributeDefaultValue("Enum", "GROUND");
    templ1.AddAttributeTemplate(AttributeTypeHelper::Type::TableString, "Table", AttributeParam());

    AddStructureDB(templ1);


    TemplateStructure templ2 = TemplateStructure("Another Struct", QColorConstants::DarkBlue);
    templ2.AddAttributeTemplate(AttributeTypeHelper::Type::Int, "Int", AttributeParam());             // Int use the param given but default are ok
    templ2.AddAttributeTemplate(AttributeTypeHelper::Type::Float, "Float", AttributeParam());         // Same
    templ2.AddAttributeTemplate(AttributeTypeHelper::Type::ShortString, "Short", AttributeParam());   // Same
    AttributeParam tempRefParam = AttributeParam();
    tempRefParam.structTable = GetStructureTable(0);
    templ2.AddAttributeTemplate(AttributeTypeHelper::Type::Reference, "Ref", tempRefParam);

    AddStructureDB(templ2);


    StructureDB* db1 = GetStructureTable(0);
    db1->AddStructureAt(0);
    db1->AddStructureAt(0);
    db1->AddStructureAt(0);

    StructureDB* db2 = GetStructureTable(1);
    db2->AddStructureAt(0);
    db2->AddStructureAt(0);

    emit StructItemChanged(0);
    emit StructItemChanged(1);


/*
TemplateStructure* templ = new TemplateStructure("Test2StructDB", QColorConstants::Red);

//templ->AddAttribute("Mesh", new AAMesh());
templ->AddAttribute("Texture", new AATexture());
//templ->AddAttribute("Sound", new AASound());
//templ->AddAttribute("Anim", new AAAnimInstance());
//templ->AddAttribute("Niagara", new AANiagara());
templ->AddAttribute("Bool", new ABool());
AttributeParam* enumParam = new AttributeParam();
enumParam->enumerator = &enumerators[0];
templ->AddAttribute("Enum", new AEnumerator(enumParam, 4));
AttributeParam* floatParam = new AttributeParam();
AFloat* floatAttTempl = new AFloat(floatParam);
floatAttTempl->SetValueFromText("0.253");
templ->AddAttribute("Float", floatAttTempl);
AttributeParam* intParam = new AttributeParam();
templ->AddAttribute("Int", new AInt(intParam));
AttributeParam* stringParam = new AttributeParam();
stringParam->max_i = 32;
templ->AddAttribute("Short String", new AShortString(stringParam, "Hello There !!!"));
templ->AddAttribute("Table String", new ATableString());

TemplateStructure* templ2 = new TemplateStructure("Test_Strct", QColorConstants::Red);
AStructure* structAtt = new AStructure(*templ);
templ2->AddAttribute("Struct", structAtt);
AttributeParam* arrayParam = new AttributeParam();
arrayParam->templateAtt = floatAttTempl->CreateDuplica();
AArray* arrayAtt = new AArray(arrayParam);
arrayAtt->AddRow();
arrayAtt->AddRow();
templ2->AddAttribute("Array", arrayAtt);

TemplateStructure* templ3 = new TemplateStructure("Test_Combine", QColorConstants::Red);
AttributeParam* arrayParam2 = new AttributeParam();
arrayParam2->templateAtt = structAtt->CreateDuplica();
AArray* arrayAtt2 = new AArray(arrayParam2);
arrayAtt2->AddRow();
arrayAtt2->AddRow();
templ3->AddAttribute("ArrayOfStruct", arrayAtt2);

TemplateStructure* templ4 = new TemplateStructure("Test_Combine", QColorConstants::Red);
AStructure* structAtt2 = new AStructure(*templ2);
templ4->AddAttribute("StructOfStructAndArray", structAtt2);


AddStructures(*templ);
AddStructures(*templ2);
AddStructures(*templ3);
AddStructures(*templ4);

StructureDB* db1 = GetStructures(0);

//Ref in struct 1
AttributeParam* refParam = new AttributeParam();
refParam->structTable = GetStructures(1);
templ->AddAttribute("Ref", new AReference(refParam));

db1->AddStructureAt(0);
db1->AddStructureAt(0);
db1->AddStructureAt(0);
*/


//#define fileExportTest
#ifdef fileExportTest
    // Create structure
    Structure structure = Structure(*templ);
    Structure structure1 = Structure(*templ);
    structure1.SetAttributeValueFromText("Bool", "tRuEe");
    structure1.SetAttributeValueFromText("Enum", "FIREe");
    structure1.SetAttributeValueFromText("Float", "15.623665l");
    structure1.SetAttributeValueFromText("Int", "10.1");
    structure1.SetAttributeValueFromText("String", "General Kenobi...");
    Structure structure20 = Structure(*templ2);
    Structure structure2 = Structure(*templ2);
    structure2.SetAttributeValueFromText("Struct", "{true,WATER,5.86,90,Meh}");
    structure2.SetAttributeValueFromText("Array", "[3.0,5.36]");
    Structure structure30 = Structure(*templ3);
    Structure structure3 = Structure(*templ3);
    structure3.SetAttributeValueFromText("ArrayOfStruct", "[{true,FIRE,5.8886,53,hé},{false,Wind,2.33333,3,oh}]");
    Structure structure40 = Structure(*templ4);
    Structure structure4 = Structure(*templ4);
    structure4.SetAttributeValueFromText("StructOfStructAndArray", "{{true,FIRE,5.8886,53,hé},[23.5,63.6]}");

    // File handling
    std::ofstream file;
    file.open("Test_File_1.csv");
    structure.WriteValue_CSV_AsRow(file, 0);
    structure1.WriteValue_CSV_AsRow(file, 1);
    file << "\n";
    structure20.WriteValue_CSV_AsRow(file, 2);
    structure2.WriteValue_CSV_AsRow(file, 2);
    file << "\n";
    structure30.WriteValue_CSV_AsRow(file, 3);
    structure3.WriteValue_CSV_AsRow(file, 3);
    file << "\n";
    structure40.WriteValue_CSV_AsRow(file, 4);
    structure4.WriteValue_CSV_AsRow(file, 4);
    file.close();

    //QString testStr = structure4.GetAttribute("StructOfStructAndArray")->GetDisplayedText(true);
#else

#endif

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
void DB_Manager::RemoveEnum(int _index)
{
    if (_index < 0 || _index >= (int)enumerators.size())
        return;

    for (auto& attrParamPtr : myAttributeParamPtrs)
    {
        if (attrParamPtr->enumeratorIndex == _index)
            attrParamPtr->enumeratorIndex = -1;
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
void DB_Manager::RemoveStructureDB(int _index, bool forDelete)
{
    const int count = myStructures.count();
    if (_index < 0 || _index > count)
        return;


    if (!forDelete)
    {
        emit StructTableRemoved(_index);
    }

    auto* structDB = myStructures.takeAt(_index);
    delete structDB;
}
void DB_Manager::RemoveStructureDB(const QString& _tableName, bool forDelete)
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
    RemoveStructureDB(index, forDelete);
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
void DB_Manager::AddAttribute(int _tableIndex, int _attrIndex, bool _copyFromPrevious)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->AddAttribute(_attrIndex, _copyFromPrevious);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::AddAttribute(const QString& _tableName, int _attrIndex, bool _copyFromPrevious)
{
    AddAttribute(GetStructureTableIndex(_tableName), _attrIndex, _copyFromPrevious);
}
void DB_Manager::RemoveAttribute(int _tableIndex, int _attrIndex)
{
    const int count = myStructures.count();
    if (_tableIndex < 0 || _tableIndex > count)
        return;

    myStructures[_tableIndex]->RemoveAttribute(_attrIndex);
    emit StructItemChanged(_tableIndex);
}
void DB_Manager::RemoveAttribute(const QString& _tableName, int _attrIndex)
{
    RemoveAttribute(GetStructureTableIndex(_tableName), _attrIndex);
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
void DB_Manager::AddStringTablePrivate(const QString& _newTableName, int _index)
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
        return "<font color=\"darkred\">!!! ERROR !!!</font>";

    const QString* myStr = GetStringTable(_tableId)->GetString(_stringId, SStringHelper::SStringLanguages::French);
    if (!myStr)
        return "<font color=\"darkyellow\">??? UNSET ??? </font>";

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
