#include "qassetpreview.h"

#include "db_manager.h"
#include <QVBoxLayout>
#include <QSpacerItem>

QAssetPreview::QAssetPreview(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignCenter);
    vLayout->setContentsMargins(0,0,0,0);
    setLayout(vLayout);

    myAssetPath = new QLabel();
    myAssetPath->setMinimumSize(300, 20);
    vLayout->addWidget(myAssetPath);
    vLayout->addItem(new QSpacerItem(1,3));
}

void QAssetPreview::SetFilePath(const QString& _filePath)
{
    QString shortFilePath = _filePath;
    shortFilePath.replace(DB_Manager::GetDB_Manager().GetProjectContentFolderPath(), "<font color=\"darkgreen\">PROJECT_FOLDER/</font>");
    myAssetPath->setText(shortFilePath);
}
