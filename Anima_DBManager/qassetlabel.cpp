#include "qassetlabel.h"

#include "aasset.h"
#include "db_manager.h"

#include "qassetpreviewdialog.h"
#include <QFileDialog>
#include <QMessageBox>

QAssetLabel::QAssetLabel(const AttributeTypeHelper::Type assetType, const QString& dialogTitle, const QString& dialogExtensions, QWidget* parent) :
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
    setText(myFilePath.isEmpty() ? "" : AAsset::GetFilePathForDisplay(_filePath, _filePath[0] == '!'));
}

const QString& QAssetLabel::GetValue() const
{
    return myFilePath;
}



void QAssetLabel::OpenFileDialog()
{
    QString fileName = myFilePath;

    if (myAssetType == AttributeTypeHelper::Type::Niagara || myAssetType == AttributeTypeHelper::Type::AnimInstance || myFilePath.isEmpty())
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


    auto& dbManager = DB_Manager::GetDB_Manager();
    const QString& projectFolder = dbManager.GetProjectContentFolderPath();
    if (AAsset::IsDirty(projectFolder))
    {
        QString warningtext = dbManager.IsProjectContentFolderPathValid() ?
                    "Selected asset isn't in the Project Folder.\nProject path : " + projectFolder :
                    "There is currently no valid Project Folder.";

        warningtext += "\nIf Ignore, asset will be set but ignored during export to csv.";
        auto btn = QMessageBox::warning(
            this,
            "Warning",
            warningtext,
            QMessageBox::StandardButtons(QMessageBox::Ignore | QMessageBox::Abort) );
        if (btn != QMessageBox::Ignore)
        {
            return;
        }
        fileName = '!' + fileName;
    }

    SetValue(fileName);
    emit OnValueEdited();
}
