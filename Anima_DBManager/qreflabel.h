#ifndef QREFLABEL_H
#define QREFLABEL_H

#include <QLabel>

#include <QPointer>
#include "structuredb.h"

class QRefLabel : public QLabel
{
    Q_OBJECT

private:
    QPointer<const StructureDB> hisStructureDB;
    int myRefIndexInDB;

public:
    QRefLabel(QWidget* parent = nullptr);

    void SetStructureDB(const StructureDB* structureDB);
    void SetValue(int index);
    int GetValue() const;
    const QString GetValueText();

signals:
    void OnValueEdited();

public slots:
    void EditValue();
};

#endif // QREFLABEL_H
