#include "mainwindow.h"

#include <QAction>
#include <QCloseEvent>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QStringList>
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
    UpdateWindowName();

    myMenuBar = new QMenuBar(this);
    QMenu* fileMenu = myMenuBar->addMenu("File");
    QMenu* exportImportMenu = myMenuBar->addMenu("Export/Import");
    QAction* projSettings = myMenuBar->addAction("Project Settings");
    setMenuBar(myMenuBar);

    auto* newDB = fileMenu->addAction("New");
    QObject::connect(newDB, &QAction::triggered, this, &MainWindow::OnNewDB);
    newDB->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    auto* openDB = fileMenu->addAction("Open");
    QObject::connect(openDB, &QAction::triggered, this, &MainWindow::OnOpenDB);
    openDB->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    myOpenRecentMenu = fileMenu->addMenu("Open Recent...");
    auto* saveDB = fileMenu->addAction("Save");
    QObject::connect(saveDB, &QAction::triggered, this, &MainWindow::OnSaveDB);
    saveDB->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    auto* saveAsDB = fileMenu->addAction("Save As...");
    QObject::connect(saveAsDB, &QAction::triggered, this, &MainWindow::OnSaveAsDB);
    saveAsDB->setShortcut(QKeySequence(Qt::SHIFT | Qt::CTRL | Qt::Key_S));
    fileMenu->addSeparator();
    auto* resetQSettingsAction = fileMenu->addAction("Reset Window");
    QObject::connect(resetQSettingsAction, &QAction::triggered, this, &MainWindow::ResetQSettings);
    resetQSettingsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    fileMenu->addSeparator();
    auto* quitAction = fileMenu->addAction("Quit application");
    QObject::connect(quitAction, &QAction::triggered, this, &QCoreApplication::quit);
    quitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    BuildOpenRecentMenu();


#ifdef CSV_EXPORT_ENABLED
    QMenu* exportCurrentStructMenu = exportImportMenu->addMenu("Export Current Structure Table");
    QMenu* exportAllStructsMenu = exportImportMenu->addMenu("Export All Structure Tables");
    auto* exportCurrentStructJSON = exportCurrentStructMenu->addAction("Export Current as JSON file");
    QObject::connect(exportCurrentStructJSON, &QAction::triggered, this, &MainWindow::OnExportCurrentStructTable_JSON);
    auto* exportCurrentStructCSV = exportCurrentStructMenu->addAction("Export Current as CSV file");
    QObject::connect(exportCurrentStructCSV, &QAction::triggered, this, &MainWindow::OnExportCurrentStructTable_CSV);
    auto* exportAllStructJSON = exportAllStructsMenu->addAction("Export All as JSON file");
    QObject::connect(exportAllStructJSON, &QAction::triggered, this, &MainWindow::OnExportAllStructTables_JSON);
    auto* exportAllStructCSV = exportAllStructsMenu->addAction("Export All as CSV file");
    QObject::connect(exportAllStructCSV, &QAction::triggered, this, &MainWindow::OnExportAllStructTables_CSV);
#else
    auto* exportCurrentStructJSON = exportImportMenu->addAction("Export Current Structure Table");
    QObject::connect(exportCurrentStructJSON, &QAction::triggered, this, &MainWindow::OnExportCurrentStructTable_JSON);
    myExportOneStructMenu = exportImportMenu->addMenu("Export One Structure Table");
    auto* exportAllStructJSON = exportImportMenu->addAction("Export All Structure Tables");
    QObject::connect(exportAllStructJSON, &QAction::triggered, this, &MainWindow::OnExportAllStructTables_JSON);
    BuildExportOneStructTableMenu();
