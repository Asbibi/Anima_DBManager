#ifndef ASTRUCTURE_H
#define ASTRUCTURE_H

#include "attribute.h"
#include "structure.h"

class AStructure : public Attribute
{
public:
    Structure* myValue;

    AStructure(TemplateAttribute& _template);
    AStructure(TemplateAttribute& _template, const Structure* _value);

    virtual AttributeTypeHelper::Type GetType() const override { return AttributeTypeHelper::Type::Structure; };
    virtual QString GetDisplayedText()                          const override;
    virtual QString GetValueAsText()                            const override;
    virtual QString GetValue_CSV()                         const override;
    virtual QJsonValue GetValue_JSON()                     const override;
    virtual void SetValueFromText(const QString& text)                override;
    virtual void CopyValueFromOther(const Attribute* _other)          override;
    virtual bool SetValue_JSON(const QJsonValue& _value)             override;

    QList<QString> GetDisplayedTexts() const;
    const QList<Attribute*>& GetAttributes() const;
    void ResetValueToDefaults();

    static QString GetValueAsTextFromAttributes(const QList<Attribute*>& _attributes);
};

#endif // ASTRUCTURE_H
