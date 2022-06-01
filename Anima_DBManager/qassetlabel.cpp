#include "qassetlabel.h"

#include "aasset.h"
#include "db_manager.h"

#include "qassetpreviewdialog.h"
#include <QFileDialog>

QAssetLabel::QAssetLabel(const Attribute::Type assetType, const QString& dialogTitle, const QString& dialogExtensions, QWidget* parent) :
    QLabel(parent),
    myAssetType(assetType),
    myDialogTitle("Select a " + dialogTitle + " asset"),
    myDialogExtensions(dialogExtensions)
{
    setMinimumWidth(100);
}

void QAssetLabel::SetValue(const QString& _filePath)
{
    myFilePath = _filePath;
    setText(AAsset::GetFilePathForDisplay(_filePath));
}

const QString& QAssetLabel::GetValue() const
{
    return myFilePath;
}



void QAssetLabel::OpenFileDialog()
{
    QString fileName = myFilePath;

    if (myAssetType == Attribute::Type::Niagara || myAssetType == Attribute::Type::AnimInstance || myFilePath.isEmpty())
    {
        fileName = QFileDialog::getOpenFileName(this, myDialogTitle, myFilePath.isEmpty() ? DB_Manager::GetDB_Manager().GetProjectContentFolderPath() : myFilePath, myDialogExtensions);
    }
    else
    {
        auto* dialog = new QAssetPreviewDialog(myAssetType, myDialogTitle, myDialogExtensions, fileName, this);
        dialog->exec();
        int res = dialog->result();
        if (res == QDialog::Rejected)
        {
            fileName = "";
        }
        delete dialog;
    }

    if (fileName.isEmpty() || fileName == myFilePath)
        return;

    SetValue(fileName);
    emit OnValueEdited();
}
