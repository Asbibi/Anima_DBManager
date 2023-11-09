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
    virtual QString GetValue_CSV()                         const override;
    virtual QJsonValue GetValue_JSON()                     const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)             override;
    virtual void SetValue_CSV(const QString& _text)                  override;

    void SetReference(const Structure* _reference);
    const Structure* GetReference() const;
    const StructureDB* GetStructureDB() const;
    int GetReferenceIndex() const;

    QString ConvertRowNameToTextValue(const QString& _rowName) const;
};

#endif // AREFERENCE_H
