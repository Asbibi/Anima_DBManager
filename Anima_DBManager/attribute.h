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
    virtual ~Attribute();

    virtual AttributeTypeHelper::Type GetType()             const = 0;
    virtual Attribute* CreateDuplica()                      const = 0;
    virtual QString GetDisplayedText(bool complete = false) const = 0;
    virtual void WriteValue_CSV(std::ofstream& file)        const = 0;
    virtual void CopyValueFromOther(const Attribute* _other)      = 0;
    virtual void ReadValue_CSV(const QString& text);

signals:
    void OnValueChanged(const Attribute* _this);    // arg given during emit()

public slots:
    virtual void SetValueFromText(const QString& text)            = 0;
};

#endif // ATTRIBUTE_H
