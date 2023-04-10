#ifndef QIMPORTSTRINGDIALOG_H
#define QIMPORTSTRINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include "sstringhelper.h"
#include "sstringtable.h"
#include "qpanelstring.h"

class QImportStringDialog : public QDialog
{
    Q_OBJECT

private:
    QMap<SStringHelper::SStringLanguages, QString> myCSVMap;
    QComboBox* myTableComboBox;
    QComboBox* myOverrideComboBox;
    QLineEdit* myNewTableName;
    QPanelString* myStringWidget;

    void PerformImport(SStringTable* _stringTable, int _overrideChoice);

public:
    QImportStringDialog(QPanelString* _stringWidget, QWidget* _parent = nullptr);
    int GetTableIndex() const;

public slots:
    void OnApplyBtnClicked();
    void OnFileBtnClicked(SStringHelper::SStringLanguages _language, QLabel* _label);
    void OnTableComboBoxChanged(int _index);
};

#endif // QIMPORTSTRINGDIALOG_H
