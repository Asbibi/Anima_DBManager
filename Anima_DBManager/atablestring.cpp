#include "atablestring.h"

#include "db_manager.h"

ATableString::ATableString() :
    Attribute(nullptr)
{}

Attribute* ATableString::CreateDuplica() const
{
    auto* duplica = new ATableString();
    duplica->myTableName = myTableName;
    duplica->myStringIdentifier = myStringIdentifier;
    return duplica;
}
QString ATableString::GetDisplayedText(bool complete) const
{
    auto& DB = DB_Manager::GetDB_Manager();
    return DB.GetStringForDisplay(myTableName, myStringIdentifier, complete);
}
void ATableString::WriteValue_CSV(std::ofstream& file) const
{
    qFatal("TODO : implement CSV exporter for Table Strings");
}
void ATableString::SetValueFromText(const QString& text)
{
    QString tableId = text.section('#', 1,1);
    QString stringId = text.section('#', 2,2);
    if (text != ("#" + tableId + "#" + stringId) || stringId.contains('#'))
    {
        qFatal("\n\nInvalid string while setting <TABLE_STRING> attribute's value:\n\n\t===== Abort =====\n\n");
        return;
    }

   QString oldTableId = myTableName;
   QString oldStringId = myStringIdentifier;
   myTableName = tableId;
   myStringIdentifier = stringId;


   if ((myTableName != oldTableId || myStringIdentifier != oldStringId ) && HasValidValues())
   {
       EmitValueChanged();
   }
   else
   {
       // Abort Change
       myTableName = oldTableId;
       myStringIdentifier = oldStringId;
   }
}

bool ATableString::HasValidValues() const
{
    return DB_Manager::GetDB_Manager().AreValidIdentifiers(myTableName, myStringIdentifier);
}

const QString& ATableString::GetTableName() const
{
    return myTableName;
}

const QString& ATableString::GetStringIdentifier() const
{
    return myStringIdentifier;
}
