#ifndef QSSTRINGTABLE_H
#define QSSTRINGTABLE_H

#include <QTableWidget>

#include "sstringtable.h"
#include <QPoint>

class QSStringTable : public QTableWidget
{
    Q_OBJECT

private:
    int myStringTableIndex;
    const QBrush defBrush = QBrush(QColorConstants::White);
    const QBrush idBrush = QBrush(QColorConstants::LightGray);
    const QBrush emptyBrush = QBrush(QColor(255,158,158));

    SStringTable& GetTable();
    void PrivateUpdate();

public:
    QSStringTable(int _strTableIndex, QWidget* _parent = nullptr);
    void UpdateIndex(int _strTableIndex);
    void ExportStringsToCSV(const QString _directoryPath, SStringHelper::SStringLanguages _language, bool _withDictionaryReplacement = true);

signals:
    void IdentifierEdited(int _tableIndex);

public slots:
    void OnCellEdit(QTableWidgetItem *item);
    void OnCellEdited(int row, int col);
    void UpdateTable();
    void HandleContextMenu(const QPoint& point);
};

#endif // QSSTRINGTABLE_H
