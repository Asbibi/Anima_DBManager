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
    AEnumerator(const AttributeParam& _sharedParam, int _valueIndex = 0);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Enum; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValueAsText()                            const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    const Enumerator*   GetEnum()       const { return mySharedParam.GetEnum(); }
    int                 GetEnumValue()  const { return value_index; }
    void                SetEnumValue(int _valueIndex);
};

#endif // AENUMERATOR_H
