#include "aasset.h"

#include "db_manager.h"


AAsset::AAsset(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam, "")
{}

AAsset::AAsset(const AttributeParam& _sharedParam, QString _filePath) :
    Attribute(_sharedParam),
    filePath(_filePath),
    isDirty(IsDirty(_filePath))
{}


Attribute* AAsset::CreateDuplica() const
{
    return new AAsset(mySharedParam, filePath);
}

QString AAsset::GetDisplayedText(bool complete) const
{
    if (complete)
        return filePath;

    return GetFilePathForDisplay(filePath, isDirty);
}

void AAsset::WriteValue_CSV(std::ofstream& file) const
{
    if (filePath.isEmpty() || isDirty)
    {
        if (isDirty)
        {
            qWarning("Asset ignored because dirty");
        }
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

    file << GetAssetClassNameForCSV().toStdString();
    file << '\'';
    file << editedPath.toStdString();
    file << '.';
    file << assetName.toStdString();
    file << '\'';
}

void AAsset::SetValueFromText(const QString& text)
{
    if (text.isEmpty())
    {
        filePath = text;
        isDirty = false;
        return;
    }

    isDirty = text[0] == '!';
    filePath = isDirty ? text.right(text.length() -1) : text;
    Q_ASSERT(isDirty == IsDirty(filePath));
}

void AAsset::CopyValueFromOther(const Attribute* _other)
{
    if (!_other || GetType() != _other->GetType())
        return;

    const AAsset* other_AA = dynamic_cast<const AAsset*>(_other);
    filePath = other_AA->filePath;
    isDirty = other_AA->isDirty;
}

void AAsset::ReadValue_CSV(const QString& text)
{    
    if(text.isEmpty() || text.endsWith("\'\'"))
    {
        SetValueFromText("");
    }

    const QString& className = GetAssetClassNameForCSV();
    const DB_Manager& dbManager = DB_Manager::GetDB_Manager();
    if (!dbManager.IsProjectContentFolderPathValid())
    {
        // Log ? MessageBox ?
        return;
    }

    if (!text.startsWith(className))
    {
        // Log ? Message Box ?
        return;
    }

    QString assetPath = text.section('\'', 1, -1);
    if (assetPath.isEmpty())
    {
        SetValueFromText(assetPath);
        return;
    }

    // replacing "\Game\" in the path by the project directory path
    assetPath.replace(0, 5, dbManager.GetProjectContentFolderPath());

    // restoring the extension
    const int pointIndex = assetPath.lastIndexOf('.');
    assetPath = assetPath.left(pointIndex);
    assetPath.append(GetAssetFileExtension());

    SetValueFromText(assetPath);
}


const QString& AAsset::GetAssetClassNameForCSV() const
{
    // May need to subclass AAset because of this value (like for niagara objects)
    static QString className = "/Script/Engine.BlueprintGeneratedClass";
    return className;
}
const QString& AAsset::GetAssetFileExtension() const
{
    return AAsset::GetStaticAssetFileExtension();
}
const QString& AAsset::GetStaticAssetFileExtension()
{
    static QString ext = "uasset";
    return ext;
}


QString AAsset::GetFilePathForDisplay(const QString& _filePath, bool _isDirty)
{
    if (_filePath.length() <= 15)
        return _isDirty ? "<font color=\"red\">" + _filePath + "</font>" : _filePath;

    return _isDirty ?
                "<font color=\"red\">..." + _filePath.right(15) + "</font>"
              : "<font color=\"darkgreen\">...</font>" + _filePath.right(15);
}

bool AAsset::IsDirty(const QString& _filePath)
{
    auto& dbManager = DB_Manager::GetDB_Manager();
    return !(dbManager.IsProjectContentFolderPathValid()
                && _filePath.contains(dbManager.GetProjectContentFolderPath()));
}
