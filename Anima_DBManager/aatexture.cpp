#include "aatexture.h"

AATexture::AATexture(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam)
{}

AATexture::AATexture(const AttributeParam& _sharedParam, QString _filePath) :
    AAsset(_sharedParam, _filePath)
{}


Attribute* AATexture::CreateDuplica() const
{
    return new AATexture(mySharedParam, filePath);
}


const QString& AATexture::GetAssetClassNameForCSV() const
{
    static QString className = "/Script/Engine.Texture2D";
    return className;
}
const QString& AATexture::GetAssetFileExtension() const
{
    return AATexture::GetStaticAssetFileExtension();
}
const QString& AATexture::GetStaticAssetFileExtension()
{
    static QString ext = "png";
    return ext;
}
