#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>
#include <fstream>
#include "attributeparam.h"

class Attribute : public QObject
{
    Q_OBJECT

protected:
    const AttributeParam* sharedParam;

    void EmitValueChanged();

public:
    enum class Type
    {
        Invalid,

        Bool,
        Enum,
        Float,
        Int,
        ShortString,
        TableString,

        Array,
        Structure,

        Texture,
        Mesh,
        AnimInstance,
        Niagara,
        Sound,
    };

    Attribute(const AttributeParam* _sharedParam);
    virtual ~Attribute();

    virtual Type GetType()                                  const = 0;
    virtual Attribute* CreateDuplica()                      const = 0;
    virtual QString GetDisplayedText(bool complete = false) const = 0;
    virtual void WriteValue_CSV(std::ofstream& file)        const = 0;

signals:
    void OnValueChanged(const Attribute* _this);    // arg given during emit()

public slots:
    virtual void SetValueFromText(const QString& text)            = 0;
};

#endif // ATTRIBUTE_H
