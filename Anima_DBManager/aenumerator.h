#ifndef AENUMERATOR_H
#define AENUMERATOR_H

#include "attribute.h"
#include "enumerator.h"

class AEnumerator : public Attribute
{
public:
    const Enumerator* enumerator;
    int value_index;

    AEnumerator(const Enumerator* _enumerator, int _valueIndex);

    virtual Attribute* CreateDuplica()              const override;
    virtual QString GetDisplayedText()              const override;
    virtual void WriteValue_CSV(std::ofstream& file) const override;

};

#endif // AENUMERATOR_H
