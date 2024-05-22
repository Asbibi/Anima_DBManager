#ifndef AINT_H
#define AINT_H

#include "attribute.h"

class AInt : public Attribute
{
private:
    int value;

public:
    AInt(TemplateAttribute& _template);
    AInt(TemplateAttribute& _template, int _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Int; }
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValue_CSV()                              const override;
    virtual QJsonValue GetValue_JSON()                          const override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)              override;
    virtual void SetValue_CSV(const QString& _text)                   override;

    bool FitsMinParam() const;
    bool FitsMaxParam() const;
    int GetValue(bool _validated = true) const;

    int GetMax(bool& useIt) const;
    int GetMin(bool& useIt) const;
};

#endif // AINT_H
