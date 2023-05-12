#ifndef ATTRIBUTEPARAM_H
#define ATTRIBUTEPARAM_H

#include <QPointer>

class TemplateAttribute;
class StructureDB;
class Enumerator;

struct AttributeParam
{
    AttributeParam();
    AttributeParam(const AttributeParam& _another);
    AttributeParam(const QString& _csvString);
    ~AttributeParam();
    void operator=(const AttributeParam& _another);

    bool ignoreMin = true;
    bool ignoreMax = true;
    int min_i = 0;
    int max_i = -1;                                 // Max are inclusive for int,
                                                    // exclusive for string char count
                                                    // and arrays in general
    float min_f = 0;
    float max_f = -1;                               // Max are inclusive for float
    TemplateAttribute* templateAtt = nullptr;       // Used for AArray only;
    StructureDB* structTable = nullptr;             // Used for AReference only;

    // Only for Enum Attributes
    int enumeratorIndex = -1;      // Used for AEnumerator only;    When changed, need to update all enum attribute, template attribute & qtemplateattribute
    const Enumerator* GetEnum() const;

    void SaveParams_CSV(std::ofstream& file) const;
};

#endif // ATTRIBUTEPARAM_H
