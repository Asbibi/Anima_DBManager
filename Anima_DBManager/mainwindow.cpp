#include "mainwindow.h"

#include "db_manager.h"
//#include<QDebug>
//#include "structure.h"
//#include "astring.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    DB_Manager& manager = DB_Manager::GetDB_Manager();
}

MainWindow::~MainWindow()
{
}

