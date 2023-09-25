#include "aasound.h"

AASound::AASound(TemplateAttribute& _template) :
    AAsset(_template)
{}

AASound::AASound(TemplateAttribute& _template, QString _filePath) :
    AAsset(_template, _filePath)
{}


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
