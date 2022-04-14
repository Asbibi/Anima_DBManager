#include "mainwindow.h"

//#include<QDebug>
//#include "structure.h"
//#include "astring.h"
#include <QLabel>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      myManager(DB_Manager::GetDB_Manager())
{
    myTabPanel = new QTabWidget(this);
    auto lab = new QLabel(nullptr);
    lab->setText("Hello world");
    myTabPanel->addTab(lab, "Hello");
    myTabPanel->addTab(new QTextEdit(nullptr), "Edit");
    myTabPanel->resize(100,60);
    //QGroupBox for grouping things maybe for creating structures templates or enum
}

MainWindow::~MainWindow()
{
}

