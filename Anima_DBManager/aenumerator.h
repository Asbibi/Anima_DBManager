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
    AEnumerator(TemplateAttribute& _template, int _valueIndex = 0);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Enum; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValueAsText()                            const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual QJsonValue GetAttributeAsJSON()                     const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool ReadValue_JSON(const QJsonValue& _value)             override;

    const Enumerator*   GetEnum()       const;
    int                 GetEnumValue()  const;
    void                SetEnumValue(int _valueIndex);
};

#endif // AENUMERATOR_H
