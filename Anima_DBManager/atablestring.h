#ifndef ATABLESTRING_H
#define ATABLESTRING_H

#include "attribute.h"

class ATableString : public Attribute
{
private:
    QString myTableName;
    QString myStringIdentifier;

public:
    ATableString(const AttributeParam& _sharedParam);
    ATableString(const AttributeParam& _sharedParam, const QString& _tableName, const QString& _stringIdentifier);

    virtual Type GetType() const override { return Type::TableString; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    bool HasValidValues() const;
    const QString& GetTableName() const;
    const QString& GetStringIdentifier() const;
};

#endif // ATABLESTRING_H
