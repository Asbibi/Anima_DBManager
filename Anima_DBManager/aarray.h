#ifndef AARRAY_H
#define AARRAY_H

#include "attribute.h"

class AArray : public Attribute
{
public:
    std::vector<Attribute*> values;

    AArray(const AttributeParam* _sharedParam);
    AArray(const AttributeParam* _sharedParam, const std::vector<Attribute*>* _values);
    ~AArray();

    virtual Attribute* CreateDuplica()              const override;
    virtual QString GetDisplayedText()              const override;
    virtual void WriteValue_CSV(std::ofstream& file) const override;

    std::vector<QString> GetDisplayedTexts() const;
    void AddRow();
    void RemoveRow(int _index);
};

#endif // AARRAY_H
