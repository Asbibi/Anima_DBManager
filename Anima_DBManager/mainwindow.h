#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTabWidget>

#include "db_manager.h"

#include "attribute.h"
#include "qattribute.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    DB_Manager& myManager;

    QTabWidget* myTabPanel = nullptr;

    std::vector<Attribute*> myDebugAttributes;
    QAttribute* myAttr = nullptr;
};
#endif // MAINWINDOW_H
