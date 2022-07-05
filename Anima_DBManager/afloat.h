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

    virtual Type GetType() const override { return Type::Float; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;

    bool FitsMinParam() const;
    bool FitsMaxParam() const;
    float GetValue(bool _validated = true) const;

    float GetMax(bool& useIt) const;
    float GetMin(bool& useIt) const;
};

#endif // AFLOAT_H
