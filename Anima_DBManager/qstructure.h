#ifndef QSTRUCTURE_H
#define QSTRUCTURE_H

#include <QWidget>
#include "structure.h"

class QStructure : public QWidget
{
    Q_OBJECT

private:
    Structure* structure;
    QColor structureColor;
    QColor backgroundColor = QColor(240,240,240);
    bool displayAttributeNames = true;

public:
    explicit QStructure(QWidget *parent = nullptr);

signals:

};

#endif // QSTRUCTURE_H
