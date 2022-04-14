#ifndef QSTRUCTUREDB_H
#define QSTRUCTUREDB_H

#include <QWidget>
#include <QVBoxLayout>

#include "qstructure.h"
#include "structuredb.h"

class QStructureDB : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout* myLayout;
    std::vector<QStructure*> myStructureWidgets;

public:
    explicit QStructureDB(QWidget* parent = nullptr);
    ~QStructureDB();

    void Init(StructureDB& _structureDB);

public slots:
    void UpdateRows();
    void UpdateCols();
};

#endif // QSTRUCTUREDB_H
