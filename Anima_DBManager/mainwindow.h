#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMenuBar>
#include <QTabWidget>
#include <QToolBox>

#include "db_manager.h"
#include "qpanelenum.h"
#include "qpanelstring.h"


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
    QWidget* myStructWidget;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Debug_Update();

public slots:
    void OnStringTableAdded(const int _index);
    void OnStringTableMoved(const int _indexFrom, const int _indexTo);
    void OnStringTableRemoved(const int _index);
    void OnStringTableRenamed(const int _index, const QString& _name);
};
#endif // MAINWINDOW_H
