#ifndef QIMPORTSTRINGDIALOG_H
#define QIMPORTSTRINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include "sstringhelper.h"
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

public:
    QImportStringDialog(QPanelString* _stringWidget, QWidget* _parent = nullptr);

signals:
    //void FilePathChanged(const QString& _filePath);

public slots:
    void OnApplyBtnClicked();
    void OnFileBtnClicked(SStringHelper::SStringLanguages _language, QLabel* _label);
    void OnTableComboBoxChanged(int _index);
};

#endif // QIMPORTSTRINGDIALOG_H
