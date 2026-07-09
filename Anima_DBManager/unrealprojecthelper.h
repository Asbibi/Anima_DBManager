#ifndef UNREALPROJECTHELPER_H
#define UNREALPROJECTHELPER_H

#include <QString>


namespace UnrealProjectHelper {

const QString& GetUnrealContentFolder();

bool IsPathValidUnrealProject(const QString& _path);

QString GetAbsolutePathFromRelative(const QString& _saveFilePath, const QString& _relativeprojectPath);
QString GetRelativePathFromAbsolute(const QString& _saveFilePath, const QString& _absoluteprojectPath);

}

#endif // UNREALPROJECTHELPER_H
