#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidgetAction>

#include "sstringhelper.h"
#include "savemanager.h"

#include "qstructuretable.h"
#include "qsstringtable.h"
#include "qimportstringdialog.h"
#include "qimportstructdialog.h"
#include "qprojectdialog.h"
#include "qpanelsearch.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    myManager(DB_Manager::GetDB_Manager())
{
    setWindowIcon(QIcon(IconManager::GetAppIcon()));
    resize(1280,720);

    myMenuBar = new QMenuBar(this);
    QMenu* fileMenu = myMenuBar->addMenu("File");
    QMenu* exportImportMenu = myMenuBar->addMenu("Export/Import");
    QAction* projSettings = myMenuBar->addAction("Project Settings");
    setMenuBar(myMenuBar);

    auto* newDB = fileMenu->addAction("New");
    QObject::connect(newDB, &QAction::triggered, this, &MainWindow::OnNewDB);
    auto* openDB = fileMenu->addAction("Open");
    QObject::connect(openDB, &QAction::triggered, this, &MainWindow::OnOpenDB);
    auto* saveDB = fileMenu->addAction("Save");
    QObject::connect(saveDB, &QAction::triggered, this, &MainWindow::OnSaveDB);
    auto* saveAsDB = fileMenu->addAction("Save As...");
    QObject::connect(saveAsDB, &QAction::triggered, this, &MainWindow::OnSaveAsDB);

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


    QObject::connect(projSettings, &QAction::triggered, this, &MainWindow::OnProjectSettings);



    //---------

    QSplitter* splitter = new QSplitter(this);
    setCentralWidget(splitter);
    //splitter->setStyleSheet("QSplitter::handle { width: 2px; color: solid green;}");

    //---------

    myTableToolBox = new QToolBox();
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
    QPanelSearch* searchPanel = new QPanelSearch();
    myEnumWidget->Init();
    myStringWidget->Init();
    //myStructWidget->Init();
    defToolBox->addItem(myStructWidget, "Structures");
    defToolBox->addItem(myStringWidget, "String Tables");
    defToolBox->addItem(myEnumWidget, "Enumerators");
    defToolBox->addItem(searchPanel, "Search");

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
    CONNECT_DB(StructTableIconChanged);
    CONNECT_DB(StructTableFocus);
    CONNECT_DB(StructItemFocus);
    CONNECT_DB(StructItemChanged);
    CONNECT_DB(StructAttributeNameChanged);

    CONNECT_DB(ResetView);

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
void MainWindow::OnStringItemFocus(const int _tableIndex, const int _index, const int _languageIndex, const bool _forceFocus)
{
    QSStringTable* currentTab = dynamic_cast<QSStringTable*>(myTabString->widget(_tableIndex));
    if (!currentTab)
        return;

    if (_forceFocus)
    {
        myTabString->setCurrentIndex(_tableIndex);
        myTableToolBox->setCurrentIndex(1);
    }

    currentTab->setCurrentCell(_index, _languageIndex+1);
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
    const int insertedIndex = myTabStruct->insertTab(_index, stuctureTable, stcTable->GetTemplateName());
    OnStructTableIconChanged(insertedIndex, stcTable->GetIcon());
    myTabStruct->setCurrentIndex(insertedIndex);
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
void MainWindow::OnStructTableIconChanged(const int _index, const QIcon& _icon)
{
    myTabStruct->setTabIcon(_index, _icon);
}
void MainWindow::OnStructTableFocus(const int _tableIndex, const int _itemIndex)
{
    if (myStructWidget->GetSelectedItem() == _tableIndex)
        myStructWidget->OnElementSelected(_itemIndex);
}
void MainWindow::OnStructItemFocus(const int _tableIndex, const int _index, const int _attrIndex, const bool _forceFocus)
{
    QStructureTable* currentTab = dynamic_cast<QStructureTable*>(myTabStruct->widget(_tableIndex));
    if (!currentTab)
        return;

    if (_forceFocus)
    {
        myTabStruct->setCurrentIndex(_tableIndex);
        myTableToolBox->setCurrentIndex(0);
    }

    currentTab->setCurrentCell(_index, _attrIndex);
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
    for (const auto* attr : templateAttributeArray)
    {
        headerList.append(attr->GetName());
    }
    currentTab->setHorizontalHeaderLabels(headerList);
}

void MainWindow::CleanTabWidget(QTabWidget* _tabWidget)
{
    const int tabCount = _tabWidget->count();
    _tabWidget->blockSignals(true);
    for (int i = tabCount -1; i > -1; i--)
    {
        auto* wid = _tabWidget->widget(i);
        _tabWidget->removeTab(i);
        wid->disconnect();
        delete wid;
    }
    _tabWidget->blockSignals(false);
}
void MainWindow::OnResetView()
{
    myStructWidget->Reset();
    myStringWidget->Reset();
    myEnumWidget->Reset();

    CleanTabWidget(myTabStruct);
    CleanTabWidget(myTabString);

    const int stringTableCount = myManager.GetStringTableCount();
    for (int i = 0; i < stringTableCount; i++)
    {
        OnStringTableAdded(i);
    }
    const int structTableCount = myManager.GetStructuresCount();
    for (int i = 0; i < structTableCount; i++)
    {
        OnStructTableAdded(i);
        OnStructItemChanged(i);
    }
}



// ================       File Methods       ================

bool MainWindow::OnNewDB()
{
    QMessageBox::StandardButton btn = QMessageBox::question(this, "Save ?", "Save project before proceeding ?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
    if (btn == QMessageBox::Cancel)
    {
        return false;
    }
    else if (btn == QMessageBox::Yes)
    {
        bool saveComplete = OnSaveDB();
        if (!saveComplete)
            return false;
    }

    myStructWidget->UnselectItem();
    const int structTabCount = myTabStruct->count();
    for (int i = 0; i < structTabCount; i++)
    {
        QStructureTable* structTable = dynamic_cast<QStructureTable*>(myTabStruct->widget(i));
        Q_ASSERT(structTable != nullptr);
        structTable->UnselectCurrent();
    }

    DB_Manager::GetDB_Manager().Reset();

    return true;
}
bool MainWindow::OnSaveDB()
{
    return OnSaveDB_Internal(false);
}
void MainWindow::OnSaveAsDB()
{
    OnSaveDB_Internal(true);
}
bool MainWindow::OnSaveDB_Internal(bool _saveAs)
{
    const QString& fileExt = SaveManager::GetSaveFileExtension();
    QString filePath = myCurrentlyOpenedFile;

    if (_saveAs || myCurrentlyOpenedFile.isEmpty() || !QFileInfo::exists(myCurrentlyOpenedFile))
    {
        if (myCurrentlyOpenedFile.isEmpty())
        {
            filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/New DataBase." + fileExt;
        }


        filePath = QFileDialog::getSaveFileName(this, "Save DataBase",
                                                   filePath,
                                                   "Unreal Anima Database (*." + fileExt + ")");
        if (filePath.isEmpty())
            return false;
    }
    //else : check if any change ? Or do nothing ?

    qDebug() << filePath;

    SaveManager::SaveFile(filePath);
    return true;
}
void MainWindow::OnOpenDB()
{
    // Choose FilePath

    const QString& fileExt = SaveManager::GetSaveFileExtension();
    QString filePath = myCurrentlyOpenedFile;

    if (filePath.isEmpty())
    {
        filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }
    filePath = QFileDialog::getOpenFileName(this, "Open DataBase",
                                               filePath,
                                               "Unreal Anima Database (*." + fileExt + ")");
    if (filePath.isEmpty())
    {
        return;
    }
    if (myCurrentlyOpenedFile == filePath)
    {
        QMessageBox::StandardButton btn = QMessageBox::question(this, "Refresh opened", "The Database you asked to open is already opened.\nDo you want to reopen it ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (btn == QMessageBox::No)
        {
            return;
        }
    }

    qDebug() << filePath;
    Q_ASSERT(QFileInfo::exists(filePath));


    // New
    OnNewDB();

    // OpenInternal
    myManager.blockSignals(true);
    SaveManager::OpenFile(filePath);
    myManager.blockSignals(false);
    OnResetView();

    // set myCurrentlyOpenedFile et the end of the process
    myCurrentlyOpenedFile = filePath;
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
void MainWindow::OnProjectSettings()
{
    auto* dialog = new QProjectDialog(this);
    dialog->exec();
    int res = dialog->result();
    delete dialog;

    if (res != QDialog::Accepted)
    {
        return;
    }

    myManager.UpdateAAssetIsDirty();
}
