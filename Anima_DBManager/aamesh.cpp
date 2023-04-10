#include "aamesh.h"

AAMesh::AAMesh(const AttributeParam& _sharedParam, bool _isSkeletal) :
    AAsset(_sharedParam),
    isSkeletal(_isSkeletal)
{}

AAMesh::AAMesh(const AttributeParam& _sharedParam, QString _filePath, bool _isSkeletal) :
    AAsset(_sharedParam, _filePath),
    isSkeletal(_isSkeletal)
{}


Attribute* AAMesh::CreateDuplica() const
{
    return new AAMesh(mySharedParam, filePath, isSkeletal);
}

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
