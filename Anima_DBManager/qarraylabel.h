#ifndef QARRAYLABEL_H
#define QARRAYLABEL_H

#include <QLabel>

#include "aarray.h"


class QArrayLabel : public QLabel
{    
    Q_OBJECT

private:
    const AArray* myArrayAttribute;
    QString myValue;

    void ComputeMyValue(const QList<Attribute*>& _attributes);

public:
    QArrayLabel(QWidget* _parent);

    void SetValue(const AArray* _arrayAttr);
    QString GetValue() const;

signals:
    void OnValueEdited();

public slots:
    void EditValue();
};

#endif // QARRAYLABEL_H
