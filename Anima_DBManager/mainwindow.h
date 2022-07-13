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

    QMenuBar* myMenuBar;
    QTabWidget* myTabString;
    QTabWidget* myTabStruct;

    QPanelEnum* myEnumWidget;
    QPanelString* myStringWidget;
    QPanelStruct* myStructWidget;


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
    void OnStringItemFocus(const int _tableIndex, const int _index);
    void OnStringItemChanged(const int _tableIndex);

    void OnStructTableAdded(const int _index);
    void OnStructTableMoved(const int _indexFrom, const int _indexTo);
    void OnStructTableRemoved(const int _index);
    void OnStructTableRenamed(const int _index, const QString& _name);
    void OnStructTableChanged(const int _tableIndex);
    void OnStructItemFocus(const int _tableIndex, const int _index);
    void OnStructItemChanged(const int _tableIndex);
};
#endif // MAINWINDOW_H
