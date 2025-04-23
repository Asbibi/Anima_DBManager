#ifndef QIMPORTENUMFROMTEXTDIALOG_H
#define QIMPORTENUMFROMTEXTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>

class QImportEnumFromTextDialog : public QDialog
{
    Q_OBJECT

private:
    QComboBox* myEnumComboBox;
    QLineEdit* myNewEnumName;
    QTextEdit* myTextEdit;

public:
    QImportEnumFromTextDialog(QWidget* _parent = nullptr);

public slots:
    void OnApplyBtnClicked();
    void OnEnumComboBoxChanged(int _index);
};

#endif // QIMPORTENUMFROMTEXTDIALOG_H
