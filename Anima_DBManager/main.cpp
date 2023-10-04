#include "mainwindow.h"

#include "db_manager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle("fusion");
    //a.setStyle("macintosh");

    /*QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Anima_DBManager_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }*/

    MainWindow w;
    DB_Manager::GetDB_Manager().Init();
    w.Debug_Update();
    w.show();
    return a.exec();
}
