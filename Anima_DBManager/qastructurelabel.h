#ifndef QASTRUCTURELABEL_H
#define QASTRUCTURELABEL_H

#include <QLabel>

#include "attribute.h"

class QAStructureLabel : public QLabel
{
    Q_OBJECT

private:
    QList<Attribute*> myAttributesPtr;
    void ClearMyAttributes();

public:
    QAStructureLabel(QWidget* _parent);
    ~QAStructureLabel();

    void SetValue(const QList<Attribute*>& _structureAttrPtr);
    QString GetValue() const;

signals:
    void OnValueEdited();

public slots:
    void EditValue();
};

#endif // QASTRUCTURELABEL_H
