#ifndef AENUMERATOR_H
#define AENUMERATOR_H

#include "attribute.h"
#include "enumerator.h"

class AEnumerator : public Attribute
{
private:
    int value_index;
    bool CheckEnumIsValid() const;

public:
    AEnumerator(const AttributeParam* _sharedParam, int _valueIndex = 0);

    virtual Type GetType() const override { return Type::Enum; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;

    const Enumerator*   GetEnum()       const { return CheckEnumIsValid() ? sharedParam->GetEnum() : nullptr; };
    int                 GetEnumValue()  const { return value_index; };
    void                SetEnumValue(int _valueIndex);
};

#endif // AENUMERATOR_H
