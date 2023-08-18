#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>
#include <fstream>
#include "attributetype.h"
#include "attributeparam.h"

class Attribute : public QObject
{
    Q_OBJECT

protected:
    const AttributeParam& mySharedParam;

    void EmitValueChanged();

public:
    Attribute(const AttributeParam& _sharedParam);
    Attribute* CreateDuplica() const;
    virtual ~Attribute();

    virtual AttributeTypeHelper::Type GetType()             const = 0;
    virtual QString GetDisplayedText()                      const = 0;
    virtual QString GetValueAsText()                        const = 0;
    virtual QString GetAttributeAsCSV()                     const = 0;
    virtual void CopyValueFromOther(const Attribute* _other)      = 0;
    virtual void ReadValue_CSV(const QString& text);
    void WriteValue_CSV(std::ofstream& file) const;

signals:
    void OnValueChanged(const Attribute* _this);    // arg given during emit()

public slots:
    virtual void SetValueFromText(const QString& text)            = 0;
};

#endif // ATTRIBUTE_H
