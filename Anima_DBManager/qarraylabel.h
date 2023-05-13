#ifndef QARRAYLABEL_H
#define QARRAYLABEL_H

#include <QLabel>

#include "templateattribute.h"


class QArrayLabel : public QLabel
{    
    Q_OBJECT

public:
    QArrayLabel(QWidget* _parent);

    void SetValue(TemplateAttribute* _template, const QStringList& _attrStringList);

signals:
    void OnValueEdited();

public slots:
    void EditValue();
};

#endif // QARRAYLABEL_H
