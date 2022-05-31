#include "aasset.h"

#include "db_manager.h"


AAsset::AAsset() :
    AAsset("")
{}

AAsset::AAsset(QString _filePath) :
    Attribute(nullptr),
    filePath(_filePath)
{}


QString AAsset::GetDisplayedText(bool complete) const
{
    if (complete)
        return filePath;

    return GetFilePathForDisplay(filePath);
}

void AAsset::WriteValue_CSV(std::ofstream& file) const
{
    if (filePath.isEmpty())
    {
        file << "\'\'";
        return;
    }

    QString editedPath = filePath;
    editedPath = editedPath.replace(DB_Manager::GetDB_Manager().GetProjectContentFolderPath(), "/Game");
    const int pointIndex = editedPath.lastIndexOf('.');
    editedPath = editedPath.left(pointIndex);
    const int lastSepIndex = editedPath.lastIndexOf('/');
    const int size = editedPath.length();
    QString assetName = editedPath.right(size - lastSepIndex - 1);

    file << '\'';
    file << editedPath.toStdString();
    file << '.';
    file << assetName.toStdString();
    file << '\'';
}

void AAsset::SetValueFromText(const QString& text)
{
    if (text.isEmpty() || text.contains(DB_Manager::GetDB_Manager().GetProjectContentFolderPath()))
        filePath = text;

    else
        qFatal("\n\nFile Path given when setting <ASSET> Attribute's value isn't in project folder:\n\n\t===== Abort =====\n\n");
}


QString AAsset::GetFilePathForDisplay(const QString& _filePath)
{
    if (_filePath.length() <= 15)
        return _filePath;

    return "<font color=\"darkgreen\">...</font>" + _filePath.right(15);
}
