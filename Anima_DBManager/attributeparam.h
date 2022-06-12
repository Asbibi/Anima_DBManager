#ifndef ATTRIBUTEPARAM_H
#define ATTRIBUTEPARAM_H

#include <QPointer>

class Attribute;
class StructureDB;
class Enumerator;

struct AttributeParam
{
    AttributeParam();
    AttributeParam(const AttributeParam& _another);
    ~AttributeParam();

    bool ignoreMin = true;
    bool ignoreMax = true;
    int min_i = 0;
    int max_i = -1;                                 // Max are inclusive for int,
                                                    // exclusive for string char count
                                                    // and arrays in general
    float min_f = 0;
    float max_f = -1;                               // Max are inclusive for float
    Attribute* templateAtt = nullptr;               // Used for AArray only;
    StructureDB* structTable = nullptr;             // Used for AReference only;         //QPointer<>

    // Only for Enum Attributes
    int enumeratorIndex = -1;      // Used for AEnumerator only;
    const Enumerator* GetEnum() const;
};

#endif // ATTRIBUTEPARAM_H
