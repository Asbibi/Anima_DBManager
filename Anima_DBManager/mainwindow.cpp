#include "mainwindow.h"

#include<QDebug>
#include "structure.h"
#include "astring.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    StructureTemplate str = StructureTemplate("Test", QColorConstants::Red);
    str.AddAttribute("Hello", new AString(nullptr, "Hey"));
    const Attribute* att = str.GetAttribute("Hello");
}

MainWindow::~MainWindow()
{
}

