#ifndef ATABLESTRING_H
#define ATABLESTRING_H

#include "attribute.h"

class ATableString : public Attribute
{
private:
    static const QString tableString;
    static const QString identifierString;

    QString myTableName;
    QString myStringIdentifier;

public:
    ATableString(TemplateAttribute& _template);
    ATableString(TemplateAttribute& _template, const QString& _tableName, const QString& _stringIdentifier);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::TableString; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValue_CSV()                              const override;
    virtual QJsonValue GetValue_JSON()                          const override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)              override;
    virtual void SetValue_CSV(const QString& _text)                   override;

    bool HasValidValues() const;
    const QString& GetTableName() const;
    const QString& GetStringIdentifier() const;

    static QJsonValue ConvertToJsonValue(const QString& _tableId, const QString& _stringId);
};

#endif // ATABLESTRING_H
