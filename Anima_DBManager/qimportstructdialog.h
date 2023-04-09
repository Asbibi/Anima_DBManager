#ifndef QIMPORTSTRUCTDIALOG_H
#define QIMPORTSTRUCTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include "sstringhelper.h"
#include "sstringtable.h"
#include "qpanelstruct.h"

class QImportStructDialog : public QDialog
{
    Q_OBJECT

private:
    QLabel* mySelectedFile;
    QString mySelectedFilePath;
    QComboBox* myTableComboBox;
    QComboBox* myOverrideComboBox;
    QPanelStruct* myStructWidget;

    //void PerformImport(SStringTable* _stringTable, int _overrideChoice);

public:
    QImportStructDialog(QPanelStruct* _structWidget, QWidget* _parent = nullptr);
    int GetTableIndex() const;

public slots:
    void OnApplyBtnClicked();
    void OnFileBtnClicked();
};

#endif // QIMPORTSTRUCTDIALOG_H
