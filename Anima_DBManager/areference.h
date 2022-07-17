#ifndef AREFERENCE_H
#define AREFERENCE_H

#include "attribute.h"

#include <QPointer>

class Structure;
class StructureDB;

class AReference : public Attribute
{
private:
    QPointer<const Structure> myStructureRef;

public:
    AReference(const AttributeParam& _sharedParam);
    AReference(const AttributeParam& _sharedParam, QPointer<const Structure> _structureRef);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Reference; };
    virtual Attribute* CreateDuplica()                          const override;
    virtual QString GetDisplayedText(bool complete = false)     const override;
    virtual void WriteValue_CSV(std::ofstream& file)            const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;

    void SetReference(const Structure* _reference);
    const Structure* GetReference() const;
    const StructureDB* GetStructureDB() const;
    int GetReferenceIndex() const;
};

#endif // AREFERENCE_H
