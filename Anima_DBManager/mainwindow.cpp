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

#include "sstringhelper.h"

#include "qstructuretable.h"
#include "qsstringtable.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    myManager(DB_Manager::GetDB_Manager())
{
    setWindowIcon(QIcon("../DB_Icon.png"));
    resize(1280,720);

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
    myTableToolBox->addItem(myTabStruct, "Structures");
    myTableToolBox->addItem(myTabString, "Table Strings");

    //---------

    QToolBox* defToolBox = new QToolBox();
    splitter->addWidget(defToolBox);

    myEnumWidget = new QPanelEnum();
    myStringWidget = new QPanelString();
    myStructWidget = new QPanelStruct();
    myEnumWidget->Init();
    myStringWidget->Init();
    //myStructWidget->Init();
    defToolBox->addItem(myStructWidget, "Structures");
    defToolBox->addItem(myStringWidget, "String Tables");
    defToolBox->addItem(myEnumWidget, "Enumerators");

    //---------
#define CONNECT_DB(method)  QObject::connect(&myManager, &DB_Manager::method, this, &MainWindow::On##method)

    CONNECT_DB(StringTableAdded);
    CONNECT_DB(StringTableMoved);
    CONNECT_DB(StringTableRemoved);
    CONNECT_DB(StringTableRenamed);
    CONNECT_DB(StringTableChanged);
    CONNECT_DB(StringItemFocus);
    CONNECT_DB(StringItemChanged);

    CONNECT_DB(StructTableAdded);
    CONNECT_DB(StructTableMoved);
    CONNECT_DB(StructTableRemoved);
    CONNECT_DB(StructTableRenamed);
    CONNECT_DB(StructTableChanged);
    CONNECT_DB(StructItemFocus);
    CONNECT_DB(StructItemChanged);

#undef CONNECT_DB
}

MainWindow::~MainWindow()
{}


void MainWindow::Debug_Update()
{
    myEnumWidget->UpdateItemList();
    myStringWidget->UpdateItemList();
    myStructWidget->UpdateItemList();
}


void MainWindow::OnStringTableAdded(const int _index)
{
    SStringTable* sTable = myManager.GetStringTable(_index);
    if (!sTable)
        return;

    QSStringTable* stringTable = new QSStringTable(_index);
    myTabString->insertTab(_index, stringTable, sTable->GetTableName());
    myTabString->setCurrentIndex(_index);
}
void MainWindow::OnStringTableMoved(const int _indexFrom, const int _indexTo)
{
    QSStringTable* tabToMove = dynamic_cast<QSStringTable*>(myTabString->widget(_indexFrom));
    if (!tabToMove)
        return;

    const QString tabName = myTabString->tabText(_indexFrom);
    const bool wasCurrent = myTabString->currentIndex() == _indexFrom;
    myTabString->removeTab(_indexFrom);
    myTabString->insertTab(_indexTo, tabToMove, tabName);

    const int count = myTabString->count();
    for (int i = 0; i < count; i++)
    {
        QSStringTable* tab = dynamic_cast<QSStringTable*>(myTabString->widget(_indexFrom));
        if (tab)
            tab->UpdateIndex(i);
    }

    if(wasCurrent)
        myTabString->setCurrentIndex(_indexTo);
}
void MainWindow::OnStringTableRemoved(const int _index)
{
    QWidget* tabToDelete = myTabString->widget(_index);
    if (!tabToDelete)
        return;

    myTabString->removeTab(_index);
    delete tabToDelete;
}
void MainWindow::OnStringTableRenamed(const int _index, const QString& _name)
{
    myTabString->setTabText(_index, _name);
}
void MainWindow::OnStringTableChanged(const int _tableIndex)
{
    if (myStringWidget->GetSelectedItem() == _tableIndex)
        myStringWidget->OnItemSelected(_tableIndex);
}
void MainWindow::OnStringItemFocus(const int _tableIndex, const int _index)
{
    QSStringTable* currentTab = dynamic_cast<QSStringTable*>(myTabString->widget(_tableIndex));
    if (!currentTab)
        return;

    currentTab->setCurrentCell(_index, 0);
}
void MainWindow::OnStringItemChanged(const int _tableIndex)
{
    QSStringTable* currentTab = dynamic_cast<QSStringTable*>(myTabString->widget(_tableIndex));
    if (!currentTab)
        return;

    currentTab->UpdateTable();
}


void MainWindow::OnStructTableAdded(const int _index)
{
    StructureDB* stcTable = myManager.GetStructureTable(_index);
    if (!stcTable)
        return;

    QStructureTable* stuctureTable = new QStructureTable(*stcTable);
    myTabStruct->insertTab(_index, stuctureTable, stcTable->GetTemplateName());
    myTabStruct->setCurrentIndex(_index);
}
void MainWindow::OnStructTableMoved(const int _indexFrom, const int _indexTo)
{
    QStructureTable* tabToMove = dynamic_cast<QStructureTable*>(myTabStruct->widget(_indexFrom));
    if (!tabToMove)
        return;

    const QString tabName = myTabStruct->tabText(_indexFrom);
    const bool wasCurrent = myTabStruct->currentIndex() == _indexFrom;
    myTabStruct->removeTab(_indexFrom);
    myTabStruct->insertTab(_indexTo, tabToMove, tabName);

    if(wasCurrent)
        myTabStruct->setCurrentIndex(_indexTo);
}
void MainWindow::OnStructTableRemoved(const int _index)
{
    QWidget* tabToDelete = myTabStruct->widget(_index);
    if (!tabToDelete)
        return;

    myTabStruct->removeTab(_index);
    delete tabToDelete;
}
void MainWindow::OnStructTableRenamed(const int _index, const QString& _name)
{
    myTabStruct->setTabText(_index, _name);
}
void MainWindow::OnStructTableChanged(const int _tableIndex)
{
    /*if (myStringWidget->GetSelectedItem() == _tableIndex)
        myStringWidget->OnItemSelected(_tableIndex);*/
}
void MainWindow::OnStructItemFocus(const int _tableIndex, const int _index)
{
    QStructureTable* currentTab = dynamic_cast<QStructureTable*>(myTabStruct->widget(_tableIndex));
    if (!currentTab)
        return;

    currentTab->setCurrentCell(_index, 0);
}
void MainWindow::OnStructItemChanged(const int _tableIndex)
{
    QStructureTable* currentTab = dynamic_cast<QStructureTable*>(myTabStruct->widget(_tableIndex));
    if (!currentTab)
        return;

    currentTab->UpdateTable();
}
