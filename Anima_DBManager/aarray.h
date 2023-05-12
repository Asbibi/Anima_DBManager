#ifndef AARRAY_H
#define AARRAY_H

#include "attribute.h"

class AArray : public Attribute
{
private:
    QList<Attribute*> myValues;

public:
    AArray(const AttributeParam& _sharedParam);
    AArray(const AttributeParam& _sharedParam, const QList<Attribute*>& _values);
    ~AArray();

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Array; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual void ReadValue_CSV(const QString& text)                   override;

    QStringList GetDisplayedTexts() const;
    void AddRow();
    void RemoveRow(int _index);
};

#endif // AARRAY_H
