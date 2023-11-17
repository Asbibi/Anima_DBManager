#ifndef ABOOL_H
#define ABOOL_H

#include "attribute.h"

class ABool : public Attribute
{
private:
    bool value;

public:
    ABool(TemplateAttribute& _template);
    ABool(TemplateAttribute& _template, bool _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Bool; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValue_CSV()                              const override;
    virtual QJsonValue GetValue_JSON()                          const override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)              override;
    virtual void SetValue_CSV(const QString& _text)                   override;

    bool GetValue() const {return value;};
    void SetValue(bool _value);
};

#endif // ABOOL_H
