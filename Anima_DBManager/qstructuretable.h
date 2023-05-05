#ifndef QSTRUCTURETABLE_H
#define QSTRUCTURETABLE_H

#include <QTableWidget>

#include "structuredb.h"

class QStructureTable : public QTableWidget
{
    Q_OBJECT

private:
    StructureDB& myStructureDB;

public:
    explicit QStructureTable(StructureDB& _structureDB);
    virtual ~QStructureTable();
    void ExportStructsToCSV(const QString _directoryPath);

public slots:
    void UpdateTable();
    void OnSelectOrEditItem(int _index);
};

#endif // QSTRUCTURETABLE_H
