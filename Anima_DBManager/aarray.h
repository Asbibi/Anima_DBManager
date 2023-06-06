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
    virtual QString GetAttributeAsCSV()                         const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual void ReadValue_CSV(const QString& text)                   override;

    TemplateAttribute* GetArrayElementTemplate() const;
    QStringList GetDisplayedTexts() const;
    const QList<Attribute*>& GetAttributes() const;
    void AddRow(int _index);
    void DuplicateRow(int _index);
    void RemoveRow(int _index);
    void MoveRow(int _originalIndex, int _targetIndex);

    static QString GetShortDisplayedString(int _count);
    static QString GetStructureStringFromList(const QStringList& _listString);
};

#endif // AARRAY_H
