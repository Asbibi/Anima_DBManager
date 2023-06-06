#ifndef ABOOL_H
#define ABOOL_H

#include "attribute.h"

class ABool : public Attribute
{
private:
    bool value;

public:
    ABool(const AttributeParam& _sharedParam);
    ABool(const AttributeParam& _sharedParam, bool _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Bool; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;


    bool GetValue() const {return value;};
    void SetValue(bool _value);
};

#endif // ABOOL_H
