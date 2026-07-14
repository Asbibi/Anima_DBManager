#include "qaboutdialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include "iconmanager.h"

#define GITHUB_LINK "https://github.com/Asbibi/Anima_DBManager.git"
#define YOUTUBE_LINK "https://youtu.be/xTZ1IaC-U5w?si=iECKNyEqeSlOkpPz"

QAboutDialog::QAboutDialog(QWidget* _parent) : QDialog(_parent)
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    auto* appTitle = new QLabel("ANIMA DATABASE MANAGER");
    appTitle->setStyleSheet("font-weight: bold; font-size: 25pt;");
    vLayout->addWidget(appTitle, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    vLayout->addSpacing(50);

    QPixmap iconPixmap = IconManager::GetAppIcon().pixmap(QSize(256, 256));
    QLabel* appIcon = new QLabel();
    appIcon->setPixmap(iconPixmap);
    vLayout->addWidget(appIcon, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    vLayout->addSpacing(30);

    QLabel* appVersion = new QLabel("Version : v1.5");
    vLayout->addWidget(appVersion, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    vLayout->addSpacing(10);
    QLabel* credit = new QLabel("Developped by : Asbibi <66073617+Asbibi@users.noreply.github.com>");
    vLayout->addWidget(credit, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    QLabel* qtVersion = new QLabel("Using Qt 6.11.1");
    vLayout->addWidget(qtVersion, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    vLayout->addSpacing(30);

    QLabel* description = new QLabel("The Anima Database Manager is a completely free to use software aiming at handling a database for an Unreal Engine game. It is composed of Structure tables that can be parametrized and in filled in the software to be exported as json files to feed DataTable assets in Unreal.\nIt also presents String tables that help handling the translation for the displayed texts of the game. The string tables can be exported to csv files and used in Unreal Engine (though their usage with localization has to be implemented by the developper). The first string table is a Dictionnary in which words that are commonly used in the texts can be stored (for e.g. a character's name) so if that word has to change, the developper doesn't have to manually update each occurence.\nA search tool is also at the user's disposal to help find data.\n\nThe whole database of Structure and String tables are completely loaded in RAM, there is no SQL tables. The app doesn't feature an UNDO-REDO action for now.");
    description->setWordWrap(true);
    vLayout->addWidget(description, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    vLayout->addSpacing(30);

    QLabel* githubLink = new QLabel("GitHub Repository : <a href='" GITHUB_LINK "'>Anima_DBManager</a>");
    githubLink->setToolTip(GITHUB_LINK);
    githubLink->setOpenExternalLinks(true);
    vLayout->addWidget(githubLink, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    vLayout->addSpacing(10);
    QLabel* ytLink = new QLabel("Youtube Presentation (App version v1.2) : <a href='" YOUTUBE_LINK "'>Anima Database Manager</a>");
    ytLink->setToolTip(YOUTUBE_LINK);
    ytLink->setOpenExternalLinks(true);
    vLayout->addWidget(ytLink, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    vLayout->addSpacing(10);
}
