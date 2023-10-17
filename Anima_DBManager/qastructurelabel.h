#ifndef QASTRUCTURELABEL_H
#define QASTRUCTURELABEL_H

#include <QLabel>

#include "attribute.h"

class QAStructureLabel : public QLabel
{
    Q_OBJECT

private:
    QList<Attribute*> myAttributesPtr; // ptr to the astructure's actual attributes => !!! NOT THE OWNER !!!

public:
    QAStructureLabel(QWidget* _parent);

    void SetValue(const QList<Attribute*>& _structureAttrPtr);
    QString GetValue() const;

signals:
    void OnValueEdited();

public slots:
    void EditValue();
};

#endif // QASTRUCTURELABEL_H
