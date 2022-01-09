#include "db_manager.h"

#include "aarray.h"
#include "aenumerator.h"
#include "afloat.h"
#include "aint.h"
#include "astring.h"
#include "astructure.h"
#include <fstream>

DB_Manager::DB_Manager()
{
    // Enums (TODO : remove to instead add automatic creation from loading file)
    AddEnum(Enumerator("Type", {"SOUL", "FIRE", "WATER", "GRASS", "GROUND","THUNDER","WIND"}));
    AddEnum(Enumerator("MoveCategory", {"PHYSIC","SPECIAL","SUPPORT"}));

    // Setup template
    StructureTemplate templ = StructureTemplate("Test", QColorConstants::Red);

    templ.AddAttribute("Enum", new AEnumerator(&enumerators[0], 4));
    AttributeParam floatParam = AttributeParam();
    AFloat* floatAttTempl = new AFloat(&floatParam);
    floatAttTempl->value = 0.253f;
    templ.AddAttribute("Float", floatAttTempl);
    AttributeParam intParam = AttributeParam();
    templ.AddAttribute("Int", new AInt(&intParam));
    AttributeParam stringParam = AttributeParam();
    stringParam.max_i = 32;
    templ.AddAttribute("String", new AString(&stringParam, "Hello There !!!"));

    StructureTemplate templ2 = StructureTemplate("Test_Strct", QColorConstants::Red);
    AStructure* structAtt = new AStructure(templ);
    templ2.AddAttribute("Struct", structAtt);
    AttributeParam arrayParam = AttributeParam();
    arrayParam.templateAtt = floatAttTempl->CreateDuplica();
    AArray* arrayAtt = new AArray(&arrayParam);
    arrayAtt->AddRow();
    arrayAtt->AddRow();
    templ2.AddAttribute("Array", arrayAtt);

    StructureTemplate templ3 = StructureTemplate("Test_Combine", QColorConstants::Red);
    AttributeParam arrayParam2 = AttributeParam();
    arrayParam2.templateAtt = structAtt->CreateDuplica();
    AArray* arrayAtt2 = new AArray(&arrayParam2);
    arrayAtt2->AddRow();
    arrayAtt2->AddRow();
    templ3.AddAttribute("ArrayOfStruct", arrayAtt2);

    StructureTemplate templ4 = StructureTemplate("Test_Combine", QColorConstants::Red);
    AStructure* structAtt2 = new AStructure(templ2);
    templ4.AddAttribute("StructOfStructAndArray", structAtt2);

    // Create structure
    Structure structure = Structure(templ);
    Structure structure2 = Structure(templ2);
    Structure structure3 = Structure(templ3);
    Structure structure4 = Structure(templ4);

    // File handling
    std::ofstream file;
    file.open("Test_File_1.csv");
    structure.WriteValue_CSV_AsRow(file, 0);
    structure2.WriteValue_CSV_AsRow(file, 2);
    structure3.WriteValue_CSV_AsRow(file, 3);
    structure4.WriteValue_CSV_AsRow(file, 4);
    file.close();
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