#endif
    exportImportMenu->addSeparator();

    QMenu* exportCurrentStringMenu = exportImportMenu->addMenu("Export Current String Table");
    myExportOneStringMenu = exportImportMenu->addMenu("Export One String Table");
    QMenu* exportAllStringsMenu = exportImportMenu->addMenu("Export All String Tables");
    BuildExportOneStringTableMenu();

    exportImportMenu->addSeparator();
    auto* exportAll = exportImportMenu->addAction("Export Everything");
    QObject::connect(exportAll, &QAction::triggered, this, &MainWindow::OnExportAll);
    exportAll->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));

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
    projSettings->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));



    //---------

    mySplitter = new QSplitter(this);
    setCentralWidget(mySplitter);
    //splitter->setStyleSheet("QSplitter::handle { width: 2px; color: solid green;}");

    //---------

    myTableToolBox = new QToolBox();
    mySplitter->addWidget(myTableToolBox);

    myTabStruct = new QTabWidget();
    myTabString = new QTabWidget();
    myTableToolBox->addItem(myTabStruct, "Structures");
    myTableToolBox->addItem(myTabString, "Table Strings");

    //---------

    myPanelToolBox = new QToolBox();
    mySplitter->addWidget(myPanelToolBox);

    myEnumWidget = new QPanelEnum();
    myStringWidget = new QPanelString();
    myStructWidget = new QPanelStruct();
    QPanelSearch* searchPanel = new QPanelSearch();
    myEnumWidget->Init();
    myStringWidget->Init();
    //myStructWidget->Init();
    myPanelToolBox->addItem(myStructWidget, "Structures");
    myPanelToolBox->addItem(myStringWidget, "String Tables");
    myPanelToolBox->addItem(myEnumWidget, "Enumerators");
    myPanelToolBox->addItem(searchPanel, "Search");

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

    CONNECT_DB(AcknowledgeChange);
    CONNECT_DB(AutoSaveFeedback);

#undef CONNECT_DB

    LoadQSettings();
}

MainWindow::~MainWindow()
{}


void MainWindow::AddFileToOpenRecentList(const QString& _openFilePath)
{
    QSettings settings;

    // Get Current file paths
    QStringList filePaths = QStringList();
    if (settings.contains("recentFiles"))
    {
        QVariantList originalVariantList = settings.value("recentFiles").toList();
        for (const auto& filePathAsVar : originalVariantList)
        {
            filePaths.push_back(filePathAsVar.toString());
        }
    }

    // Add the new filepath to the list
    filePaths.removeAll(_openFilePath);
    filePaths.prepend(_openFilePath);

    // Save filepaths in QSettings
    QVariantList variantList;
    for (const QString& item : filePaths)
    {
        // - filter out invalid files
        if (!QFileInfo::exists(item))
        {
            continue;
        }
        QVariant variantItem = QVariant::fromValue(item);
        variantList.append(variantItem);
    }
    // - filter out files that are too old
    if (variantList.count() > MAX_RECENT_FILES)
    {
        variantList = variantList.first(MAX_RECENT_FILES);
    }
    settings.setValue("recentFiles", variantList);

    // Update Actions
    BuildOpenRecentMenu();
}
void MainWindow::BuildOpenRecentMenu()
{
    QSettings settings;
    myOpenRecentMenu->clear();

    // Get Current file paths
    if (!settings.contains("recentFiles"))
    {
        myOpenRecentMenu->setEnabled(false);
    }

    QVariantList originalVariantList = settings.value("recentFiles").toList();
    bool mostRecent = true;
    for (const auto& filePathAsVar : originalVariantList)
    {
        QString filePath = filePathAsVar.toString();
        auto* exportStructJSON = myOpenRecentMenu->addAction(filePath);
        QObject::connect(exportStructJSON, &QAction::triggered, this, [this, filePath]{OpenDB(filePath);});
        if (mostRecent)
        {
            exportStructJSON->setShortcut(QKeySequence(Qt::SHIFT | Qt::CTRL | Qt::Key_O));
            mostRecent = false;
        }
    }
    myOpenRecentMenu->setEnabled(originalVariantList.count() > 0);
}
void MainWindow::BuildExportOneStructTableMenu()
{
    myExportOneStructMenu->clear();
    const int structTableCount = myManager.GetStructuresCount();
    for (int i = 0; i < structTableCount; i++)
    {
        const auto* structTable = myManager.GetStructureTable(i);
        auto* exportStructJSON = myExportOneStructMenu->addAction("Export " + structTable->GetTemplateName());
        QObject::connect(exportStructJSON, &QAction::triggered, this, [this, i]{OnExportOneStructTable_JSON(i);});
    }
    myExportOneStructMenu->setEnabled(structTableCount > 0);
}
void MainWindow::BuildExportOneStringTableMenu()
{
    myExportOneStringMenu->clear();
    const int stringTableCount = myManager.GetStringTableCount();
    for (int i = 0; i < stringTableCount; i++)
    {
        const auto* stringTable = myManager.GetStringTable(i);
        QMenu* exportStringMenu = myExportOneStringMenu->addMenu("Export " + stringTable->GetTableName());
        for (int j = 0; j < SStringHelper::SStringLanguages::Count; j++)
        {
            auto* exportOneStringAction = exportStringMenu->addAction("Export in " + SStringHelper::GetLanguageString((SStringHelper::SStringLanguages)j));
            QObject::connect(exportOneStringAction, &QAction::triggered, this, [this, i, j]{OnExportOneStringTable(i, (SStringHelper::SStringLanguages)j);});
        }

        auto* exportOneStringActionAllLanguages = exportStringMenu->addAction("Export in All Languages");
        QObject::connect(exportOneStringActionAllLanguages, &QAction::triggered, this, [this, i]{OnExportOneStringTable(i, SStringHelper::SStringLanguages::Count);});
    }
    myExportOneStringMenu->setEnabled(stringTableCount > 0);
}


