#include "aamesh.h"

AAMesh::AAMesh(TemplateAttribute& _template, bool _isSkeletal) :
    AAsset(_template),
    isSkeletal(_isSkeletal)
{}

AAMesh::AAMesh(TemplateAttribute& _template, QString _filePath, bool _isSkeletal) :
    AAsset(_template, _filePath),
    isSkeletal(_isSkeletal)
{}


const QString& AAMesh::GetAssetClassNameForCSV() const
{
    static QString staticName = "/Script/Engine.StaticMesh";
    static QString skeletalName = "/Script/Engine.SkeletalMesh";
    return isSkeletal ? staticName : skeletalName;
}
const QString& AAMesh::GetAssetFileExtension() const
{
    return AAMesh::GetStaticAssetFileExtension();
}
const QString& AAMesh::GetStaticAssetFileExtension()
{
    static QString ext = "fbx";
    return ext;
}
