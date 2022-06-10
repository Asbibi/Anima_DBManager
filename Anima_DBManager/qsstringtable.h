#ifndef QSSTRINGTABLE_H
#define QSSTRINGTABLE_H

#include <QTableWidget>

#include "sstringtable.h"
#include <QPoint>

class QSStringTable : public QTableWidget
{
    Q_OBJECT

private:
    SStringTable& myStringTable;
    const QBrush defBrush = QBrush(QColorConstants::White);
    const QBrush idBrush = QBrush(QColorConstants::LightGray);
    const QBrush emptyBrush = QBrush(QColor(255,158,158));

public:
    QSStringTable(SStringTable& _stringTable, QWidget* _parent = nullptr);

signals:
    void TableEdited(int _row, int _col, const QString& _text);

public slots:
    void OnCellEdited(int row, int col);
    void UpdateTable();
    void HandleContextMenu(const QPoint& point);
};

#endif // QSSTRINGTABLE_H
