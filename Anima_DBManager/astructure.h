#ifndef ASTRUCTURE_H
#define ASTRUCTURE_H

#include "attribute.h"
#include "structure.h"

class AStructure : public Attribute
{
public:
    Structure value;

    AStructure(const StructureTemplate& structureTemplate);
    AStructure(const Structure& _value);

    virtual Type GetType() const override { return Type::Structure; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;

    std::vector<QString> GetDisplayedTexts() const;
};

#endif // ASTRUCTURE_H
