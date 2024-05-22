#ifndef AENUMERATOR_H
#define AENUMERATOR_H

#include "attribute.h"
#include "enumerator.h"

class AEnumerator : public Attribute
{
private:
    int value_index;
    bool CheckEnumIsValid() const;
    void SetValueFromText(const QString& _text);

public:
    AEnumerator(TemplateAttribute& _template, int _valueIndex = 0);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Enum; }
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValue_CSV()                              const override;
    virtual QJsonValue GetValue_JSON()                          const override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)              override;
    virtual void SetValue_CSV(const QString& _text)                   override;

    const Enumerator*   GetEnum()       const;
    int                 GetEnumValue()  const;
    void                SetEnumValue(int _valueIndex);
};

#endif // AENUMERATOR_H
