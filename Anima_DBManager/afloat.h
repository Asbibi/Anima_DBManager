#ifndef AFLOAT_H
#define AFLOAT_H

#include "attribute.h"

class AFloat : public Attribute
{
public:
    float value;

    AFloat(const AttributeParam* _sharedParam);
    AFloat(const AttributeParam* _sharedParam, float _value);

    virtual Attribute* CreateDuplica()              const override;
    virtual QString GetDisplayedText()              const override;
    virtual void WriteValue_CSV(std::ofstream& file) const override;

    bool FitsMinParam() const;
    bool FitsMaxParam() const;
    float GetValidValue() const;
};

#endif // AFLOAT_H
