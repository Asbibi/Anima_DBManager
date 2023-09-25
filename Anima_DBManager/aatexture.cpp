#include "aatexture.h"

AATexture::AATexture(TemplateAttribute& _template) :
    AAsset(_template)
{}

AATexture::AATexture(TemplateAttribute& _template, QString _filePath) :
    AAsset(_template, _filePath)
{}


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
QPixmap AATexture::GetPixmapFromAssetPath(const QString& _texturePath)
{
    QString actualPath = _texturePath.isEmpty() || _texturePath[0] != '!' ? _texturePath : _texturePath.right(_texturePath.length() -1);
    return QPixmap(actualPath);
}
