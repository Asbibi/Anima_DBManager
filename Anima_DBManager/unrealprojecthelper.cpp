#include "unrealprojecthelper.h"

#include <QDir>
#include <QFileInfo>
#include <QStringList>


const QString& UnrealProjectHelper::GetUnrealContentFolder()
{
    const static QString unrealContentFolder = "Content";
    return unrealContentFolder;
}

bool UnrealProjectHelper::IsPathValidUnrealProject(const QString& _path)
{
    const QDir dir = QDir(_path);
    if (!dir.exists())
    {
        return false;
    }

    if (!QDir(_path + '/' + UnrealProjectHelper::GetUnrealContentFolder()).exists())
    {
        return false;
    }


    QStringList unrealFileFilter;
    unrealFileFilter << "*.uproject";
    QStringList matchingFiles = dir.entryList(unrealFileFilter, QDir::Files);
    if (matchingFiles.isEmpty())
    {
        return false;
    }

    return true;
}

QString UnrealProjectHelper::GetAbsolutePathFromRelative(const QString& _saveFilePath, const QString& _relativeprojectPath)
{
    if (_saveFilePath.isEmpty() || _relativeprojectPath.isEmpty())
    {
        Q_ASSERT(!_saveFilePath.isEmpty());
        return _relativeprojectPath;
    }

    return QDir::cleanPath(QFileInfo(_saveFilePath).absoluteDir().absoluteFilePath(_relativeprojectPath));
}
QString UnrealProjectHelper::GetRelativePathFromAbsolute(const QString& _saveFilePath, const QString& _absoluteprojectPath)
{
    if (_saveFilePath.isEmpty() || _absoluteprojectPath.isEmpty())
    {
        return _absoluteprojectPath;
    }

    QFileInfo saveFileInfo = QFileInfo(_saveFilePath);
    QDir absoluteSaveDir = saveFileInfo.absoluteDir();
    QString relativePathToUnrealProject = absoluteSaveDir.relativeFilePath(_absoluteprojectPath);
    if (!relativePathToUnrealProject.startsWith("../") && !QFileInfo(relativePathToUnrealProject).isAbsolute())
    {
        relativePathToUnrealProject.prepend("./");
    }
    return relativePathToUnrealProject;
}
