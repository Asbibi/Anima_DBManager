#ifndef QSTRUCTURETABLE_H
#define QSTRUCTURETABLE_H

#include <QTableWidget>

#include "structuredb.h"

class QStructureTable : public QTableWidget
{
    Q_OBJECT

private:
    StructureDB& myStructureDB;
    std::vector<QWidget*> myChildWidgetsToDelete;

    void DeleteAllChilds();

public:
    explicit QStructureTable(StructureDB& _structureDB);
    virtual ~QStructureTable();

signals:
public slots:
    void UpdateTable();
};

#endif // QSTRUCTURETABLE_H
