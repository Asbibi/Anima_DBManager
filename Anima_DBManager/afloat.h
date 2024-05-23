#ifndef AFLOAT_H
#define AFLOAT_H

#include "attribute.h"

class AFloat : public Attribute
{
private:
    float value;

public:
    AFloat(TemplateAttribute& _template);
    AFloat(TemplateAttribute& _template, float _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Float; }
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValue_CSV()                              const override;
    virtual QJsonValue GetValue_JSON()                          const override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)              override;
    virtual void SetValue_CSV(const QString& _text)                   override;

    bool FitsMinParam() const;
    bool FitsMaxParam() const;
    float GetValue(bool _validated = true) const;

    float GetMax(bool& useIt) const;
    float GetMin(bool& useIt) const;
};

#endif // AFLOAT_H
