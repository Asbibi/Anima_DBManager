#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>

#include "attributetype.h"
#include "attributeparam.h"
#include <fstream>
#include <QJsonValue>

class TemplateAttribute;

class Attribute : public QObject
{
    Q_OBJECT

protected:
    TemplateAttribute& myTemplate;

    void EmitValueChanged();

public:
    Attribute(TemplateAttribute& _template);
    Attribute* CreateDuplica() const;
    void PreManualDelete();
    virtual ~Attribute();

    virtual AttributeTypeHelper::Type GetType()             const = 0;
    virtual QString GetDisplayedText()                      const = 0;
    virtual QString GetValueAsText()                        const = 0;
    virtual QString GetAttributeAsCSV()                     const = 0;
    virtual QJsonValue GetAttributeAsJSON()                 const = 0;
    virtual void CopyValueFromOther(const Attribute* _other)      = 0;
    virtual bool ReadValue_JSON(const QJsonValue& _value)         = 0;
    virtual void ReadValue_CSV(const QString& text);
    void WriteValue_CSV(std::ofstream& file) const;

    const TemplateAttribute* GetTemplate() const;
    const AttributeParam& GetTemplateParam() const;

signals:
    void OnValueChanged(const Attribute* _this);    // arg given during emit()

public slots:
    virtual void SetValueFromText(const QString& text)            = 0;
};

#define MY_SHARED_PARAM myTemplate.GetSharedParam()

#endif // ATTRIBUTE_H
