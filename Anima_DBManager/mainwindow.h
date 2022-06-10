#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMenuBar>
#include <QTabWidget>
#include <QToolBox>

#include "db_manager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    DB_Manager& myManager;

    QMenuBar* myMenuBar;
    QTabWidget* myTabStruct;
    QTabWidget* myTabString;
    QToolBox* myDefToolBox;



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H
