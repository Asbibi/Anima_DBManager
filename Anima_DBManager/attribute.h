#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>
#include <fstream>
#include "attributeparam.h"
//class QAttribute;


class Attribute : public QObject
{
    Q_OBJECT

protected:
    const AttributeParam* sharedParam;
    //QAttribute* myAttributeWidget = nullptr;

    //void ReplaceMyAttributeWidget(QAttribute* _newAttributeWidget);

public:
    enum class Type
    {
        Invalid,
        Bool,
        Enum,
        Float,
        Int,
        String,
        Array,
        Structure
    };

    Attribute(const AttributeParam* _sharedParam);
    virtual ~Attribute();

    virtual Type GetType()                         const = 0;
    virtual Attribute* CreateDuplica()                      const = 0;
    virtual QString GetDisplayedText(bool complete = false) const = 0;
    virtual void WriteValue_CSV(std::ofstream& file)        const = 0;

signals:
    void OnValueChanged(const Attribute& _this);    // arg given during emit()

public slots:
    virtual void SetValueFromText(const QString& text)            = 0;
};


//#define DefAttribute_CreateWidget(attName, q_attName) \
//QAttribute* attName::CreateWidget(QWidget* parent, const QColor& color) \
//{ \
//    QAttribute* widget = new q_attName(parent, QAttributeParam(ourWidth, ourHeight, color), this); \
//    ReplaceMyAttributeWidget(widget); \
//    return widget; \
//}


#endif // ATTRIBUTE_H
