#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMenuBar>
#include <QTabWidget>
#include <QToolBox>

#include "db_manager.h"
#include "qpanelenum.h"
#include "qpanelstring.h"
#include "qpanelstruct.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    DB_Manager& myManager;
    QString myCurrentlyOpenedFile = "";

    QMenuBar* myMenuBar;
    QToolBox* myTableToolBox;
    QTabWidget* myTabString;
    QTabWidget* myTabStruct;

    QPanelEnum* myEnumWidget;
    QPanelString* myStringWidget;
    QPanelStruct* myStructWidget;

    bool OnSaveDB_Internal(bool _saveAs);
    void CleanTabWidget(QTabWidget* _tabWidget);
    void ExportCurrentStructTable(bool _JSON);
    void ExportAllStructTables(bool _JSON);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Debug_Update();

public slots:
    void OnStringTableAdded(const int _index);
    void OnStringTableMoved(const int _indexFrom, const int _indexTo);
    void OnStringTableRemoved(const int _index);
    void OnStringTableRenamed(const int _index, const QString& _name);
    void OnStringTableChanged(const int _tableIndex);
    void OnStringItemFocus(const int _tableIndex, const int _index, const int _languageIndex, const bool _forceFocus);
    void OnStringItemChanged(const int _tableIndex);

    void OnStructTableAdded(const int _index);
    void OnStructTableMoved(const int _indexFrom, const int _indexTo);
    void OnStructTableRemoved(const int _index);
    void OnStructTableRenamed(const int _index, const QString& _name);
    void OnStructTableIconChanged(const int _index, const QIcon& _color);
    void OnStructTableFocus(const int _tableIndex, const int _itemIndex);
    void OnStructItemFocus(const int _tableIndex, const int _index, const int _attrIndex, const bool _forceFocus);
    void OnStructItemChanged(const int _tableIndex);
    void OnStructAttributeNameChanged(const int _tableIndex);

    void OnResetView();


    bool OnNewDB();
    bool OnSaveDB();
    void OnSaveAsDB();
    void OnOpenDB();

    void OnExportCurrentStringTable(SStringHelper::SStringLanguages _language);
    void OnExportAllStringTables(SStringHelper::SStringLanguages _language);
    void OnExportCurrentStructTable_JSON();
    void OnExportAllStructTables_JSON();
    void OnExportCurrentStructTable_CSV();
    void OnExportAllStructTables_CSV();
    void OnExportAll();

    void OnImportStringTable();
    void OnImportStuctTable();

    void OnProjectSettings();
};
#endif // MAINWINDOW_H
