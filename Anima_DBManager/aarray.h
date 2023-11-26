#ifndef AARRAY_H
#define AARRAY_H

#include "attribute.h"

class AArray : public Attribute
{
private:
    QList<Attribute*> myValues;

    void SetCount(int _count);

public:
    AArray(TemplateAttribute& _template);
    AArray(TemplateAttribute& _template, const QList<Attribute*>& _values);
    ~AArray();

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Array; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValue_CSV()                              const override;
    virtual QJsonValue GetValue_JSON()                          const override;
    virtual void CopyValueFromOther(const Attribute* _other)         override;
    virtual bool SetValue_JSON(const QJsonValue& _value)             override;
    virtual void SetValue_CSV(const QString& text)                   override;

    TemplateAttribute* GetArrayElementTemplate() const;
    const QList<Attribute*>& GetAttributes() const;
    void AddRow(int _index);
    void DuplicateRow(int _index);
    void RemoveRow(int _index);
    void Empty(bool _clean = true);
    void MoveRow(int _originalIndex, int _targetIndex);

    static QString GetShortDisplayedString(int _count);
};

#endif // AARRAY_H
