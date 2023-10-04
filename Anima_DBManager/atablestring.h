#ifndef ATABLESTRING_H
#define ATABLESTRING_H

#include "attribute.h"

class ATableString : public Attribute
{
private:
    QString myTableName;
    QString myStringIdentifier;

public:
    ATableString(TemplateAttribute& _template);
    ATableString(TemplateAttribute& _template, const QString& _tableName, const QString& _stringIdentifier);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::TableString; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValueAsText()                            const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual void ReadValue_CSV(const QString& _text)                   override;

    bool HasValidValues() const;
    const QString& GetTableName() const;
    const QString& GetStringIdentifier() const;
};

#endif // ATABLESTRING_H
