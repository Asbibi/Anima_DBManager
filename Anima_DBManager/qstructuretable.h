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
    void Unselect(int _row, int _col);

public:
    explicit QStructureTable(StructureDB& _structureDB);
    virtual ~QStructureTable();
    void ExportStructsToJSON(const QString _directoryPath);
    void ExportStructsToCSV(const QString _directoryPath);
    void UnselectCurrent();

public slots:
    void UpdateTable();
    void OnSelectItem(int _currentRow, int _currentColumn, int _previousRow, int _previousColumn);
    void OnSelectOrEditItem(int _index);
    void OnSelectionChanged();
};

#endif // QSTRUCTURETABLE_H
