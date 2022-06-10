#ifndef ATTRIBUTEPARAM_H
#define ATTRIBUTEPARAM_H

#include "enumerator.h"
#include <QPointer>

class Attribute;
class StructureDB;
struct AttributeParam
{
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
    StructureDB* structTable = nullptr;    // Used for AReference only;         //QPointer<>
    QPointer<Enumerator> enumerator = nullptr;      // Used for AEnumerator only;
};

#endif // ATTRIBUTEPARAM_H
