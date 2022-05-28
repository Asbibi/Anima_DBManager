#ifndef QSSTRINGDIALOG_H
#define QSSTRINGDIALOG_H

#include <QDialog>

#include "sstringhelper.h"

#include <QComboBox>
#include <QLabel>

class QSStringDialog : public QDialog
{
    Q_OBJECT

private:
    QString& myTableName;
    QString& myStringIdentifier;

    QComboBox* myTableComboBox;
    QComboBox* myStringComboBox;

    SStringHelper::SStringLanguages myLanguage = SStringHelper::SStringLanguages::French;
    QLabel* myStringPreview;

    void UpdateStringComboBox();
    void UpdateStringPreview();

public:
    QSStringDialog(QString& _outTableId, QString& _outStringId, QWidget* parent = nullptr);

public slots:
    void SetTableName(const QString& text);
    void SetStringName(const QString& text);
    void SetLanguage(const int languageIndex);
};

#endif // QSSTRINGDIALOG_H
