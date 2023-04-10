#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>

#include "sstringhelper.h"

#include "qstructuretable.h"
#include "qsstringtable.h"
#include "qimportstringdialog.h"
#include "qimportstructdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    myManager(DB_Manager::GetDB_Manager())
{
    setWindowIcon(QIcon("../DB_Icon.png"));
    resize(1280,720);

    myMenuBar = new QMenuBar(this);
    QMenu* fileMenu = myMenuBar->addMenu("File");
    QMenu* exportImportMenu = myMenuBar->addMenu("Export/Import");
    QMenu* displayMenu = myMenuBar->addMenu("Display");
    setMenuBar(myMenuBar);

    fileMenu->addAction("New");
    fileMenu->addAction("Open");
    fileMenu->addAction("Save");
    fileMenu->addAction("Save As...");

    auto* exportCurrentStruct = exportImportMenu->addAction("Export Current Structure Table");
    QObject::connect(exportCurrentStruct, &QAction::triggered, this, &MainWindow::OnExportCurrentStructTable);
    auto* exportAllStruct = exportImportMenu->addAction("Export All Structure Tables");
    QObject::connect(exportAllStruct, &QAction::triggered, this, &MainWindow::OnExportAllStructTables);
    exportImportMenu->addSeparator();
    QMenu* exportCurrentStringMenu = exportImportMenu->addMenu("Export Current String Table");
    QMenu* exportAllStringsMenu = exportImportMenu->addMenu("Export All String Tables");
    exportImportMenu->addSeparator();
    auto* exportAll = exportImportMenu->addAction("Export Everything");
    QObject::connect(exportAll, &QAction::triggered, this, &MainWindow::OnExportAll);

    exportImportMenu->addSeparator();
    auto* importStructAction = exportImportMenu->addAction("Import Structure Table");
    QObject::connect(importStructAction, &QAction::triggered, this, &MainWindow::OnImportStuctTable);
    auto* importStringAction = exportImportMenu->addAction("Import String Table");
    QObject::connect(importStringAction, &QAction::triggered, this, &MainWindow::OnImportStringTable);


    for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
    {
        auto* exportCurrentStringAction = exportCurrentStringMenu->addAction("Export Current in " + SStringHelper::GetLanguageString((SStringHelper::SStringLanguages)i));
        QObject::connect(exportCurrentStringAction, &QAction::triggered, this, [this, i]{OnExportCurrentStringTable((SStringHelper::SStringLanguages)i);});

        auto* exportAllStringAction = exportAllStringsMenu->addAction("Export All in " + SStringHelper::GetLanguageString((SStringHelper::SStringLanguages)i));
        QObject::connect(exportAllStringAction, &QAction::triggered, this, [this, i]{OnExportAllStringTables((SStringHelper::SStringLanguages)i);});
    }
    auto* exportCurrentStringAllLanguage = exportCurrentStringMenu->addAction("Export Current in All Languages");
    QObject::connect(exportCurrentStringAllLanguage, &QAction::triggered, this, [this]{OnExportCurrentStringTable(SStringHelper::SStringLanguages::Count);});
    auto* exportAllStringAllLanguage = exportAllStringsMenu->addAction("Export All in All Languages");
    QObject::connect(exportAllStringAllLanguage, &QAction::triggered, this, [this]{OnExportAllStringTables(SStringHelper::SStringLanguages::Count);});


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
    CONNECT_DB(StructTableFocus);
    CONNECT_DB(StructItemFocus);
    CONNECT_DB(StructItemChanged);
    CONNECT_DB(StructAttributeNameChanged);

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

    /*
    //QString style = QString("QTabBar::tab::selected {  color: %1 }").arg(stcTable->GetTemplateColorString());
    //QString style = QString("color: %1;").arg(stcTable->GetTemplateColorString());
    //myTabStruct->setTabsClosable(true);
    auto* tabBar = myTabStruct->tabBar();
    tabBar->setStyleSheet("QTabBar::tab {border-bottom: 2px solid black}");

    tabBar->setTabTextColor(0, QColorConstants::Red);
    QColor col = tabBar->tabTextColor(0);
    QString str = tabBar->tabText(0);
    qDebug() << col.name();
    qDebug() << str;
    tabBar->tabRect(0);

    auto* tabBtn = tabBar->tabButton(0, QTabBar::RightSide);
    if (tabBtn)
        qDebug("HHHHH");
        //tabBtn->setStyleSheet(style);
    auto* tabBtn2 = tabBar->tabButton(0, QTabBar::LeftSide);
    if (tabBtn2)
        qDebug("AAAAA");
    */
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
void MainWindow::OnStructTableFocus(const int _tableIndex, const int _itemIndex)
{
    if (myStructWidget->GetSelectedItem() == _tableIndex)
        myStructWidget->OnElementSelected(_itemIndex);
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
void MainWindow::OnStructAttributeNameChanged(const int _tableIndex)
{
    QStructureTable* currentTab = dynamic_cast<QStructureTable*>(myTabStruct->widget(_tableIndex));
    if (!currentTab)
        return;

    QStringList headerList;
    const auto* table = myManager.GetStructureTable(_tableIndex);
    Q_ASSERT(table);
    const auto& templateAttributeArray = table->GetTemplate().GetAttributes();
    for (const auto& attr : templateAttributeArray)
    {
        headerList.append(attr.GetName());
    }
    currentTab->setHorizontalHeaderLabels(headerList);
}





// ================      Export Methods      ================

void MainWindow::OnExportCurrentStringTable(SStringHelper::SStringLanguages _language)
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select String Table Directory",
                                                    myManager.GetProjectContentFolderPath());
    if (dir.isEmpty())
    {
        return;
    }

    QSStringTable* currentTab = dynamic_cast<QSStringTable*>(myTabString->currentWidget());
    Q_ASSERT(currentTab != nullptr);

    if (_language != SStringHelper::SStringLanguages::Count)
    {
        currentTab->ExportStringsToCSV(dir, _language);
    }
    else
    {
        for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
        {
            currentTab->ExportStringsToCSV(dir, (SStringHelper::SStringLanguages)i);
        }
    }
}
void MainWindow::OnExportAllStringTables(SStringHelper::SStringLanguages _language)
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select String Table Directory",
                                                    myManager.GetProjectContentFolderPath());
    if (dir.isEmpty())
    {
        return;
    }

    int widgetCount = myTabString->count();
    for (int i = 0; i < widgetCount; i++)
    {
        QSStringTable* tab = dynamic_cast<QSStringTable*>(myTabString->widget(i));
        if(tab == nullptr)
        {
            continue;
        }

        if (_language != SStringHelper::SStringLanguages::Count)
        {
            tab->ExportStringsToCSV(dir, _language);
        }
        else
        {
            for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
            {
                tab->ExportStringsToCSV(dir, (SStringHelper::SStringLanguages)i);
            }
        }
    }
}
void MainWindow::OnExportCurrentStructTable()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Struct DataTable Directory",
                                                    myManager.GetProjectContentFolderPath());
    if (dir.isEmpty())
    {
        return;
    }

    QStructureTable* currentTab = dynamic_cast<QStructureTable*>(myTabStruct->currentWidget());
    Q_ASSERT(currentTab != nullptr);

    currentTab->ExportStructsToCSV(dir);
}
void MainWindow::OnExportAllStructTables()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Struct DataTable Directory",
                                                    myManager.GetProjectContentFolderPath());
    if (dir.isEmpty())
    {
        return;
    }

    int widgetCount = myTabString->count();
    for (int i = 0; i < widgetCount; i++)
    {
        QStructureTable* tab = dynamic_cast<QStructureTable*>(myTabStruct->widget(i));
        if(tab == nullptr)
        {
            continue;
        }

        tab->ExportStructsToCSV(dir);
    }
}
void MainWindow::OnExportAll()
{
    // Todo : change so there is only 1 directory selection for both struct and string exports ?

    OnExportAllStructTables();
    OnExportAllStringTables(SStringHelper::SStringLanguages::Count);
}

void MainWindow::OnImportStringTable()
{
    auto* dialog = new QImportStringDialog(myStringWidget, this);
    dialog->exec();
    int res = dialog->result();
    if (res == QDialog::Accepted)
    {
        QSStringTable* stringTable = dynamic_cast<QSStringTable*>(myTabString->widget(dialog->GetTableIndex()));
        if(stringTable != nullptr)
        {
            // can be nullptr if we imported in a new table that has been emptied and thus deleted
            stringTable->UpdateTable();
        }
    }
    delete dialog;
}
void MainWindow::OnImportStuctTable()
{
    auto* dialog = new QImportStructDialog(myStructWidget, this);
    dialog->exec();
    int res = dialog->result();
    if (res == QDialog::Accepted)
    {
        QStructureTable* structTable = dynamic_cast<QStructureTable*>(myTabStruct->widget(dialog->GetTableIndex()));
        Q_ASSERT(structTable);
        structTable->UpdateTable();
    }

    delete dialog;
}
