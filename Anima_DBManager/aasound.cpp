#include "aasound.h"

AASound::AASound(const AttributeParam& _sharedParam) :
    AAsset(_sharedParam)
{}

AASound::AASound(const AttributeParam& _sharedParam, QString _filePath) :
    AAsset(_sharedParam, _filePath)
{}


Attribute* AASound::CreateDuplica() const
{
    return new AASound(mySharedParam, filePath);
}

void AASound::WriteValue_CSV(std::ofstream& file) const
{
    qFatal("Need to set the corret prefix");
    file << "??? - Sound - ???";
    file << GetDisplayedText().toStdString();
}
