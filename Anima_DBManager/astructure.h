#ifndef ASTRUCTURE_H
#define ASTRUCTURE_H

#include "attribute.h"
#include "structure.h"

class AStructure : public Attribute
{
public:
    Structure* myValue;

    AStructure(const AttributeParam& _sharedParam);
    AStructure(const AttributeParam& _sharedParam, const Structure* _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Structure; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    QList<QString> GetDisplayedTexts() const;
    const QList<Attribute*>& GetAttributes() const;

    static QString GetDisplayTextFromAttributes(const QList<Attribute*>& _attributes);
};

#endif // ASTRUCTURE_H
