#ifndef QATTRIBUTE_H
#define QATTRIBUTE_H

#include <QWidget>

#include "attribute.h"


class QAttribute : public QWidget
{
    Q_OBJECT

private:
    QWidget* myContent;
    Attribute::Type myType = Attribute::Type::Invalid;

public:
    explicit QAttribute(QWidget* parent = nullptr);
    ~QAttribute();

signals:
    void OnAttributeEdited(const QString& _thisQAttribute);    // arg given during emit()

public slots:
    void UpdateAttribute(const Attribute& _attribute);
};

#endif // QATTRIBUTE_H
