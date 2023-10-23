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
    AReference(TemplateAttribute& _template);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Reference; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValueAsText()                            const override;
    virtual QString GetAttributeAsCSV()                         const override;
    virtual QJsonValue GetAttributeAsJSON()                     const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual void ReadValue_CSV(const QString& _text)                  override;

    void SetReference(const Structure* _reference);
    const Structure* GetReference() const;
    const StructureDB* GetStructureDB() const;
    int GetReferenceIndex() const;
};

#endif // AREFERENCE_H
