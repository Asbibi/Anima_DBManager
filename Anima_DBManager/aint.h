#ifndef AINT_H
#define AINT_H

#include "attribute.h"

class AInt : public Attribute
{
private:
    int value;

public:
    AInt(const AttributeParam& _sharedParam);
    AInt(const AttributeParam& _sharedParam, int _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Int; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    bool FitsMinParam() const;
    bool FitsMaxParam() const;
    int GetValue(bool _validated = true) const;

    int GetMax(bool& useIt) const;
    int GetMin(bool& useIt) const;
};

#endif // AINT_H
