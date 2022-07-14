#ifndef AARRAY_H
#define AARRAY_H

#include "attribute.h"

class AArray : public Attribute
{
private:
    std::vector<Attribute*> values;

public:
    AArray(const AttributeParam& _sharedParam);
    AArray(const AttributeParam& _sharedParam, const std::vector<Attribute*>* _values);
    ~AArray();

    virtual Type GetType() const override { return Type::Array; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    std::vector<QString> GetDisplayedTexts() const;
    void AddRow();
    void RemoveRow(int _index);
};

#endif // AARRAY_H
