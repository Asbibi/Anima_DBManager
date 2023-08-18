#ifndef ASHORTSTRING_H
#define ASHORTSTRING_H

#include "attribute.h"

class AShortString : public Attribute
{
private:
    QString value;

public:
    AShortString(const AttributeParam& _sharedParam);
    AShortString(const AttributeParam& _sharedParam, QString _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::ShortString; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValueAsText()                            const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    bool HasValidSize() const;
    QString GetFullText() const;
};

#endif // ASHORTSTRING_H
