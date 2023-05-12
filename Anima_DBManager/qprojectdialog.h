#ifndef QPROJECTDIALOG_H
#define QPROJECTDIALOG_H

#include <QDialog>
#include <QLabel>


class QProjectDialog : public QDialog
{
    Q_OBJECT

private:
    QLabel* myProjectPath;

    void SetPath(const QString& _path);

public:
    QProjectDialog(QWidget* _parent = nullptr);

public slots:
    void OnSelectPath();
    void OnResetPath();

    void OnApplyBtnClicked();
};

#endif // QPROJECTDIALOG_H
