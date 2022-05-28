#include "db_manager.h"

#include "aarray.h"
#include "abool.h"
#include "aenumerator.h"
#include "afloat.h"
#include "aint.h"
#include "ashortstring.h"
#include "astructure.h"
#include "atablestring.h"
#include <fstream>

#include <qdebug.h>

DB_Manager::DB_Manager()
{
    // Enums (TODO : remove to instead add automatic creation from loading file)
    AddEnum(Enumerator("Type", {"SOUL", "FIRE", "WATER", "GRASS", "GROUND","THUNDER","WIND"},
                       {QColorConstants::Gray, QColorConstants::Red, QColorConstants::Blue,
                       QColorConstants::DarkGreen, QColorConstants::DarkMagenta, QColorConstants::DarkYellow, QColorConstants::DarkCyan}));
    AddEnum(Enumerator("MoveCategory", {"PHYSIC","SPECIAL","SUPPORT"}));

    // String Tables;
    AddStringTable("Ceribou");
    AddStringTable("Shitty Stuff");
    QString text1s[] = {"Ceribou (anglais : Cherubi ; japonais : チェリンボ Cherinbo) est un Pokémon de type Plante de la quatrième génération.",
                        "Cherubi (Japanese: チェリンボ Cherinbo) is a Grass-type Pokémon introduced in Generation IV. It evolves into Cherrim starting at level 25."};
    QString text2s[] = {"Palkia shiny de ses morts", "Fucka youuuuuu"};
    QString text3s[] = {"Les grandes apparitions de ce Pokémon remontent à son utilisation par Flo dans les DP031 et 37.\nOn peut noter l'apparition d'un Ceribou dans le film Pokémon : L'ascension de Darkrai.\nOn reverra très peu (ou très rapidement) ce Pokémon par la suite (DP066 et 88), jusqu'à son utilisation par Maryline dans le DP125 : en effet, cette dernière juge Ceribou faisant partie des Pokémon mignons.",
                        "Cherubi debuted in The Grass-type Is Always Greener!, under the ownership of Gardenia.\nIt was first used in a battle against Ash. It faced off against Turtwig, and during the battle, Gardenia was able to learn quite a bit about Turtwig.\nCherubi reappeared in The Grass Menagerie!, where it once again fought Turtwig during an official Gym battle.\nIts speed forced Ash to recall Turtwig for Staravia, after which Cherubi was defeated."};
    QString id = "Palkia";
    QString text4s[] = {"He ho hohohoh", "ah ahaha ahiiiii"};
    QString id2 = "7 nains";
    myStringTables[0].AddStringItemWithTexts(text1s);
    myStringTables[0].AddStringItemWithTexts(text2s, &id);
    myStringTables[0].AddStringItemWithTexts(text3s);
    myStringTables[1].AddStringItem();
    myStringTables[1].AddStringItem(&id);
    myStringTables[1].AddStringItemWithTexts(text4s, &id2);

    // Setup template
    StructureTemplate* templ = new StructureTemplate("Test2StructDB", QColorConstants::Red);

    templ->AddAttribute("Bool", new ABool());
    templ->AddAttribute("Enum", new AEnumerator(&enumerators[0], 4));
    AttributeParam floatParam = AttributeParam();
    AFloat* floatAttTempl = new AFloat(&floatParam);
    floatAttTempl->SetValueFromText("0.253");
    templ->AddAttribute("Float", floatAttTempl);
    AttributeParam intParam = AttributeParam();
    templ->AddAttribute("Int", new AInt(&intParam));
    AttributeParam stringParam = AttributeParam();
    stringParam.max_i = 32;
    templ->AddAttribute("Short String", new AShortString(&stringParam, "Hello There !!!"));
    templ->AddAttribute("Table String", new ATableString());

    StructureTemplate* templ2 = new StructureTemplate("Test_Strct", QColorConstants::Red);
    AStructure* structAtt = new AStructure(*templ);
    templ2->AddAttribute("Struct", structAtt);
    AttributeParam arrayParam = AttributeParam();
    arrayParam.templateAtt = floatAttTempl->CreateDuplica();
    AArray* arrayAtt = new AArray(&arrayParam);
    arrayAtt->AddRow();
    arrayAtt->AddRow();
    templ2->AddAttribute("Array", arrayAtt);

    StructureTemplate* templ3 = new StructureTemplate("Test_Combine", QColorConstants::Red);
    AttributeParam arrayParam2 = AttributeParam();
    arrayParam2.templateAtt = structAtt->CreateDuplica();
    AArray* arrayAtt2 = new AArray(&arrayParam2);
    arrayAtt2->AddRow();
    arrayAtt2->AddRow();
    templ3->AddAttribute("ArrayOfStruct", arrayAtt2);

    StructureTemplate* templ4 = new StructureTemplate("Test_Combine", QColorConstants::Red);
    AStructure* structAtt2 = new AStructure(*templ2);
    templ4->AddAttribute("StructOfStructAndArray", structAtt2);


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
    AddStructures(*templ);
    AddStructures(*templ2);
    AddStructures(*templ3);
    AddStructures(*templ4);

    StructureDB* db1 = GetStructures(0);
    db1->AddStructureAt(0);
    db1->AddStructureAt(0);
    db1->AddStructureAt(0);
#endif
}
DB_Manager::~DB_Manager()
{
    for(auto const& strct : myStructures)
        delete(strct);
}
DB_Manager& DB_Manager::GetDB_Manager()
{
    static DB_Manager singleton = DB_Manager();
    return singleton;
}

int DB_Manager::GetEnumCount() const
{
    return (int)enumerators.size();
}
const Enumerator* DB_Manager::GetEnum(int _index) const
{
    if (_index < 0 || _index >= enumerators.size())
        return nullptr;

    return &enumerators[_index];
}
void DB_Manager::AddEnum(const Enumerator& _enum)
{
    enumerators.push_back(_enum);
}
void DB_Manager::RemoveEnum(int _index)
{
    if (_index < 0 || _index >= enumerators.size())
        return;

    enumerators.erase(enumerators.begin() + _index);
}


int DB_Manager::GetStructuresCount() const
{
    return (int)(myStructures.size());
}
const StructureDB* DB_Manager::GetStructures(int index) const
{
    if (index < 0)
        index = 0;
    else if (index >= GetStructuresCount())
        index = GetStructuresCount() -1;

    return myStructures[index];
}
StructureDB* DB_Manager::GetStructures(int index)
{
    if (index < 0)
        index = 0;
    else if (index >= GetStructuresCount())
        index = GetStructuresCount() -1;

    return myStructures[index];
}
void DB_Manager::AddStructures(const StructureTemplate& _structureTemplate)
{
    myStructures.push_back(new StructureDB(_structureTemplate));
}



int DB_Manager::GetIndexFromStringTableName(const QString& _tableName) const
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
    return GetStringTable(GetIndexFromStringTableName(_tableName));
}
void DB_Manager::AddStringTable(const QString& _newTableName)
{
    if (GetIndexFromStringTableName(_newTableName) == -1)
        myStringTables.push_back(SStringTable(_newTableName));
}
void DB_Manager::RemoveStringTable(int _index)
{
    if (_index < 0 || _index >= GetStringTableCount())
        return;

    myStringTables.erase(myStringTables.begin() + _index);
}
void DB_Manager::RemoveStringTable(const QString& _tableName)
{
    RemoveStringTable(GetIndexFromStringTableName(_tableName));
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
