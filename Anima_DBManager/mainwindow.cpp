#include "mainwindow.h"

#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMenu>
#include <QAction>

#include "qstructuretable.h"
#include "qsstringtable.h"
#include "sstringhelper.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      myManager(DB_Manager::GetDB_Manager())
{
    myMenuBar = new QMenuBar(this);
    QMenu* fileMenu = myMenuBar->addMenu("File");
    QMenu* exportMenu = myMenuBar->addMenu("Export");
    QMenu* importMenu = myMenuBar->addMenu("Import");
    QMenu* displayMenu = myMenuBar->addMenu("Display");
    setMenuBar(myMenuBar);

    fileMenu->addAction("New");
    fileMenu->addAction("Open");
    fileMenu->addAction("Save");
    fileMenu->addAction("Save As...");

    exportMenu->addAction("Export Current Structure Table");
    exportMenu->addAction("Export All Structure Tables");
    exportMenu->addSeparator();
    QMenu* exportCurrentStringMenu = exportMenu->addMenu("Export Current String Table");
    QMenu* exportAllStringsMenu = exportMenu->addMenu("Export All String Tables");
    exportMenu->addSeparator();
    exportMenu->addAction("Export Everything");

    importMenu->addAction("Import in Current Structure Table");
    QMenu* importCurrentStringMenu = importMenu->addMenu("Import in Current String Table");

    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        exportCurrentStringMenu->addAction("Export Current in " + SStringHelper::GetLanguageString((SStringHelper::SStringLanguages)i));
        exportAllStringsMenu->addAction("Export All in " + SStringHelper::GetLanguageString((SStringHelper::SStringLanguages)i));
        importCurrentStringMenu->addAction("Import in Current in " + SStringHelper::GetLanguageString((SStringHelper::SStringLanguages)i));
    }
    exportCurrentStringMenu->addAction("Export Current in All Languages");
    exportAllStringsMenu->addAction("Export All in All Languages");

    displayMenu->addAction("Update Current Table");
    displayMenu->addAction("Update All Tables");


    //---------

    QSplitter* splitter = new QSplitter(this);
    setCentralWidget(splitter);
    //splitter->setStyleSheet("QSplitter::handle { width: 2px; color: solid green;}");

    //---------

    QToolBox* myTableToolBox = new QToolBox();
    myTableToolBox->setMinimumWidth(600);
    splitter->addWidget(myTableToolBox);

    myTabStruct = new QTabWidget();
    myTabString = new QTabWidget();
    myTableToolBox->addItem(myTabString, "Structures");
    myTableToolBox->addItem(myTabStruct, "Table Strings");


    StructureDB* db = myManager.GetStructureTable(0);
    if (db != nullptr)
    {
        QStructureTable* testTable = new QStructureTable(*db);
        myTabStruct->addTab(testTable, db->GetTemplateName());
    }
    StructureDB* db2 = myManager.GetStructureTable(1);
    if (db2 != nullptr)
    {
        QStructureTable* testTable = new QStructureTable(*db2);
        myTabStruct->addTab(testTable, db2->GetTemplateName());
    }

    SStringTable* sTable = myManager.GetStringTable(0);
    if (sTable != nullptr)
    {
        QSStringTable* stringTable = new QSStringTable(*sTable);
        myTabString->addTab(stringTable, sTable->GetTableName());
    }

    //---------

    myDefToolBox = new QToolBox();
    splitter->addWidget(myDefToolBox);

    QWidget* enumWidget = new QWidget();
    QWidget* stringWidget = new QWidget();
    QWidget* structWidget = new QWidget();
    myDefToolBox->addItem(enumWidget, "Enumerators");
    myDefToolBox->addItem(stringWidget, "String Tables");
    myDefToolBox->addItem(structWidget, "Structures");

    /*QGroupBox* group = new QGroupBox("Attributes");
    QVBoxLayout *vbox = new QVBoxLayout(group);
    group->setLayout(vbox);
    vbox->addWidget(btn);*/
}

MainWindow::~MainWindow()
{}

