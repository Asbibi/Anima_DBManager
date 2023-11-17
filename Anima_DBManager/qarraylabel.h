#ifndef QARRAYLABEL_H
#define QARRAYLABEL_H

#include <QLabel>

#include "aarray.h"
#include <QJsonArray>


class QArrayLabel : public QLabel
{    
    Q_OBJECT

private:
    const AArray* myArrayAttribute;
    QJsonArray myValue;

    void ComputeMyValue(const QList<Attribute*>& _attributes);

public:
    QArrayLabel(QWidget* _parent);

    void SetValue(const AArray* _arrayAttr);
    QJsonArray GetValue() const;

signals:
    void OnValueEdited();

public slots:
    void EditValue();
};

#endif // QARRAYLABEL_H
