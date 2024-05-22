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

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::ShortString; }
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValue_CSV()                              const override;
    virtual QJsonValue GetValue_JSON()                          const override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)              override;
    virtual void SetValue_CSV(const QString& _text)                   override;

    bool HasValidSize() const;
    QString GetFullText() const;
};

#endif // ASHORTSTRING_H
