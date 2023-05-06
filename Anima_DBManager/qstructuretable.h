#ifndef QSTRUCTURETABLE_H
#define QSTRUCTURETABLE_H

#include <QTableWidget>

#include "structuredb.h"
#include "qattribute.h"

class QStructureTable : public QTableWidget
{
    Q_OBJECT

private:
    StructureDB& myStructureDB;
    QAttribute* myCurrentAttributeEditor = nullptr;

public:
    explicit QStructureTable(StructureDB& _structureDB);
    virtual ~QStructureTable();
    void ExportStructsToCSV(const QString _directoryPath);

public slots:
    void UpdateTable();
    void OnSelectItem(int _currentRow, int _currentColumn, int _previousRow, int _previousColumn);
    void OnSelectOrEditItem(int _index);
};

#endif // QSTRUCTURETABLE_H
