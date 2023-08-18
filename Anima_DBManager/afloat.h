#ifndef AFLOAT_H
#define AFLOAT_H

#include "attribute.h"

class AFloat : public Attribute
{
private:
    float value;

public:
    AFloat(const AttributeParam& _sharedParam);
    AFloat(const AttributeParam& _sharedParam, float _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Float; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValueAsText()                            const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    bool FitsMinParam() const;
    bool FitsMaxParam() const;
    float GetValue(bool _validated = true) const;

    float GetMax(bool& useIt) const;
    float GetMin(bool& useIt) const;
};

#endif // AFLOAT_H
