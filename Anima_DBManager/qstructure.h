#ifndef QSTRUCTURE_H
#define QSTRUCTURE_H

#include <QWidget>
#include <QBoxLayout>

#include "qattribute.h"
#include "structure.h"

class QStructure : public QWidget
{
    Q_OBJECT

private:
    QBoxLayout* myLayout;
    std::vector<QAttribute*> myStructureWidgets;

public:
    explicit QStructure(QWidget *parent = nullptr);
    ~QStructure();


//signals:

public slots:
    void UpdateAttributes(const Structure& _structure);
};

#endif // QSTRUCTURE_H
