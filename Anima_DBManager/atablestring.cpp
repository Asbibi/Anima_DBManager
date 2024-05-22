#include "atablestring.h"

#include "db_manager.h"


const QString ATableString::tableString = "myTable";
const QString ATableString::identifierString = "myKey";

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
QString ATableString::GetValue_CSV() const
{
    return QString("(%1=%3,%2=\"%4\")").arg(tableString, identifierString, myTableName, myStringIdentifier);
}
QJsonValue ATableString::GetValue_JSON() const
{
    return ConvertToJsonValue(myTableName, myStringIdentifier);
}
QString ATableString::GetValue_String() const
{
    return QString("('%1', %2)").arg(myStringIdentifier, myTableName);
}
void ATableString::CopyValueFromOther(const Attribute* _other)
{
    const ATableString* other_ATS = dynamic_cast<const ATableString*>(_other);
    if (!other_ATS)
        return;

    myTableName = other_ATS->myTableName;
    myStringIdentifier = other_ATS->myStringIdentifier;
}
bool ATableString::SetValue_JSON(const QJsonValue& _value)
{
    if (!_value.isObject())
    {
        return false;
    }

    QJsonObject valueAsObj = _value.toObject();
    const QJsonValue& tabKey = valueAsObj.value(tableString);
    const QJsonValue& strKey = valueAsObj.value(identifierString);

    if (!tabKey.isString() || !strKey.isString())
    {
        return false;
    }

    myTableName = tabKey.toString();
    myStringIdentifier = strKey.toString();

    return true;
}
void ATableString::SetValue_CSV(const QString& _text)
{
    // Text has format (myTable=...,myKey="...")
    // Need to clean it first
    QString textCopy = _text;
    textCopy.remove(QString("(%1=").arg(tableString));
    textCopy.remove(textCopy.length() - 2, 2);  //remove ending ") string

    // Separate the 2 values + finish cleaning
    auto splits = textCopy.split(QString(",%2=\"").arg(identifierString));
    Q_ASSERT(splits.size() == 2);

    // Assign values
    myTableName = splits[0];
    myStringIdentifier = splits[1];
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



QJsonValue ATableString::ConvertToJsonValue(const QString& _tableId, const QString& _stringId)
{
    QJsonObject stringRefAsJSON = QJsonObject();

    stringRefAsJSON.insert(tableString, _tableId);
    stringRefAsJSON.insert(identifierString, _stringId);

    return QJsonValue(stringRefAsJSON);
}