void MainWindow::SaveQSettings() const
{
    QSettings settings;
    settings.setValue("mainWindow/sizeW", size().width());
    settings.setValue("mainWindow/sizeH", size().height());
    settings.setValue("mainWindow/posX", pos().x());
    settings.setValue("mainWindow/posY", pos().y());
    settings.setValue("mainWindow/isMaximized", windowState() == Qt::WindowMaximized);
    settings.setValue("splitter", mySplitter->saveState());
    settings.setValue("focus/table", myTableToolBox->currentIndex());
    settings.setValue("focus/panel", myPanelToolBox->currentIndex());
}
void MainWindow::LoadQSettings()
{
    QSettings settings;
    //settings.clear();

    if (!settings.contains("mainWindow/sizeW"))
    {
        QList<int> splitterDefaultSizes = QList<int>();
        splitterDefaultSizes.push_back(600);
        splitterDefaultSizes.push_back(680);
        mySplitter->setSizes(splitterDefaultSizes);
        resize(1280,720);

        int width = frameGeometry().width();
        int height = frameGeometry().height();
        QScreen *screen = qApp->primaryScreen();
        int screenWidth = screen->geometry().width();
        int screenHeight = screen->geometry().height();
        move((screenWidth - width) / 2, (screenHeight - height) / 2);
        return;
    }

    const bool isMaximized = settings.value("mainWindow/isMaximized").toBool();
    if (!isMaximized)
    {
        resize(settings.value("mainWindow/sizeW").toInt(), settings.value("mainWindow/sizeH").toInt());
        move(settings.value("mainWindow/posX").toInt(), settings.value("mainWindow/posY").toInt());
    }
    else
    {
        resize(1280,720);
    }
    setWindowState(isMaximized ? Qt::WindowMaximized : Qt::WindowNoState);
    mySplitter->restoreState(settings.value("splitter").toByteArray());
    myTableToolBox->setCurrentIndex(settings.value("focus/table").toInt());
    myPanelToolBox->setCurrentIndex(settings.value("focus/panel").toInt());
}
void MainWindow::ResetQSettings()
{
    QSettings settings;
    QVariantList recentFiles;
    if (settings.contains("recentFiles"))
    {
        recentFiles = settings.value("recentFiles").toList();
    }
    settings.clear();
    settings.setValue("recentFiles", recentFiles);

    LoadQSettings();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!myHasUnsavedChanges)
    {
        SaveQSettings();
        event->accept();
        return;
    }

    event->ignore();
    auto res = QMessageBox::question(this, "Closing DB Manager", "Do you want to close the DB Manager ?\nUnsaved change will be lost.", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
    if (res != QMessageBox::Cancel)
    {
        if (res == QMessageBox::Save)
        {
            OnSaveDB();
        }
        SaveQSettings();
        event->accept();
    }
}

