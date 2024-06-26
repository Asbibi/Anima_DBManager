#include "qassetpreviewdialog.h"

#include "db_manager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>

QAssetPreviewDialog::QAssetPreviewDialog(const AttributeTypeHelper::Type& assetType,
                                         const QString& dialogTitle,
                                         const QString& dialogExtensions,
                                         QString& filePath,
                                         QWidget* parent) :
    QDialog(parent),
    myFilePath(filePath),
    myDialogTitle(dialogTitle),
    myDialogExtensions(dialogExtensions)
{
    // ----------
    QString title = " Asset Details";
    setModal(true);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    setLayout(vLayout);

    // ----------
    myOpenFileButton = new QPushButton("Select another asset", this);
    QObject::connect(myOpenFileButton, &QPushButton::clicked, this, &QAssetPreviewDialog::ChangeFilePath);
    vLayout->addWidget(myOpenFileButton);

    // ----------
    switch (assetType)
    {
        case AttributeTypeHelper::Type::Texture:
        {
            myTexturePreview = new QAPTexture();
            myTexturePreview->SetFilePath(filePath);
            vLayout->addWidget(myTexturePreview);
            QObject::connect(this, &QAssetPreviewDialog::FilePathChanged, myTexturePreview, &QAPTexture::SetFilePath);
            title = "Texture" + title;
            break;
        }
        case AttributeTypeHelper::Type::Sound:
        {
            mySoundPreview = new QAPSound();
            mySoundPreview->SetFilePath(filePath);
            vLayout->addWidget(mySoundPreview);
            QObject::connect(this, &QAssetPreviewDialog::FilePathChanged, mySoundPreview, &QAPSound::SetFilePath);
            title = "Sound" + title;
            break;
        }
        case AttributeTypeHelper::Type::SkeletalMesh:
        case AttributeTypeHelper::Type::StaticMesh:
        {
            myMeshPreview = new QAPMesh();
            myMeshPreview->SetFilePath(filePath);
            vLayout->addWidget(myMeshPreview);
            QObject::connect(this, &QAssetPreviewDialog::FilePathChanged, myMeshPreview, &QAPMesh::SetFilePath);
            title = QString(assetType == AttributeTypeHelper::Type::SkeletalMesh ? "Skeletal" : "Static") + " Mesh" + title;
            break;
        }
        default:
            break;
    }

    // ----------
    QWidget* btnWidget = new QWidget();
    QPushButton* applyBtn = new QPushButton("Apply");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QHBoxLayout* hLayoutBtn = new QHBoxLayout(this);
    btnWidget->setLayout(hLayoutBtn);
    hLayoutBtn->addWidget(applyBtn);
    QObject::connect(applyBtn, &QPushButton::clicked, this, &QDialog::accept);
    hLayoutBtn->addWidget(cancelBtn);
    QObject::connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    vLayout->addWidget(btnWidget);

    // ----------
    setWindowTitle(title);
}

void QAssetPreviewDialog::ChangeFilePath()
{
    QString fileName = QFileDialog::getOpenFileName(this, myDialogTitle, myFilePath.isEmpty() ? DB_Manager::GetDB_Manager().GetProjectContentFolderPath() : myFilePath, myDialogExtensions);
    if (fileName.isEmpty() || fileName == myFilePath)
        return;

    myFilePath = fileName;
    emit FilePathChanged(myFilePath);
}
