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
    virtual QString GetValueAsText()                            const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual QJsonValue GetAttributeAsJSON()                     const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;


    bool GetValue() const {return value;};
    void SetValue(bool _value);
};

#endif // ABOOL_H