void MainWindow::Debug_Update()
{
    myEnumWidget->UpdateItemList();
    myStringWidget->UpdateItemList();
    myStructWidget->UpdateItemList();
}
void MainWindow::UpdateWindowName()
{
    QString windowName = "Anima DataBase Manager";
    if (SaveManager::HasCurrentFile())
    {
        windowName.append(" - ").append(SaveManager::GetCurrentSaveFile());
    }
    if (myHasUnsavedChanges)
    {
        windowName.append("*");
    }
    if (myShowAutoSaveFeedBack)
    {
        windowName.append(" --- Backup Auto-Saved");
    }
    setWindowTitle(windowName);
}
void MainWindow::OpenDB(const QString& _savefile, bool _resetApp)
{
    if (!QFileInfo::exists(_savefile))
    {
        QMessageBox::information(this, "File not found", QString("Failed to open file :\nDoes not exists.\n\n%1").arg(_savefile));
        return;
    }

    if (_resetApp)
    {
        // New -> Reset the db and window
        OnNewDB();
    }

    // Open
    myManager.blockSignals(true);
    SaveManager::OpenFile(_savefile);
    myManager.blockSignals(false);
    OnResetView();
    myHasUnsavedChanges = false;
    UpdateWindowName();
    AddFileToOpenRecentList(_savefile);
}


