#include "aasset.h"

#include "db_manager.h"


AAsset::AAsset(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam, "")
{}

AAsset::AAsset(const AttributeParam& _sharedParam, QString _filePath) :
    Attribute(_sharedParam),
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

void AAsset::CopyValueFromOther(const Attribute* _other)
{
    if (!_other || GetType() != _other->GetType())
        return;

    const AAsset* other_AA = dynamic_cast<const AAsset*>(_other);
    filePath = other_AA->filePath;
}


QString AAsset::GetFilePathForDisplay(const QString& _filePath)
{
    if (_filePath.length() <= 15)
        return _filePath;

    return "<font color=\"darkgreen\">...</font>" + _filePath.right(15);
}
