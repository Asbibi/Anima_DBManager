#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>

#include "attributetype.h"
#include "attributeparam.h"
#include <fstream>
#include <QJsonArray>
#include <QJsonObject>
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
    virtual QString GetValue_CSV()                          const = 0;
    virtual QJsonValue GetValue_JSON()                      const = 0;
    virtual void CopyValueFromOther(const Attribute* _other)      = 0;
    virtual bool SetValue_JSON(const QJsonValue& _value)          = 0;
    virtual void SetValue_CSV(const QString& _text)               = 0;

    void WriteValue_CSV(std::ofstream& file)                const;

    const TemplateAttribute* GetTemplate()                  const;
    const AttributeParam& GetTemplateParam()                const;
    virtual QString GetValue_String()                       const;

    static QString GetContentStringFromList(const QList<Attribute*>& _attributes);

signals:
    void OnValueChanged(const Attribute* _this);    // arg given during emit()

//public slots:
};

#define MY_SHARED_PARAM myTemplate.GetSharedParam()

#endif // ATTRIBUTE_H
