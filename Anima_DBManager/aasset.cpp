#include "aasset.h"

#include "db_manager.h"


const QString AAsset::noPathValue = "None";

AAsset::AAsset(TemplateAttribute& _template) :
    AAsset(_template, "")
{}

AAsset::AAsset(TemplateAttribute& _template, QString _filePath) :
    Attribute(_template),
    myFilePath(_filePath),
    myIsDirty(IsDirty(_filePath))
{}


bool AAsset::IsDirty() const
{
    return myIsDirty;
}
bool AAsset::IsEmpty() const
{
    return myFilePath.isEmpty();
}
const QString& AAsset::GetFilePath() const
{
    return myFilePath;
}
bool AAsset::UpdateIsDirty()
{
    bool newDirty = IsDirty(myFilePath);
    bool changed = (newDirty != myIsDirty);
    myIsDirty = newDirty;
    return changed;
}
void AAsset::SetValueFromText(const QString& text)
{
    if (text.isEmpty() || text == noPathValue)
    {
        myFilePath = "";
        myIsDirty = false;
        return;
    }

    myIsDirty = text[0] == '!';
    myFilePath = myIsDirty ? text.right(text.length() -1) : text;
    Q_ASSERT(myIsDirty == IsDirty(myFilePath));
}


QString AAsset::GetDisplayedText() const
{
    return GetFilePathForDisplay(myFilePath, myIsDirty);
}
QString AAsset::GetValue_CSV() const
{
    if (myFilePath.isEmpty() || myIsDirty)
    {
        if (myIsDirty)
        {
            qWarning("Asset ignored because dirty");
        }
        return noPathValue;
    }

    QString editedPath = myFilePath;
    editedPath = editedPath.replace(DB_Manager::GetDB_Manager().GetProjectContentFolderPath(), "/Game");
    const int pointIndex = editedPath.lastIndexOf('.');
    editedPath = editedPath.left(pointIndex);
    const int lastSepIndex = editedPath.lastIndexOf('/');
    const int size = editedPath.length();
    QString assetName = editedPath.right(size - lastSepIndex - 1);

    return GetAssetClassNameForCSV() + '\''
            + editedPath + '.'
            + assetName + '\'';
}
QJsonValue AAsset::GetValue_JSON() const
{
    return QJsonValue(GetValue_CSV());
}
QString AAsset::GetValue_String() const
{
    return myFilePath.section('/', -1);
}


void AAsset::CopyValueFromOther(const Attribute* _other)
{
    if (!_other || GetType() != _other->GetType())
        return;

    const AAsset* other_AA = dynamic_cast<const AAsset*>(_other);
    myFilePath = other_AA->myFilePath;
    myIsDirty = other_AA->myIsDirty;
}
bool AAsset::SetValue_JSON(const QJsonValue& _value)
{
    if (!_value.isString())
    {
        return false;
    }

    SetValue_CSV(_value.toString());
    return true;
}
void AAsset::SetValue_CSV(const QString& text)
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
        SetValueFromText(text);
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
    Q_ASSERT(pointIndex != -1);
    assetPath = assetPath.left(pointIndex + 1);
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
