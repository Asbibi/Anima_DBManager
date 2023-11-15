#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QString>

class  Attribute;


namespace JsonHelper {

QJsonArray ConvertAttributeListToJsonArray(const QList<Attribute*>& _attributes);
QJsonObject ConvertAttributeListToJsonObject(const QList<Attribute*>& _attributes);

QString JsonToString(const QJsonArray& _json);
QString JsonToString(const QJsonObject& _json);
QString JsonToString(const QJsonValue& _json);

}

#endif // JSONHELPER_H
