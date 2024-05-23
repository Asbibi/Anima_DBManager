#include "aamesh.h"

AAMesh::AAMesh(TemplateAttribute& _template) :
    AAsset(_template)
{}

AAMesh::AAMesh(TemplateAttribute& _template, QString _filePath) :
    AAsset(_template, _filePath)
{}


#ifdef USE_SPECIFIC_FILE_EXTENSION_FOR_AAMESH
const QString& AAMesh::GetAssetFileExtension() const
{
    return AAMesh::GetStaticAssetFileExtension();
}
const QString& AAMesh::GetStaticAssetFileExtension()
{
    static QString ext = "fbx";
    return ext;
}
#endif
