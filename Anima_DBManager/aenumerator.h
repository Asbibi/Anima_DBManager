#ifndef AENUMERATOR_H
#define AENUMERATOR_H

#include "attribute.h"
#include "enumerator.h"

class AEnumerator : public Attribute
{
private:
    const Enumerator* enumerator;
    int value_index;

public:
    AEnumerator(const Enumerator* _enumerator, int _valueIndex = 0);

    virtual Type GetType() const override { return Type::Enum; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;
};

#endif // AENUMERATOR_H
