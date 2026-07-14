#ifndef QABOUTDIALOG_H
#define QABOUTDIALOG_H

#include <QDialog>

class QAboutDialog : public QDialog
{
    Q_OBJECT

public:
    QAboutDialog(QWidget* _parent = nullptr);
};

#endif // QABOUTDIALOG_H
