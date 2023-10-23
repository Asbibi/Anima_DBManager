#ifndef ASHORTSTRING_H
#define ASHORTSTRING_H

#include "attribute.h"

class AShortString : public Attribute
{
private:
    QString value;

public:
    AShortString(TemplateAttribute& _template);
    AShortString(TemplateAttribute& _template, QString _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::ShortString; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValueAsText()                            const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual QJsonValue GetAttributeAsJSON()                     const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool ReadValue_JSON(const QJsonValue& _value)             override;

    bool HasValidSize() const;
    QString GetFullText() const;
};

#endif // ASHORTSTRING_H
