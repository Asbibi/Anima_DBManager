#include "jsonhelper.h"

#include "attribute.h"
#include "templateattribute.h"
#include "db_manager.h"

#include <QJsonDocument>


namespace JsonHelper {


QJsonArray ConvertAttributeListToJsonArray(const QList<Attribute*>& _attributes)
{
    QJsonArray retVal = QJsonArray();
    for (const auto* attr : _attributes)
    {
        retVal.append(attr->GetValue_JSON());
    }
    return retVal;
}
QJsonObject ConvertAttributeListToJsonObject(const QList<Attribute*>& _attributes)
{
    const auto& dbManager = DB_Manager::GetDB_Manager();
    QJsonObject retVal = QJsonObject();
    for (const auto* attr : _attributes)
    {
        retVal.insert(dbManager.GetAttributeFullName(attr->GetTemplate()->GetName()), attr->GetValue_JSON());
    }
    return retVal;
}


QString JsonToString(const QJsonArray& _json)
{
    return QJsonDocument(_json).toJson(QJsonDocument::Compact);
}
QString JsonToString(const QJsonObject& _json)
{
    return QJsonDocument(_json).toJson(QJsonDocument::Compact);
}
QString JsonToString(const QJsonValue& _json)
{
    Q_ASSERT(!_json.isNull());
    const int extraCharCount = _json.isString() ? 2 : 1;

    QJsonArray wrapper = QJsonArray();
    wrapper.append(_json);
    QString retVal = QJsonDocument(wrapper).toJson(QJsonDocument::Compact);
    retVal.remove(0, extraCharCount);
    retVal.remove(retVal.length() - extraCharCount, extraCharCount);
    return retVal;
}

}
