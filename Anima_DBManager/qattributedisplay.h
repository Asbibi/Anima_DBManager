#ifndef QATTRIBUTEDISPLAY_H
#define QATTRIBUTEDISPLAY_H

#include <QTableWidgetItem>
#include "attribute.h"

class QAttributeDisplay : public QTableWidgetItem
{
private:
    static QColor defaultColor;
    static QColor nullColor;
    static QColor invalidColor;

    AttributeTypeHelper::Type myCurrentType = AttributeTypeHelper::Type::Invalid;
    QString myCurrentValue;

    bool AskUpdateContent(const Attribute* _attribute);
    void UpdateContent(const Attribute* _attribute);

public:
    QAttributeDisplay();

    bool IsRepresentingABool() const;
    void SetContentFromAttribute(const Attribute* _attribute);
    virtual QTableWidgetItem* clone() const override;
};

#endif // QATTRIBUTEDISPLAY_H
