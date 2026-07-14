#ifndef QSAVESETTINGSDIALOG_H
#define QSAVESETTINGSDIALOG_H

#include <QDialog>

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>


class QSaveSettingsDialog : public QDialog
{
    Q_OBJECT

private:
    QCheckBox* myAutoSaveEnable;
    QSpinBox* myAutoSaveInterval;
    QCheckBox* myCompressSaveFile;

public:
    QSaveSettingsDialog(QWidget* _parent = nullptr);

public slots:
    void OnApplyBtnClicked();
};

#endif // QSAVESETTINGSDIALOG_H
