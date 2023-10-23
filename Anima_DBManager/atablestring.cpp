#include "atablestring.h"

#include "db_manager.h"

ATableString::ATableString(TemplateAttribute& _template) :
    Attribute(_template)
{}
ATableString::ATableString(TemplateAttribute& _template, const QString& _tableName, const QString& _stringIdentifier):
    Attribute(_template),
    myTableName(_tableName),
    myStringIdentifier(_stringIdentifier)
{}


QString ATableString::GetDisplayedText() const
{
    auto& DB = DB_Manager::GetDB_Manager();
    return DB.GetStringForDisplay(myTableName, myStringIdentifier, false);
}
QString ATableString::GetValueAsText() const
{
    return '#' + myTableName + '#' + myStringIdentifier;
}
QString ATableString::GetAttributeAsCSV() const
{
    return "(myTable=" + myTableName + ",myKey=\"" + myStringIdentifier+ "\")";
}
QJsonValue ATableString::GetAttributeAsJSON() const
{
    QJsonObject stringRefAsJSON = QJsonObject();

    stringRefAsJSON.insert("myTable", myTableName);
    stringRefAsJSON.insert("myKey", myStringIdentifier);

    return QJsonValue(stringRefAsJSON);
}
void ATableString::SetValueFromText(const QString& text)
{
    if (text.isEmpty() || text == "##")
    {
        myTableName = "";
        myStringIdentifier = "";
        return;
    }

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
void ATableString::ReadValue_CSV(const QString& _text)
{
    QString textCopy = _text;
    textCopy.replace("(myTable=", "#");
    textCopy.replace(",myKey=\"", "#");
    int textLength = textCopy.length();
    textCopy.remove(textLength - 2, 2);
    SetValueFromText(textCopy);
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
