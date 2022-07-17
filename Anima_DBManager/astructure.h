#ifndef ASTRUCTURE_H
#define ASTRUCTURE_H

#include "attribute.h"
#include "structure.h"

class AStructure : public Attribute
{
public:
    Structure value;

    AStructure(const AttributeParam& _sharedParam, TemplateStructure& structureTemplate);
    AStructure(const AttributeParam& _sharedParam, const Structure& _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Structure; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    std::vector<QString> GetDisplayedTexts() const;
};

#endif // ASTRUCTURE_H
