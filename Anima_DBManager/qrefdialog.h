#ifndef QREFDIALOG_H
#define QREFDIALOG_H

#include <QDialog>

#include <QComboBox>
#include <QLabel>
#include "structuredb.h"

class QRefDialog : public QDialog
{
    Q_OBJECT

private:
    int& myIndex;
    const StructureDB& myStructureDB;

    QComboBox* myComboBox;
    QLabel* myStructContent;

public:
    QRefDialog(const StructureDB& structureDB, int& outIndex, QWidget* parent = nullptr);

public slots:
    void UpdateIndex(int index);
};

#endif // QREFDIALOG_H
