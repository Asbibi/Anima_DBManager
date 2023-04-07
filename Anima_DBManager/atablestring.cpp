#include "atablestring.h"

#include "db_manager.h"

ATableString::ATableString(const AttributeParam& _sharedParam) :
    Attribute(_sharedParam)
{}
ATableString::ATableString(const AttributeParam& _sharedParam, const QString& _tableName, const QString& _stringIdentifier):
    Attribute(_sharedParam),
    myTableName(_tableName),
    myStringIdentifier(_stringIdentifier)
{}

Attribute* ATableString::CreateDuplica() const
{
    return new ATableString(mySharedParam, myTableName, myStringIdentifier);
}
QString ATableString::GetDisplayedText(bool complete) const
{
    auto& DB = DB_Manager::GetDB_Manager();
    return DB.GetStringForDisplay(myTableName, myStringIdentifier, complete);
}
void ATableString::WriteValue_CSV(std::ofstream& file) const
{
    file << "(myTable=" << myTableName.toStdString() << ",myKey=\"" << myStringIdentifier.toStdString() << "\")";
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
void ATableString::CopyValueFromOther(const Attribute* _other)
{
    const ATableString* other_ATS = dynamic_cast<const ATableString*>(_other);
    if (!other_ATS)
        return;

    myTableName = other_ATS->myTableName;
    myStringIdentifier = other_ATS->myStringIdentifier;
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