void MainWindow::OnStringTableAdded(const int _index)
{
    SStringTable* sTable = myManager.GetStringTable(_index);
    if (!sTable)
        return;

    QSStringTable* stringTable = new QSStringTable(_index);
    myTabString->insertTab(_index, stringTable, sTable->GetTableName());
    myTabString->setCurrentIndex(_index);

    BuildExportOneStringTableMenu();
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

    BuildExportOneStringTableMenu();
}
void MainWindow::OnStringTableRemoved(const int _index)
{
    QWidget* tabToDelete = myTabString->widget(_index);
    if (!tabToDelete)
        return;

    myTabString->removeTab(_index);
    delete tabToDelete;

    BuildExportOneStringTableMenu();
}
void MainWindow::OnStringTableRenamed(const int _index, const QString& _name)
{
    myTabString->setTabText(_index, _name);

    BuildExportOneStringTableMenu();
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

    BuildExportOneStructTableMenu();
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

    BuildExportOneStructTableMenu();
}
void MainWindow::OnStructTableRemoved(const int _index)
{
    QWidget* tabToDelete = myTabStruct->widget(_index);
    if (!tabToDelete)
        return;

    myTabStruct->removeTab(_index);
    delete tabToDelete;

    BuildExportOneStructTableMenu();
}
void MainWindow::OnStructTableRenamed(const int _index, const QString& _name)
{
    myTabStruct->setTabText(_index, _name);
    BuildExportOneStructTableMenu();
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

void MainWindow::OnAcknowledgeChange()
{
    if (myHasUnsavedChanges)
    {
        return;
    }
    myHasUnsavedChanges = true;
    UpdateWindowName();
}

void MainWindow::OnAutoSaveFeedback(bool _showFeedback)
{
    myShowAutoSaveFeedBack = _showFeedback;
    UpdateWindowName();
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

    SaveManager::New();
    myHasUnsavedChanges = false;
    UpdateWindowName();

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
    QString filePath = SaveManager::GetCurrentSaveFile();

    if (_saveAs || filePath.isEmpty() || !QFileInfo::exists(filePath))
    {
        if (filePath.isEmpty())
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
    myHasUnsavedChanges = false;
    UpdateWindowName();
    AddFileToOpenRecentList(filePath);
    return true;
}
void MainWindow::OnOpenDB()
{
    // Choose FilePath

    const QString& fileExt = SaveManager::GetSaveFileExtension();
    const QString& currentFilePath = SaveManager::GetCurrentSaveFile();
    QString filePath = currentFilePath;

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
    if (currentFilePath == filePath)
    {
        QMessageBox::StandardButton btn = QMessageBox::question(this, "Refresh opened", "The Database you asked to open is already opened.\nDo you want to reopen it ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (btn == QMessageBox::No)
        {
            return;
        }
    }

    qDebug() << filePath;
    Q_ASSERT(QFileInfo::exists(filePath));

    // Open Internal
    OpenDB(filePath);
}

// ================      Export Methods      ================

void MainWindow::OnExportCurrentStringTable(SStringHelper::SStringLanguages _language)
{
    ExportOneStringTable(myTabString->currentIndex(), _language, "");
}
void MainWindow::OnExportOneStringTable(int _index, SStringHelper::SStringLanguages _language)
{
    ExportOneStringTable(_index, _language, "");
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
        ExportOneStringTable(i, _language, dir);
    }
}
void MainWindow::ExportOneStringTable(int _index, SStringHelper::SStringLanguages _language, QString _dir)
{
    QSStringTable* tab = dynamic_cast<QSStringTable*>(myTabString->widget(_index));
    if(tab == nullptr)
    {
        return;
    }

    if (_dir.isEmpty())
    {
        _dir = QFileDialog::getExistingDirectory(this, "Select Folder to export String Table " + myTabString->tabText(_index),
                                                 myManager.GetProjectContentFolderPath());
        if (_dir.isEmpty())
        {
            return;
        }
    }


    if (_language != SStringHelper::SStringLanguages::Count)
    {
        tab->ExportStringsToCSV(_dir, _language);
    }
    else
    {
        for (int i = 0; i < SStringHelper::SStringLanguages::Count; i++)
        {
            tab->ExportStringsToCSV(_dir, (SStringHelper::SStringLanguages)i);
        }
    }
}
void MainWindow::OnExportCurrentStructTable_JSON()
{
    ExportCurrentStructTable(true);
}
void MainWindow::OnExportOneStructTable_JSON(int _index)
{
    ExportOneStructTable(_index, true, "");
}
void MainWindow::OnExportAllStructTables_JSON()
{
    ExportAllStructTables(true);
}
void MainWindow::OnExportCurrentStructTable_CSV()
{
    ExportCurrentStructTable(false);
}
void MainWindow::OnExportOneStructTable_CSV(int _index)
{
    ExportOneStructTable(_index, false, "");
}
void MainWindow::OnExportAllStructTables_CSV()
{
    ExportAllStructTables(false);
}
void MainWindow::ExportCurrentStructTable(bool _JSON)
{
    ExportOneStructTable(myTabStruct->currentIndex(), _JSON, "");
}
void MainWindow::ExportAllStructTables(bool _JSON)
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
        ExportOneStructTable(i, _JSON, dir);
    }
}
void MainWindow::ExportOneStructTable(int _index, bool _JSON, QString _dir)
{
    QStructureTable* tab = dynamic_cast<QStructureTable*>(myTabStruct->widget(_index));
    if(tab == nullptr)
    {
        return;
    }


    if (_dir.isEmpty())
    {
        _dir = QFileDialog::getExistingDirectory(this, "Select Folder to export Structure Table " + myTabStruct->tabText(_index),
                                                        myManager.GetProjectContentFolderPath());
        if (_dir.isEmpty())
        {
            return;
        }
    }



    if (_JSON)
    {
        tab->ExportStructsToJSON(_dir);
    }
    else
    {
        tab->ExportStructsToCSV(_dir);
    }
}
void MainWindow::OnExportAll()
{
    // Todo : change so there is only 1 directory selection for both struct and string exports ?

    OnExportAllStructTables_JSON();
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
