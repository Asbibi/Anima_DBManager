#include "aasound.h"

AASound::AASound(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam)
{}

AASound::AASound(const AttributeParam& _sharedParam, QString _filePath) :
    AAsset(_sharedParam, _filePath)
{}


Attribute* AASound::CreateDuplica() const
{
    return new AASound(mySharedParam, myFilePath);
}


const QString& AASound::GetAssetClassNameForCSV() const
{
    static QString className = "/Script/Engine.SoundWave";
    return className;
}
const QString& AASound::GetAssetFileExtension() const
{
    return AASound::GetStaticAssetFileExtension();
}
const QString& AASound::GetStaticAssetFileExtension()
{
    static QString ext = "wav";
    return ext;
}
