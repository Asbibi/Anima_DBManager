#include "mainwindow.h"

#include "constants.h"
#include "db_manager.h"
#include "savemanager.h"

#include <QApplication>
#include <QLocale>
#include <QStyleFactory>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("WindowsVista"));

    MainWindow w;
    DB_Manager::GetDB_Manager().Init();



    if (argc == 2)
    {
        const QString fileName = argv[1];
        if (fileName.endsWith(SaveManager::GetSaveFileExtension()))
        {
#ifdef TEST_VALUES
            SaveManager::New();
#endif
            w.OpenDB(fileName);
        }
    }

#ifdef TEST_VALUES
    w.Debug_Update();
#endif


    w.show();
    return a.exec();
}
