#include "qassetlabel.h"

#include "aasset.h"
#include "db_manager.h"
#include <QFileDialog>

QAssetLabel::QAssetLabel(const QString& dialogTitle, const QString& dialogExtensions, QWidget* parent) :
    QLabel(parent),
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
    QString fileName = QFileDialog::getOpenFileName(this, myDialogTitle, myFilePath.isEmpty() ? DB_Manager::GetDB_Manager().GetProjectContentFolderPath() : myFilePath, myDialogExtensions);
    //QFileDialog* dialog = new QFileDialog(this);

    if (fileName.isEmpty() || fileName == myFilePath)
        return;

    SetValue(fileName);
    emit OnValueEdited();
}
